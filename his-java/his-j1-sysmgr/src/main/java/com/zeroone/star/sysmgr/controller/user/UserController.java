package com.zeroone.star.sysmgr.controller.user;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.sysmgr.ChangePasswordDTO;
import com.zeroone.star.project.dto.j1.sysmgr.UserDTO;
import com.zeroone.star.project.j1.user.UserApis;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.query.j1.sysmgr.UserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserDetailVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserListVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserPageVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.web.bind.annotation.*;
import io.swagger.v3.oas.annotations.tags.Tag;

@RestController
@RequestMapping("/user")
@Tag(name = "用户管理")
public class UserController implements UserApis {

    @Operation(summary   = "分页查询用户列表")
    @GetMapping("/page")
    @Override
    public JsonVO<PageDTO<UserPageVO>> getPage(UserQuery userQuery) {
        return null;
    }


    @Operation(summary   = "查询选择(租户)用户列表")
    @GetMapping("/list")
    @Override
    public JsonVO<PageDTO<UserListVO>> getList(TenantQuery tenantQuery) {
        return null;
    }

    @Operation(summary   = "根据用户ID查询用户详情")
    @GetMapping("/{id}")
    @Override
    public JsonVO<UserDetailVO> getById(@PathVariable String id) {
        return null;
    }


    @Operation(summary   = "新增用户")
    @PostMapping
    @Override
    public JsonVO<String> saveUser(@RequestBody UserDTO userDTO) {
        return null;
    }

    @Operation(summary   = "修改用户")
    @PutMapping("/update")
    @Override
    public JsonVO<String> updateUser(@RequestBody UserDTO userDTO) {
        return null;
    }


    @Operation(summary   = "删除用户")
    @DeleteMapping("/{id}")
    @Override
    public JsonVO<String> deleteById(@PathVariable String id) {
        return null;
    }

    @Operation(summary   = "修改密码")
    @PutMapping("/changePassword")
    @Override
    public JsonVO<String> changePassword(@RequestBody ChangePasswordDTO changePasswordDTO) {
        return null;
    }

    @Operation(summary   = "修改用户状态")
    @PutMapping("/updateStatus/{id}/status/{status}")
    @Override
    public JsonVO<String> updateStatus(@PathVariable String id, @PathVariable Integer status) {
        return null;
    }

    @Operation(summary   = "锁定用户")
    @PutMapping("/lockUser/{id}/lockStatus/{lockStatus}")
    @Override
    public JsonVO<String> lockUser(@PathVariable String id,@PathVariable Integer lockStatus) {
        return null;

    }
}
