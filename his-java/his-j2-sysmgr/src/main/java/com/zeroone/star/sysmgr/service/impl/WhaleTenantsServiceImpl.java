package com.zeroone.star.sysmgr.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rent.RentDTO2;
import com.zeroone.star.project.dto.j2.sysmgr.rent.RentNameDTO;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentNameQuery;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentQuery;
import com.zeroone.star.sysmgr.entity.WhaleTenants;
import com.zeroone.star.sysmgr.mapper.WhaleTenantsMapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.sysmgr.service.IWhaleTenantsService;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;
/**
 * <p>
 * 租户 服务实现类
 * </p>
 *
 * @author 鹿
 * @since 2026-07-25
 */
@Service
public class WhaleTenantsServiceImpl extends ServiceImpl<WhaleTenantsMapper, WhaleTenants> implements IWhaleTenantsService {

    @Override
    public PageDTO<RentNameDTO> queryTenantNames(RentNameQuery rentNameQuery) {
        Page<WhaleTenants> page = new Page<>(1, 100);
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WhaleTenants::getId, WhaleTenants::getName, WhaleTenants::getCode)
                .eq(WhaleTenants::getIsDeleted, false)
                .orderByAsc(WhaleTenants::getCode);
        Page<WhaleTenants> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, RentNameDTO::new);
    }

    @Override
    public PageDTO<RentDTO2> queryTenantList(RentQuery rentQuery) {
        Page<WhaleTenants> page = new Page<>(rentQuery.getPageIndex(), rentQuery.getPageSize());
        LambdaQueryWrapper<WhaleTenants> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WhaleTenants::getId, WhaleTenants::getName, WhaleTenants::getCode,
                        WhaleTenants::getDescription, WhaleTenants::getIsActive)
                .eq(WhaleTenants::getIsDeleted, false)
                .eq(StringUtils.hasText(rentQuery.getCode()), WhaleTenants::getCode, rentQuery.getCode())
                .eq(rentQuery.getIs_active() != null, WhaleTenants::getIsActive, rentQuery.getIs_active() != null && rentQuery.getIs_active() == 1)
                .and(StringUtils.hasText(rentQuery.getRentKeywords()), w -> w
                        .like(WhaleTenants::getName, rentQuery.getRentKeywords())
                        .or()
                        .like(WhaleTenants::getDescription, rentQuery.getRentKeywords()))
                .orderByAsc(WhaleTenants::getCode);
        Page<WhaleTenants> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, RentDTO2::new);
    }
}
