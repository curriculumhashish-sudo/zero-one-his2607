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

import java.util.Set;

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

    /**
     * 合法分页大小白名单
     */
    private static final Set<Long> LEGAL_PAGE_SIZES = Set.of(10L, 20L, 50L, 100L);
    /**
     * 默认分页大小
     */
    private static final long DEFAULT_PAGE_SIZE = 10L;
    // @Resource
    // private RedisTemplate<String, Object> redisTemplate;

    // private static final String TENANT_NAMES_CACHE_KEY = "sysmgr:tenant:names";
    // private static final long CACHE_TTL_MINUTES = 30;

    @Override
    public PageDTO<RentNameDTO> queryTenantNames(RentNameQuery rentNameQuery) {
        // 审计日志
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            log.debug("用户[{}]查询租户名称列表", currentUser.getId());
        } catch (Exception e) {
            log.warn("JWT解析失败，无法获取当前用户信息", e);
        }

        // 下拉场景数据量小，固定查询前100条
        Page<WhaleTenants> page = new Page<>(1, 100);
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();

        // 只查需要的3个字段 + 过滤软删除 + 按编码升序
        wrapper.select(WhaleTenants::getId, WhaleTenants::getName, WhaleTenants::getCode)
                .eq(WhaleTenants::getIsDeleted, 0)
                .orderByAsc(WhaleTenants::getCode);

        // 执行查询并转换为RentNameDTO
        Page<WhaleTenants> resultPage = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(resultPage, workbenchMapper::toRentNameDTO);
    }

    @Override
    public PageDTO<RentDTO2> queryTenantList(RentQuery rentQuery) {
        // 从JWT中获取当前登录用户信息
        String currentUserId = null;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            currentUserId = currentUser.getId();
            log.debug("用户[{}]查询租户列表, code={}, keyword={}", currentUserId, rentQuery.getCode(), rentQuery.getRentKeywords());
        } catch (Exception e) {
            log.warn("JWT解析失败，无法获取当前用户信息", e);
        }

        // ========== 分页参数校验与兜底 ==========
        // 页码：小于1则默认第1页
        long pageIndex = rentQuery.getPageIndex() >= 1 ? rentQuery.getPageIndex() : 1;
        // 每页条数：不在白名单内则使用默认值10
        long pageSize = LEGAL_PAGE_SIZES.contains(rentQuery.getPageSize()) ? rentQuery.getPageSize() : DEFAULT_PAGE_SIZE;

        Page<WhaleTenants> page = new Page<>(pageIndex, pageSize);

        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        // 补全所有返回字段，与DTO一一对应
        wrapper.select(
                WhaleTenants::getId,
                WhaleTenants::getName,
                WhaleTenants::getCode,
                WhaleTenants::getDescription,
                WhaleTenants::getIsActive,
                WhaleTenants::getExtraProperties,
                WhaleTenants::getCreatedAt,
                WhaleTenants::getCreatedBy,
                WhaleTenants::getUpdatedAt,
                WhaleTenants::getUpdatedBy
        );

        // 软删除过滤
        wrapper.eq(WhaleTenants::getIsDeleted, 0);

        // 租户编码精确查询
        wrapper.eq(StringUtils.hasText(rentQuery.getCode()), WhaleTenants::getCode, rentQuery.getCode());

        // 状态精确查询：Integer类型直接匹配，无需额外转换
        wrapper.eq(rentQuery.getIsActive() != null, WhaleTenants::getIsActive, rentQuery.getIsActive());

        // 关键词模糊查询：匹配租户名称或描述
        wrapper.and(StringUtils.hasText(rentQuery.getRentKeywords()), w -> w
                .like(WhaleTenants::getName, rentQuery.getRentKeywords())
                .or()
                .like(WhaleTenants::getDescription, rentQuery.getRentKeywords())
        );

        // 按租户编码升序排序
        wrapper.orderByAsc(WhaleTenants::getCode);

        // 执行查询
        Page<WhaleTenants> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, workbenchMapper::toRentDTO2);
    }
}
