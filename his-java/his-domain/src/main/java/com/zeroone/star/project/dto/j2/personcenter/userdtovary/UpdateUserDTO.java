package com.zeroone.star.project.dto.j2.personcenter.userdtovary;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Email;
import lombok.Data;
import lombok.experimental.Accessors;

@Data
@Accessors(chain = true)
@Schema(description = "更新用户信息请求")
public class UpdateUserDTO {

    @Schema(description = "用户ID", example = "1234567890123456789")
    private String id;

    @Schema(description = "真实姓名", example = "张三")
    private String realName;

    @Schema(description = "邮箱", example = "zhangsan@example.com")
    @Email(message = "邮箱格式不正确")
    private String email;

    @Schema(description = "手机号", example = "13800138000")
    private String phoneNumber;

    @Schema(description = "头像URL", example = "https://example.com/avatar.jpg")
    private String avatar;

    @Schema(description = "个人简介", example = "这是我的个人简介")
    private String introduction;
}
