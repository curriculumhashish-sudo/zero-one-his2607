package com.zeroone.star.project.dto.j2.personcenter.userdto;

import com.fasterxml.jackson.annotation.JsonProperty;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.experimental.Accessors;

@Data
@Accessors(chain = true)
@Schema(description = "修改密码请求")
public class ChangePasswordDTO {

    @Schema(description = "用户ID", example = "1234567890123456789")
    @NotBlank(message = "用户ID不能为空")
    @JsonProperty("id")
    private String userId;

    @Schema(description = "旧密码", example = "oldPassword123")
    @NotBlank(message = "旧密码不能为空")
    private String oldPassword;

    @Schema(description = "新密码", example = "newPassword123")
    @NotBlank(message = "新密码不能为空")
    @Size(min = 6, message = "新密码长度不能少于6位")
    private String newPassword;

    @Schema(description = "确认新密码", example = "newPassword123")
    @NotBlank(message = "确认密码不能为空")
    private String confirmPassword;
}
