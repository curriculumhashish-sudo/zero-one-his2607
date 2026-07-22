package com.zeroone.star.project.j2.sysmgr;

import com.zeroone.star.project.dto.j2.sysmgr.UserDTO.ChangePasswordDTO;
import com.zeroone.star.project.dto.j2.sysmgr.UserDTO.UpdateUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.UserDTO.UserInfoDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface UserApis {
    JsonVO<UserInfoDTO> getUserInfo(String userId);

    JsonVO<UserInfoDTO> updateUser(UpdateUserDTO updateDTO);

    JsonVO<String> modifyPassword(ChangePasswordDTO changePasswordDTO);
}
