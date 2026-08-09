package com.zeroone.star.project.dto.j2.personcenter.userdto;

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

    @Schema(description = "名", example = "三")
    private String name;

    @Schema(description = "姓", example = "张")
    private String surname;

    @Schema(description = "邮箱", example = "zhangsan@example.com")
    @Email(message = "邮箱格式不正确")
    private String email;

    @Schema(description = "手机号", example = "13800138000")
    private String phoneNumber;
}
