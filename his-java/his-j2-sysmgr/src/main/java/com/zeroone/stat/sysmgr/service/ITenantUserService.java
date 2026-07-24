package com.zeroone.stat.sysmgr.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.RemoveTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.TenantUserDTO;
import com.zeroone.star.project.query.j2_sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;

public interface ITenantUserService {

    JsonVO<PageDTO<TenantUserDTO>> listTenantUsers(TenantUserQuery query);

    JsonVO<String> addTenantUsers(AddTenantUserDTO data);

    JsonVO<String> removeTenantUsers(RemoveTenantUserDTO data);
}
