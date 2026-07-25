package com.zeroone.star.sysmgr.service.login;

import com.zeroone.star.project.dto.login.LoginDTO;
import com.zeroone.star.project.dto.login.Oauth2TokenDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.LoginVO;

/**
 * 登录认证业务服务。
 *
 * <p>认证凭证的校验和令牌签发由 OAuth2 授权服务负责，本服务仅负责
 * 登录业务编排、响应转换及当前请求登录态读取。</p>
 */
public interface ILoginService {

    JsonVO<Oauth2TokenDTO> login(LoginDTO loginDTO);

    JsonVO<String> logout();

    JsonVO<LoginVO> getCurrentUser();
}
