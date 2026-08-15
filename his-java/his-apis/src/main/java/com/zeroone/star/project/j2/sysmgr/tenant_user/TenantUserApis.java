package com.zeroone.star.project.j2.sysmgr.tenant_user;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.RemoveTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.TenantUserDTO;
import com.zeroone.star.project.query.j2.sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：租户用户管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
public interface TenantUserApis {

    /**
     * 获取租户用户列表（条件+分页）
     * @param query 租户用户查询条件
     * @return 租户用户分页数据
     */
    JsonVO<PageDTO<TenantUserDTO>> listTenantUsers(TenantUserQuery query);

    /**
     * 添加租户用户（支持批量）
     * @param data 租户用户添加数据
     * @return 添加结果
     */
    JsonVO<String> addTenantUsers(AddTenantUserDTO data);

    /**
     * 移除租户用户（支持批量）
     * @param data 租户用户移除数据
     * @return 移除结果
     */
    JsonVO<String> removeTenantUsers(RemoveTenantUserDTO data);
}
