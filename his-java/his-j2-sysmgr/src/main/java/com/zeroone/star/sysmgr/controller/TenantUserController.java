package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.RemoveTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.TenantUserDTO;
import com.zeroone.star.project.j2.sysmgr.tenant_user.TenantUserApis;
import com.zeroone.star.project.query.j2.sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sysmgr.service.tenant_user.ITenantUserService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：租户用户管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
@RestController
@Tag(name = "租户用户管理")
@RequestMapping("/sysmgr/tenant-user")
public class TenantUserController implements TenantUserApis {

    @Resource
    private ITenantUserService tenantUserService;

    @Operation(summary = "获取租户用户列表")
    @GetMapping("/list-tenant-users")
    @Override
    public JsonVO<PageDTO<TenantUserDTO>> listTenantUsers(TenantUserQuery query) {
        return tenantUserService.listTenantUsers(query);
    }

    @Operation(summary = "添加租户用户")
    @PostMapping("/add-tenant-users")
    @Override
    public JsonVO<String> addTenantUsers(@RequestBody AddTenantUserDTO data) {
        return tenantUserService.addTenantUsers(data);
    }

    @Operation(summary = "移除租户用户")
    @PostMapping("/remove-tenant-users")
    @Override
    public JsonVO<String> removeTenantUsers(@RequestBody RemoveTenantUserDTO data) {
        return tenantUserService.removeTenantUsers(data);
    }
}
