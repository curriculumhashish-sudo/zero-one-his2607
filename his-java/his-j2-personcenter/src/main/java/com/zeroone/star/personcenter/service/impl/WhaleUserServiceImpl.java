package com.zeroone.star.personcenter.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;

import com.zeroone.star.personcenter.entity.WhaleUser;
import com.zeroone.star.personcenter.mapper.WhaleUserMapper;
import com.zeroone.star.personcenter.service.IWhaleUserService;
import com.zeroone.star.project.dto.j2.personcenter.userdto.ChangePasswordDTO;

import com.zeroone.star.project.dto.j2.personcenter.userdto.UpdateUserDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdto.UserInfoDTO;
import org.springframework.stereotype.Service;
import org.springframework.util.DigestUtils;

import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：系统用户服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Service
public class WhaleUserServiceImpl extends ServiceImpl<WhaleUserMapper, WhaleUser> implements IWhaleUserService {

    @Override
    public UserInfoDTO getUserInfo(String userId) {
        WhaleUser user = baseMapper.selectById(userId);
        if (user == null) {
            return null;
        }
        UserInfoDTO dto = new UserInfoDTO();
        BeanUtil.copyProperties(user, dto);
        // LocalDateTime 转 String，格式化
        if (user.getCreatedAt() != null) {
            dto.setCreatedAt(user.getCreatedAt().toString().replace("T", " "));
        }
        if (user.getUpdatedAt() != null) {
            dto.setUpdatedAt(user.getUpdatedAt().toString().replace("T", " "));
        }
        return dto;
    }

    @Override
    public UserInfoDTO updateUser(UpdateUserDTO updateDTO) {
        if (updateDTO.getId() == null || updateDTO.getId().isBlank()) {
            throw new IllegalArgumentException("用户ID不能为空");
        }
        WhaleUser user = new WhaleUser();
        BeanUtil.copyProperties(updateDTO, user);
        user.setUserId(updateDTO.getId());
        user.setUpdatedAt(LocalDateTime.now());
        baseMapper.updateById(user);
        return getUserInfo(user.getUserId());
    }

    @Override
    public void modifyPassword(ChangePasswordDTO dto) {
        // 校验两次密码一致
        if (!dto.getNewPassword().equals(dto.getConfirmPassword())) {
            throw new IllegalArgumentException("两次输入的密码不一致");
        }

        if (dto.getId() == null || dto.getId().isBlank()) {
            throw new IllegalArgumentException("用户ID不能为空");
        }
        String userId = dto.getId();
        WhaleUser user = baseMapper.selectById(userId);
        if (user == null) {
            throw new RuntimeException("用户不存在");
        }

        // 校验旧密码
        String oldPwdHash = DigestUtils.md5DigestAsHex(
                dto.getOldPassword().getBytes(StandardCharsets.UTF_8));
        if (!oldPwdHash.equals(user.getPasswordHash())) {
            throw new IllegalArgumentException("旧密码不正确");
        }

        // 更新新密码
        WhaleUser updateUser = new WhaleUser();
        updateUser.setUserId(userId);
        updateUser.setPasswordHash(DigestUtils.md5DigestAsHex(
                dto.getNewPassword().getBytes(StandardCharsets.UTF_8)));
        updateUser.setUpdatedAt(LocalDateTime.now());
        baseMapper.updateById(updateUser);
    }
}
