package com.zeroone.star.project.j1.login;


import com.zeroone.star.project.dto.j1.login.CaptchaVerifyDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.login.CaptchaVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;

@Tag(name = "j1-登录-验证码", description = "登录流程中的图形验证码生成与校验接口")
public interface ICaptchaApi {

    /**
     * 获取验证码
     */
    @Operation(summary = "生成验证码", description = "生成算术题图形验证码，返回 UUID 和 Base64 图片")
    JsonVO<CaptchaVO> generate();

    /**
     * 验证验证码
     */
    @Operation(summary = "校验验证码", description = "根据 UUID 校验用户输入的验证码是否正确")
    JsonVO<Boolean> verify(
            @Parameter(description = "验证码校验请求", required = true) CaptchaVerifyDTO dto
    );
}
