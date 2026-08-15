package com.zeroone.star.sysmgr.service.tenant;

import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;

import java.util.List;

public interface TenantService {

    /**
     * 获取用户可选租户列表（带 Redis 缓存）
     */
    List<TenantVO> getTenantList(String userId);

    /**
     * 根据ID获取单个租户信息
     */
    TenantVO getTenantById(String tenantId);

    /**
     * 校验用户是否有指定租户权限
     */
    boolean checkUserTenant(String userId, String tenantId);

    /**
     * 获取租户下的医院列表（带 Redis 缓存）
     */
    List<HospitalVO> getHospitalList(String tenantId, HospitalQuery query);

    /**
     * 根据ID获取单个医院信息
     */
    HospitalVO getHospitalById(String hospitalId);

    /**
     * 校验医院是否属于指定租户且状态正常
     */
    boolean checkHospitalBelongsToTenant(String hospitalId, String tenantId);

    /**
     * 缓存用户选中的租户到 Redis（user:session & his:login:context）
     */
    void cacheSelectedTenant(String userId, String tenantId);

    /**
     * 缓存用户选中的医院到 Redis（user:session & his:login:context）
     */
    void cacheSelectedHospital(String userId, String hospitalId);
}