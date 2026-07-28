package com.zeroone.star.sysmgr.controller.user;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.user.ChangePasswordDTO;
import com.zeroone.star.project.dto.j1.user.UserUpdateDTO;
import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.project.j1.user.UserApis;
import com.zeroone.star.project.query.j1.user.UserListQuery;
import com.zeroone.star.project.query.j1.user.UserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.user.UserDetailVO;
import com.zeroone.star.project.vo.j1.user.UserListVO;
import com.zeroone.star.project.vo.j1.user.UserPageVO;
import com.zeroone.star.sysmgr.service.user.IUserService;
import io.swagger.v3.oas.annotations.Operation;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
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
    public JsonVO<PageDTO<UserPageVO>> getPage(@RequestBody @Valid UserQuery query) {
        return JsonVO.success(userService.getPage(query));
    }


    @Operation(summary   = "查询选择(租户)用户列表")
    @GetMapping("/list")
    @Override
    public JsonVO<PageDTO<UserListVO>> getList(@RequestBody @Valid UserListQuery query) {
        return JsonVO.success(userService.getList(query));
    }

    @Operation(summary   = "根据用户ID查询用户详情")
    @GetMapping("/{id}")
    @Override
    public JsonVO<UserDetailVO> getById(@PathVariable String id) {
        return JsonVO.success(userService.getById(id));
    }


    @Operation(summary   = "新增用户")
    @PostMapping
    @Override
    public JsonVO<Boolean> saveUser(@Valid @RequestBody UserAddDTO userAddDTO) {
        return JsonVO.success(userService.saveUser(userAddDTO));
    }

    @Operation(summary   = "修改用户")
    @PutMapping("/update")
    @Override
    public JsonVO<Boolean> updateUser(@Valid @RequestBody UserUpdateDTO userUpdateDTO) {
        return JsonVO.success(userService.updateUser(userUpdateDTO));
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
    public JsonVO<Boolean> changePassword(@Valid @RequestBody ChangePasswordDTO changePasswordDTO) {
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
