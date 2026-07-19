package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 选择租户请求DTO
 */
@Data
@Schema(description = "选择租户请求")
public class SelectTenantDTO {

    @Schema(description = "租户ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private Long tenantId;

    @Schema(description = "租户编码", requiredMode = Schema.RequiredMode.REQUIRED, example = "DEFAULT")
    private String tenantCode;
}
