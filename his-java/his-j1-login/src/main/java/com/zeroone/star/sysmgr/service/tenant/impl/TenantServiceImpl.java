package com.zeroone.star.sysmgr.service.tenant.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.sysmgr.constant.LoginRedisConstant;
import com.zeroone.star.sysmgr.entity.tenant.AdmOrganizationDO;
import com.zeroone.star.sysmgr.entity.tenant.TenantDO;
import com.zeroone.star.sysmgr.entity.tenant.UserTenantDO;
import com.zeroone.star.sysmgr.mapper.tenant.AdmOrganizationMapper;
import com.zeroone.star.sysmgr.mapper.tenant.TenantMapper;
import com.zeroone.star.sysmgr.mapper.tenant.UserTenantMapper;
import com.zeroone.star.sysmgr.service.tenant.TenantService;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import org.springframework.util.StringUtils;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

@Slf4j
@Service
@RequiredArgsConstructor
public class TenantServiceImpl implements TenantService {

    private final TenantMapper TenantMapper;
    private final UserTenantMapper UserTenantMapper;
    private final AdmOrganizationMapper admOrganizationMapper;
    private final RedisTemplate<String, Object> redisTemplate;
    private final ObjectMapper objectMapper;

    // ==================== 租户列表（带缓存） ====================

    @Override
    public List<TenantVO> getTenantList(Long userId) {
        String cacheKey = LoginRedisConstant.loginTenantsKey(String.valueOf(userId));

        // 1. 先读 Redis 缓存
        List<TenantVO> cached = getCache(cacheKey, new TypeReference<List<TenantVO>>() {});
        if (cached != null) {
            return cached;
        }

        // 2. 查 DB：用户-租户关联
        List<UserTenantDO> relations = UserTenantMapper.selectList(
                new LambdaQueryWrapper<UserTenantDO>()
                        .eq(UserTenantDO::getUserId, userId)
                        .eq(UserTenantDO::getIsDeleted, 0)
        );
        if (CollectionUtils.isEmpty(relations)) {
            return Collections.emptyList();
        }

        List<Long> tenantIds = relations.stream()
                .map(UserTenantDO::getTenantId)
                .distinct()
                .collect(Collectors.toList());

        // 3. 查租户详情（启用 + 未删除）
        List<TenantDO> tenants = TenantMapper.selectList(
                new LambdaQueryWrapper<TenantDO>()
                        .in(TenantDO::getId, tenantIds)
                        .eq(TenantDO::getIsActive, 1)
                        .eq(TenantDO::getIsDeleted, 0)
        );

        List<TenantVO> voList = tenants.stream().map(this::convertTenant).collect(Collectors.toList());

        // 4. 写入缓存
        setCache(cacheKey, voList, LoginRedisConstant.TENANT_LIST_TTL);
        return voList;
    }

    private TenantVO convertTenant(TenantDO t) {
        TenantVO vo = new TenantVO();
        vo.setId(t.getId());
        vo.setCode(t.getCode());
        vo.setName(t.getName());
        vo.setDescription(t.getDescription());
        vo.setIsActive(t.getIsActive());
        vo.setExtraProperties(t.getExtraProperties());
        vo.setCreatedAt(t.getCreatedAt());
        vo.setUpdatedAt(t.getUpdatedAt());
        vo.setCreatedBy(t.getCreatedBy());
        vo.setUpdatedBy(t.getUpdatedBy());
        return vo;
    }

    @Override
    public TenantVO getTenantById(Long tenantId) {
        // 使用 selectList + LIMIT 1，避免 whale_tenants 表存在重复 id 时 selectById 报 TooManyResults
        TenantDO tenant = TenantMapper.selectList(
                new LambdaQueryWrapper<TenantDO>()
                        .eq(TenantDO::getId, tenantId)
                        .last("LIMIT 1")
        ).stream().findFirst().orElse(null);
        if (tenant == null || tenant.getIsActive() == null || tenant.getIsActive() != 1 || tenant.getIsDeleted() != 0) {
            return null;
        }
        return convertTenant(tenant);
    }

    // ==================== 医院列表（带缓存） ====================

    @Override
    public List<HospitalVO> getHospitalList(Long tenantId, HospitalQuery query) {
        String cacheKey = LoginRedisConstant.loginHospitalsKey(String.valueOf(tenantId));

        // 1. 先读 Redis 缓存
        List<HospitalVO> cached = getCache(cacheKey, new TypeReference<List<HospitalVO>>() {});
        if (cached != null) {
            // 缓存命中后，内存过滤 keyword
            return filterByKeyword(cached, query);
        }

        // 2. 查 DB
        String tenantIdStr = String.valueOf(tenantId);
        LambdaQueryWrapper<AdmOrganizationDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmOrganizationDO::getTenantId, tenantIdStr)
                .eq(AdmOrganizationDO::getIsDeleted, 0)
                .eq(AdmOrganizationDO::getActiveFlag, 1)
                .eq(AdmOrganizationDO::getStatusEnum, "生效")
                .eq(AdmOrganizationDO::getTypeEnum, "医院")
                .orderByAsc(AdmOrganizationDO::getDisplayOrder)
                .orderByAsc(AdmOrganizationDO::getName);

        List<AdmOrganizationDO> list = admOrganizationMapper.selectList(wrapper);
        List<HospitalVO> voList = list.stream()
                .map(this::convertHospital)
                .filter(vo -> vo.getId() != null) // 过滤掉 ID 转 Long 失败的
                .collect(Collectors.toList());

        // 3. 写入缓存
        setCache(cacheKey, voList, LoginRedisConstant .HOSPITAL_LIST_TTL);

        // 4. 内存过滤 keyword
        return filterByKeyword(voList, query);
    }

    private HospitalVO convertHospital(AdmOrganizationDO h) {
        HospitalVO vo = new HospitalVO();
        // adm_organization.id 是 String(varchar64)，VO 是 Long，尝试转换
        try {
            vo.setId(Long.valueOf(h.getId()));
        } catch (NumberFormatException e) {
            log.warn("医院ID转Long失败, id={}", h.getId());
            return vo; // id 为 null，外层过滤
        }
        vo.setCode(h.getBusNo());
        vo.setName(h.getName());
        vo.setDisplayName(h.getName());
        return vo;
    }

    @Override
    public HospitalVO getHospitalById(String hospitalId) {
        AdmOrganizationDO org = admOrganizationMapper.selectById(hospitalId);
        if (org == null || org.getActiveFlag() == null || org.getActiveFlag() != 1 || org.getIsDeleted() != 0) {
            return null;
        }
        HospitalVO vo = convertHospital(org);
        if (vo.getId() == null) {
            return null;
        }
        return vo;
    }

    private List<HospitalVO> filterByKeyword(List<HospitalVO> list, HospitalQuery query) {
        if (query == null || !StringUtils.hasText(query.getKeyword())) {
            return list;
        }
        String kw = query.getKeyword().trim();
        return list.stream()
                .filter(vo -> (vo.getName() != null && vo.getName().contains(kw))
                        || (vo.getCode() != null && vo.getCode().contains(kw)))
                .collect(Collectors.toList());
    }

    // ==================== 权限校验 ====================

    @Override
    public boolean checkUserTenant(Long userId, Long tenantId) {
        Long count = UserTenantMapper.selectCount(
                new LambdaQueryWrapper<UserTenantDO>()
                        .eq(UserTenantDO::getUserId, userId)
                        .eq(UserTenantDO::getTenantId, tenantId)
                        .eq(UserTenantDO::getIsDeleted, 0)
        );
        return count != null && count > 0;
    }

    @Override
    public boolean checkHospitalBelongsToTenant(String hospitalId, Long tenantId) {
        String tenantIdStr = String.valueOf(tenantId);
        Long count = admOrganizationMapper.selectCount(
                new LambdaQueryWrapper<AdmOrganizationDO>()
                        .eq(AdmOrganizationDO::getId, hospitalId)
                        .eq(AdmOrganizationDO::getTenantId, tenantIdStr)
                        .eq(AdmOrganizationDO::getIsDeleted, 0)
                        .eq(AdmOrganizationDO::getActiveFlag, 1)
        );
        return count != null && count > 0;
    }

    // ==================== Redis 选择状态写入 ====================

    @Override
    public void cacheSelectedTenant(String userId, Long tenantId) {
        try {
            String key = LoginRedisConstant.userSessionKey(userId);
            redisTemplate.opsForHash().put(key, "tenant_id", tenantId.toString());
            redisTemplate.expire(key, LoginRedisConstant.USER_SESSION_TTL, TimeUnit.SECONDS);

                // 同步更新 his:login:context（如果存在）
            String ctxKey = LoginRedisConstant.loginContextKey(userId);
            if (Boolean.TRUE.equals(redisTemplate.hasKey(ctxKey))) {
                redisTemplate.opsForHash().put(ctxKey, "tenantId", tenantId.toString());
                redisTemplate.expire(ctxKey, LoginRedisConstant.USER_SESSION_TTL, TimeUnit.SECONDS);
            }
        } catch (Exception e) {
            log.warn("缓存租户选择失败, userId={}, tenantId={}", userId, tenantId, e);
        }
    }

    @Override
    public void cacheSelectedHospital(String userId, String hospitalId) {
        try {
            String key = LoginRedisConstant.userSessionKey(userId);
            redisTemplate.opsForHash().put(key, "hospital_organization_id", hospitalId);
            redisTemplate.expire(key, LoginRedisConstant.USER_SESSION_TTL, TimeUnit.SECONDS);

            // 同步更新 his:login:context（如果存在）
            String ctxKey = LoginRedisConstant.loginContextKey(userId);
            if (Boolean.TRUE.equals(redisTemplate.hasKey(ctxKey))) {
                redisTemplate.opsForHash().put(ctxKey, "hospitalId", hospitalId);
                redisTemplate.expire(ctxKey, LoginRedisConstant.USER_SESSION_TTL, TimeUnit.SECONDS);
            }
        } catch (Exception e) {
            log.warn("缓存医院选择失败, userId={}, hospitalId={}", userId, hospitalId, e);
        }
    }

    // ==================== Redis 缓存工具方法 ====================

    private <T> T getCache(String key, TypeReference<T> typeRef) {
        try {
            Object cached = redisTemplate.opsForValue().get(key);
            if (cached != null) {
                String json = (String) cached;
                return objectMapper.readValue(json, typeRef);
            }
        } catch (Exception e) {
            log.warn("Redis 缓存读取失败, key={}", key, e);
        }
        return null;
    }

    private void setCache(String key, Object data, long ttl) {
        try {
            String json = objectMapper.writeValueAsString(data);
            redisTemplate.opsForValue().set(key, json, ttl, TimeUnit.SECONDS);
        } catch (Exception e) {
            log.warn("Redis 缓存写入失败, key={}", key, e);
        }
    }
}