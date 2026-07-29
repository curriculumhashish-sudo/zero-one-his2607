package com.zeroone.star.sysmgr.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentNameDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO2;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentNameQuery;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentQuery;
import com.zeroone.star.sysmgr.entity.WhaleTenants;
import com.zeroone.star.sysmgr.mapper.WhaleTenantsMapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.sysmgr.service.IWhaleTenantsService;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;

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
    private UserHolder userHolder;

    @Override
    @SuppressWarnings("unchecked")
    public PageDTO<RentNameDTO> queryTenantNames(RentNameQuery rentNameQuery) {
        String currentUserId;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            currentUserId = currentUser.getId();
            log.debug("用户[{}]查询租户名称列表", currentUserId);
        } catch (Exception e) {
            log.warn("JWT解析失败，无法获取当前用户信息", e);
            currentUserId = null;
        }

        Page<WhaleTenants> page = new Page<>(1, 100);
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WhaleTenants::getId, WhaleTenants::getName, WhaleTenants::getCode)
                .eq(WhaleTenants::getIsDeleted, 0)
                .orderByAsc(WhaleTenants::getCode);
        Page<WhaleTenants> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, RentNameDTO::new);
    }

    @Override
    public PageDTO<RentDTO2> queryTenantList(RentQuery rentQuery) {
        String currentUserId;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            currentUserId = currentUser.getId();
            log.debug("用户[{}]查询租户列表, code={}, keyword={}", currentUserId, rentQuery.getCode(), rentQuery.getRentKeywords());
        } catch (Exception e) {
            log.warn("JWT解析失败，无法获取当前用户信息", e);
            currentUserId = null;
        }

        Page<WhaleTenants> page = new Page<>(rentQuery.getPageIndex(), rentQuery.getPageSize());
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WhaleTenants::getId, WhaleTenants::getName, WhaleTenants::getCode,
                        WhaleTenants::getDescription, WhaleTenants::getIsActive)
                .eq(WhaleTenants::getIsDeleted, 0)
                .eq(StringUtils.hasText(rentQuery.getCode()), WhaleTenants::getCode, rentQuery.getCode())
                .eq(rentQuery.getIs_active() != null, WhaleTenants::getIsActive,
                        rentQuery.getIs_active() == 1)
                .and(StringUtils.hasText(rentQuery.getRentKeywords()), w -> w
                        .like(WhaleTenants::getName, rentQuery.getRentKeywords())
                        .or()
                        .like(WhaleTenants::getDescription, rentQuery.getRentKeywords()))
                .orderByAsc(WhaleTenants::getCode);
        Page<WhaleTenants> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, RentDTO2::new);
    }


    @Override
    @Transactional(rollbackFor = Exception.class)
    public void addRent(RentAddDTO addDTO) {
        // 1. 校验租户编码唯一
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WhaleTenants::getCode, addDTO.getTenantCode());
        if(baseMapper.selectCount(wrapper) > 0){
            throw new RuntimeException("租户编码已存在！");
        }

        WhaleTenants tenant = new WhaleTenants();
        tenant.setCode(addDTO.getTenantCode());
        tenant.setName(addDTO.getTenantName());
        tenant.setDescription(addDTO.getTenantDescription());
        try {
            tenant.setIsActive(Integer.parseInt(addDTO.getTenantStatus()));
        }catch (NumberFormatException e){
            throw new RuntimeException("租户状态必须是数字");
        }
        tenant.setIsDeleted(0);
        LocalDateTime now = LocalDateTime.now();
        tenant.setCreatedAt(now);
        tenant.setUpdatedAt(now);
        baseMapper.insert(tenant);
        log.info("新增租户成功，租户编码:{}", addDTO.getTenantCode());
    }

    @Override
    public void updateRent(RentDTO updateDTO) {
        WhaleTenants tenant = new WhaleTenants();
        // 主键，用于updateById
        tenant.setId(updateDTO.getId());
        // 租户名称
        tenant.setName(updateDTO.getTenantName());
        // 租户描述
        tenant.setDescription(updateDTO.getTenantDescription());
        // 状态类型转换（实体isActive为Integer）
        tenant.setIsActive(Integer.parseInt(updateDTO.getTenantStatus()));

        // 根据主键更新
        baseMapper.updateById(tenant);
        log.info("修改租户成功，租户ID:{}", updateDTO.getId());
    }

    @Override
    public void deleteRent(String id) {
        LambdaUpdateWrapper<WhaleTenants> updateWrapper = new LambdaUpdateWrapper<>();
        updateWrapper.eq(WhaleTenants::getId, id)
                .set(WhaleTenants::getIsDeleted, 1);
        baseMapper.update(null, updateWrapper);
        log.info("逻辑删除租户成功，租户ID:{}", id);
    }
}