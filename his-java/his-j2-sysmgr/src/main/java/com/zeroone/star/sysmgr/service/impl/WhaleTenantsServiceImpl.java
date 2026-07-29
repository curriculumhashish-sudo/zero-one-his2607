package com.zeroone.star.sysmgr.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
// import com.zeroone.star.project.constant.RedisConstant;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rent.RentDTO2;
import com.zeroone.star.project.dto.j2.sysmgr.rent.RentNameDTO;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentNameQuery;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentQuery;
import com.zeroone.star.sysmgr.entity.WhaleTenants;
import com.zeroone.star.sysmgr.mapper.WhaleTenantsMapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.sysmgr.service.IWhaleTenantsService;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
// import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

// import java.util.concurrent.TimeUnit;

/**
 * <p>
 * 租户 服务实现类
 * </p>
 *
 * @author 鹿
 * @since 2026-07-25
 */
@Slf4j
@Service
public class WhaleTenantsServiceImpl extends ServiceImpl<WhaleTenantsMapper, WhaleTenants> implements IWhaleTenantsService {

    @Resource
    private WorkbenchMapper workbenchMapper;

    @Resource
    private UserHolder userHolder;

    // @Resource
    // private RedisTemplate<String, Object> redisTemplate;

    // private static final String TENANT_NAMES_CACHE_KEY = "sysmgr:tenant:names";
    // private static final long CACHE_TTL_MINUTES = 30;

    @Override
    @SuppressWarnings("unchecked")
    public PageDTO<RentNameDTO> queryTenantNames(RentNameQuery rentNameQuery) {
        // 从JWT中获取当前登录用户信息（可用于审计日志或权限校验）
        String currentUserId = null;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            currentUserId = currentUser.getId();
            log.debug("用户[{}]查询租户名称列表", currentUserId);
        } catch (Exception e) {
            log.warn("JWT解析失败，无法获取当前用户信息", e);
        }

        // Redis缓存：先查缓存，命中则直接返回
        // Object cached = redisTemplate.opsForValue().get(TENANT_NAMES_CACHE_KEY);
        // if (cached instanceof PageDTO<?> cachedPage) {
        //     log.debug("命中租户名称缓存, operator={}", currentUserId);
        //     return (PageDTO<RentNameDTO>) cachedPage;
        // }

        Page<WhaleTenants> page = new Page<>(1, 100);
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WhaleTenants::getId, WhaleTenants::getName, WhaleTenants::getCode)
                .eq(WhaleTenants::getIsDeleted, false)
                .orderByAsc(WhaleTenants::getCode);
        Page<WhaleTenants> result = baseMapper.selectPage(page, wrapper);
        PageDTO<RentNameDTO> pageDTO = PageDTO.create(result, workbenchMapper::toRentNameDTO);

        // Redis缓存：未命中则写入缓存
        // redisTemplate.opsForValue().set(TENANT_NAMES_CACHE_KEY, pageDTO, CACHE_TTL_MINUTES, TimeUnit.MINUTES);
        // log.debug("租户名称数据已缓存, TTL={}min", CACHE_TTL_MINUTES);

        return pageDTO;
    }

    @Override
    public PageDTO<RentDTO2> queryTenantList(RentQuery rentQuery) {
        // 从JWT中获取当前登录用户信息（可用于审计日志或权限校验）
        String currentUserId = null;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            currentUserId = currentUser.getId();
            log.debug("用户[{}]查询租户列表, code={}, keyword={}", currentUserId, rentQuery.getCode(), rentQuery.getRentKeywords());
        } catch (Exception e) {
            log.warn("JWT解析失败，无法获取当前用户信息", e);
        }

        // Redis缓存：租户列表为动态条件+分页查询，缓存key需带条件参数
        // String cacheKey = "sysmgr:tenant:list:" + rentQuery.getPageIndex() + ":" + rentQuery.getPageSize()
        //         + ":" + (rentQuery.getCode() != null ? rentQuery.getCode() : "")
        //         + ":" + (rentQuery.getRentKeywords() != null ? rentQuery.getRentKeywords() : "")
        //         + ":" + (rentQuery.getIs_active() != null ? rentQuery.getIs_active() : "");
        // Object cached = redisTemplate.opsForValue().get(cacheKey);
        // if (cached instanceof PageDTO<?> cachedPage) {
        //     log.debug("命中租户列表缓存, operator={}", currentUserId);
        //     return (PageDTO<RentDTO2>) cachedPage;
        // }

        Page<WhaleTenants> page = new Page<>(rentQuery.getPageIndex(), rentQuery.getPageSize());
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WhaleTenants::getId, WhaleTenants::getName, WhaleTenants::getCode,
                        WhaleTenants::getDescription, WhaleTenants::getIsActive)
                .eq(WhaleTenants::getIsDeleted, false)
                .eq(StringUtils.hasText(rentQuery.getCode()), WhaleTenants::getCode, rentQuery.getCode())
                .eq(rentQuery.getIs_active() != null, WhaleTenants::getIsActive,
                        rentQuery.getIs_active() != null && rentQuery.getIs_active() == 1)
                .and(StringUtils.hasText(rentQuery.getRentKeywords()), w -> w
                        .like(WhaleTenants::getName, rentQuery.getRentKeywords())
                        .or()
                        .like(WhaleTenants::getDescription, rentQuery.getRentKeywords()))
                .orderByAsc(WhaleTenants::getCode);
        Page<WhaleTenants> result = baseMapper.selectPage(page, wrapper);
        PageDTO<RentDTO2> pageDTO = PageDTO.create(result, workbenchMapper::toRentDTO2);

        // Redis缓存：未命中则写入缓存
        // redisTemplate.opsForValue().set(cacheKey, pageDTO, CACHE_TTL_MINUTES, TimeUnit.MINUTES);
        // log.debug("租户列表数据已缓存, TTL={}min", CACHE_TTL_MINUTES);

        return pageDTO;
    }
}
