package com.zeroone.star.project.dto.j1.user;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.Size;
import lombok.Data;

@Data
@Schema(description = "修改密码")
public class ChangePasswordDTO {
    @NotBlank(message = "用户ID不能为空")
    @Schema(description = "用户ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "323022664197214208")
    private String id;

    @NotEmpty(message = "新密码不能为空")
    @Size(min = 6, message = "密码至少为6位")
    @Schema(description = "新密码", requiredMode = Schema.RequiredMode.REQUIRED, example = "123456")
    private String passwordHash;
}
