package com.zeroone.star.project.sysmgr;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.RemoveTenantUserDTO;
import com.zeroone.star.project.query.j2_sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j2.sysmgr.TenantUserVO;

/**
 * <p>
 * 描述：系统管理接口 - 租户用户管理
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
public interface SysmgrApis {

    /**
     * 获取租户用户列表（条件+分页）
     * @param query 查询条件（租户、用户名、姓名、分页参数）
     * @return 分页租户用户数据
     */
    JsonVO<PageDTO<TenantUserVO>> listTenantUsers(TenantUserQuery query);

    /**
     * 添加租户用户（支持批量）
     * @param dto 租户ID及待添加用户列表
     * @return 添加结果
     */
    JsonVO<String> addTenantUsers(AddTenantUserDTO dto);

    /**
     * 移除租户用户（支持批量）
     * @param dto 租户ID及待移除用户名列表
     * @return 移除结果
     */
    JsonVO<String> removeTenantUsers(RemoveTenantUserDTO dto);
}
