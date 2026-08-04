package com.zeroone.star.sysmgr.service.captcha;

import com.zeroone.star.sysmgr.entity.captcha.CaptchaDo;

public interface ICaptchaService {

    /**
     * 生成验证码
     */
    CaptchaDo generate();

    /**
     * 校验验证码
     */
    boolean verify(String uuid, String code);
}
