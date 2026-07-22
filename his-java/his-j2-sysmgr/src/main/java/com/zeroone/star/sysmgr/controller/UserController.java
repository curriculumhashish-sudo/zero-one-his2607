package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.j2.sysmgr.UserDTO.ChangePasswordDTO;
import com.zeroone.star.project.dto.j2.sysmgr.UserDTO.UpdateUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.UserDTO.UserInfoDTO;
import com.zeroone.star.project.j2.sysmgr.UserApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/user")
@Tag(name = "用户个人中心")
public class UserController implements UserApis {
    @Operation(summary = "获取用户信息")
    @GetMapping("/get-info")
    @Override
    public JsonVO<UserInfoDTO> getUserInfo(
            @Parameter(description = "用户ID", example = "1234567890123456789")
            @RequestParam String userId) {
        return null;
    }

    @Operation(summary = "更新用户信息")
    @PostMapping("/update-user")
    @Override
    public JsonVO<UserInfoDTO> updateUser(@RequestBody UpdateUserDTO updateDTO) {
        return null;
    }

    @Operation(summary = "修改用户密码")
    @PostMapping("/modify-password")
    @Override
    public JsonVO<String> modifyPassword(@RequestBody ChangePasswordDTO changePasswordDTO) {
        return null;
    }
}
