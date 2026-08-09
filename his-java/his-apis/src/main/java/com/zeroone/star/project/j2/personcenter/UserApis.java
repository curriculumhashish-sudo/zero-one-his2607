package com.zeroone.star.project.j2.personcenter;


import com.zeroone.star.project.dto.j2.personcenter.userdto.ChangePasswordDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdto.UpdateUserDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdto.UserInfoDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface UserApis {
    JsonVO<UserInfoDTO> getUserInfo(String userId);

    JsonVO<UserInfoDTO> updateUser(String id, UpdateUserDTO updateDTO);

    JsonVO<String> modifyPassword(String id, ChangePasswordDTO changePasswordDTO);
}
