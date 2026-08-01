package com.zeroone.star.personcenter.controller;


import com.zeroone.star.project.dto.j2.personcenter.userdto.ChangePasswordDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdto.UpdateUserDTO;

import com.zeroone.star.project.dto.j2.personcenter.userdto.UserInfoDTO;
import com.zeroone.star.project.j2.personcenter.UserApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.personcenter.service.IWhaleUserService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;
/**
 * <p>
 * 描述：用户个人中心控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */

@RestController
@RequestMapping("/user")
@Tag(name = "用户个人中心")
public class UserController implements UserApis {

    @Resource
    private IWhaleUserService whaleUserService;

    @Operation(summary = "获取用户信息")
    @GetMapping("/get-info")
    @Override
    public JsonVO<UserInfoDTO> getUserInfo(
            @Parameter(description = "用户ID", example = "1234567890123456789")
            @RequestParam String userId) {
        UserInfoDTO userInfo = whaleUserService.getUserInfo(userId);
        if (userInfo == null) {
            return JsonVO.fail("用户不存在");
        }
        return JsonVO.success(userInfo);
    }

    @Operation(summary = "更新用户信息")
    @PostMapping("/update-user")
    @Override
    public JsonVO<UserInfoDTO> updateUser(@RequestBody UpdateUserDTO updateDTO) {
        UserInfoDTO result = whaleUserService.updateUser(updateDTO);
        return JsonVO.success(result);
    }

    @Operation(summary = "修改用户密码")
    @PostMapping("/modify-password")
    @Override
    public JsonVO<String> modifyPassword(@RequestBody ChangePasswordDTO changePasswordDTO) {
        try {
            whaleUserService.modifyPassword(changePasswordDTO);
            return JsonVO.success("密码修改成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
