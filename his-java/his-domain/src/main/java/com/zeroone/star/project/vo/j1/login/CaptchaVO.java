package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "验证码生成结果")
public class CaptchaVO {

    @Schema(description = "验证码唯一标识，校验时需回传", example = "a1b2c3d4e5f6")
    private String uuid;

    @Schema(description = "验证码图片 Base64 编码", example = "data:image/png;base64,iVBOR...")
    private String imageBase64;
}