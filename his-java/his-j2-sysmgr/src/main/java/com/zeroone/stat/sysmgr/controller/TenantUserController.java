package com.zeroone.stat.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.RemoveTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.TenantUserDTO;
import com.zeroone.star.project.j2.sysmgr.TenantUserApis;
import com.zeroone.star.project.query.j2_sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
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

    @Operation(summary = "获取租户用户列表")
    @GetMapping("/list-tenant-users")
    @Override
    public JsonVO<PageDTO<TenantUserDTO>> listTenantUsers(TenantUserQuery query) {
        return null;
    }

    @Operation(summary = "添加租户用户")
    @PostMapping("/add-tenant-users")
    @Override
    public JsonVO<String> addTenantUsers(@RequestBody AddTenantUserDTO data) {
        return null;
    }

    @Operation(summary = "移除租户用户")
    @PostMapping("/remove-tenant-users")
    @Override
    public JsonVO<String> removeTenantUsers(RemoveTenantUserDTO data) {
        return null;
    }
}
