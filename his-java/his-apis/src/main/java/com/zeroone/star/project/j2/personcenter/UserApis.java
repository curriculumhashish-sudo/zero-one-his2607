package com.zeroone.star.project.j2.personcenter;

import com.zeroone.star.project.dto.j2.personcenter.userdtovary.ChangePasswordDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdtovary.UpdateUserDTO;
import com.zeroone.star.project.dto.j2.personcenter.userdtovary.UserInfoDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface UserApis {
    JsonVO<UserInfoDTO> getUserInfo(String userId);

    JsonVO<UserInfoDTO> updateUser(UpdateUserDTO updateDTO);

    JsonVO<String> modifyPassword(ChangePasswordDTO changePasswordDTO);
}
