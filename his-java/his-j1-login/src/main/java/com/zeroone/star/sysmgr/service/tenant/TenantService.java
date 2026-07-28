package com.zeroone.star.sysmgr.service.tenant;

import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;

import java.util.List;

public interface TenantService {

    /**
     * 获取用户可选租户列表（带 Redis 缓存）
     */
    List<TenantVO> getTenantList(Long userId);

    /**
     * 校验用户是否有指定租户权限
     */
    boolean checkUserTenant(Long userId, Long tenantId);

    /**
     * 获取租户下的医院列表（带 Redis 缓存）
     */
    List<HospitalVO> getHospitalList(Long tenantId, HospitalQuery query);

    /**
     * 校验医院是否属于指定租户且状态正常
     */
    boolean checkHospitalBelongsToTenant(String hospitalId, Long tenantId);

    /**
     * 缓存用户选中的租户到 Redis（user:session & his:login:context）
     */
    void cacheSelectedTenant(String userId, Long tenantId);

    /**
     * 缓存用户选中的医院到 Redis（user:session & his:login:context）
     */
    void cacheSelectedHospital(String userId, String hospitalId);
}