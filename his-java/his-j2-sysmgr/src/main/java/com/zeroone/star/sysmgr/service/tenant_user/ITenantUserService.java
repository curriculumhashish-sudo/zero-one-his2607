package com.zeroone.star.sysmgr.service.tenant_user;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.RemoveTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.TenantUserDTO;
import com.zeroone.star.project.query.j2.sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;

public interface ITenantUserService {

    JsonVO<PageDTO<TenantUserDTO>> listTenantUsers(TenantUserQuery query);

    JsonVO<String> addTenantUsers(AddTenantUserDTO data);

    JsonVO<String> removeTenantUsers(RemoveTenantUserDTO data);
}
