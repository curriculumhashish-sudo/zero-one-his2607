package com.zeroone.star.project.dto.j1.user;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

@Data
@Schema(description = "修改密码")
public class ChangePasswordDTO {

    @Schema(description = "用户ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "323022664197214208")
    private String id;

    @Size(min = 8, max = 20, message = "密码长度至少为8位")
    @Schema(description = "新密码", requiredMode = Schema.RequiredMode.REQUIRED, example = "123456")
    private String passwordHash;
}
