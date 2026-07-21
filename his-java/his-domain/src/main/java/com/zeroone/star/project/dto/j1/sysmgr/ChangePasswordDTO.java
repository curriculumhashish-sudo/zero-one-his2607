package com.zeroone.star.project.dto.j1.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "修改密码")
public class ChangePasswordDTO {
    @Schema(description = "用户ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "323022664197214208")
    private String id;

    @Schema(description = "新密码", requiredMode = Schema.RequiredMode.REQUIRED, example = "123456")
    private String newPassword;
}
