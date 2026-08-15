package com.zeroone.star.sysmgr.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentNameDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO2;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentNameQuery;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentQuery;
import com.zeroone.star.sysmgr.entity.WhaleTenants;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * 租户 服务类
 * </p>
 *
 * @author 鹿
 * @since 2026-07-25
 */
public interface IWhaleTenantsService extends IService<WhaleTenants> {
    /**
     * 查询租户名称列表（轻量）
     * @param rentNameQuery 查询条件
     * @return 租户名称分页数据
     */
    PageDTO<RentNameDTO> queryTenantNames(RentNameQuery rentNameQuery);

    /**
     * 条件分页查询租户列表
     * @param rentQuery 查询条件（编码、关键字、状态、分页）
     * @return 租户信息分页数据
     */
    PageDTO<RentDTO2> queryTenantList(RentQuery rentQuery);

    /**
     * 新增租户
     * @param addDTO 新增租户DTO
     */
    void addRent(RentAddDTO addDTO);

    /**
     * 修改租户
     * @param updateDTO 修改租户DTO
     */
    void updateRent(RentDTO updateDTO);

    /**
     * 逻辑删除租户
     * @param id 租户主键ID
     */
    void deleteRent(String id);
}