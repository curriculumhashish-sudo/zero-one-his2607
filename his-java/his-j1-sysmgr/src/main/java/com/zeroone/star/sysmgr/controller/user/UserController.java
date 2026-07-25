package com.zeroone.star.sysmgr.controller.user;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.user.ChangePasswordDTO;
import com.zeroone.star.project.dto.j1.user.UserUpdateDTO;
import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.project.j1.user.UserApis;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.query.j1.user.UserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.user.UserDetailVO;
import com.zeroone.star.project.vo.j1.user.UserListVO;
import com.zeroone.star.project.vo.j1.user.UserPageVO;
import com.zeroone.star.sysmgr.service.user.IUserService;
import io.swagger.v3.oas.annotations.Operation;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;
import io.swagger.v3.oas.annotations.tags.Tag;

@RestController
@RequestMapping("/user")
@Tag(name = "用户管理")
public class UserController implements UserApis {

    @Resource
    private IUserService userService;

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
    public JsonVO<Boolean> saveUser(@RequestBody UserAddDTO userAddDTO) {
        return null;
    }

    @Operation(summary   = "修改用户")
    @PutMapping("/update")
    @Override
    public JsonVO<Boolean> updateUser(@RequestBody UserUpdateDTO userUpdateDTO) {
        return null;
    }


    @Operation(summary   = "删除用户")
    @DeleteMapping("/{id}")
    @Override
    public JsonVO<Boolean> deleteById(@PathVariable String id){
        return JsonVO.success(userService.softDeleteById(id));
    }

    @Operation(summary   = "修改密码")
    @PutMapping("/changePassword")
    @Override
    public JsonVO<Boolean> changePassword(@RequestBody ChangePasswordDTO changePasswordDTO) {
        return JsonVO.success(userService.updatePassword(changePasswordDTO.getId(), changePasswordDTO.getPasswordHash()));
    }

    @Operation(summary   = "修改用户状态")
    @PutMapping("/updateStatus/{id}/status/{status}")
    @Override
    public JsonVO<Boolean> updateStatus(@PathVariable String id, @PathVariable Integer status) {
        return JsonVO.success(userService.updateStatus(id, status));
    }

    @Operation(summary   = "锁定用户")
    @PutMapping("/lockUser/{id}/lockStatus/{lockStatus}")
    @Override
    public JsonVO<Boolean> lockUser(@PathVariable String id, @PathVariable Integer lockStatus) {
        return JsonVO.success(userService.lockUser(id,lockStatus));
    }

}
