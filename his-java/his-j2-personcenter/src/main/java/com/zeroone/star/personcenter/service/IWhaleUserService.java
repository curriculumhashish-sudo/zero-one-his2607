package com.zeroone.star.personcenter.service;

import com.baomidou.mybatisplus.extension.service.IService;

import com.zeroone.star.personcenter.entity.WhaleUser;
import com.zeroone.star.project.dto.j2.personcenter.userdtovary.ChangePasswordDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdtovary.UpdateUserDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdtovary.UserInfoDTO;

/**
 * <p>
 * 描述：系统用户服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface IWhaleUserService extends IService<WhaleUser> {

    /**
     * 根据用户ID获取用户信息
     * @param userId 用户ID
     * @return 用户信息 DTO
     */
    UserInfoDTO getUserInfo(Long userId);

    /**
     * 更新用户信息
     * @param updateDTO 更新数据
     * @return 更新后的用户信息
     */
    UserInfoDTO updateUser(UpdateUserDTO updateDTO);

    /**
     * 修改密码
     * @param changePasswordDTO 密码修改请求
     */
    void modifyPassword(ChangePasswordDTO changePasswordDTO);
}
