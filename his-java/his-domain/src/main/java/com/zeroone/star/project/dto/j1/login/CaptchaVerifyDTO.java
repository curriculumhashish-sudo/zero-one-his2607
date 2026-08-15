package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "验证码校验入参")
public class CaptchaVerifyDTO {

    @Schema(description = "验证码UUID", requiredMode = Schema.RequiredMode.REQUIRED, example = "a1b2c3d4e5f6")
    @NotBlank(message = "验证码UUID不能为空")
    private String uuid;

    @Schema(description = "用户输入的验证码", requiredMode = Schema.RequiredMode.REQUIRED, example = "8")
    @NotBlank(message = "验证码不能为空")
    private String code;
}