package com.zeroone.star.project.login;

import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.LoginVO;

public interface LoginApis {
    JsonVO<LoginVO> getCurrUser();
    JsonVO<?> getMenus();
}
