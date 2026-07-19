package com.zeroone.star.project.query.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 医院查询条件
 */
@Data
@Schema(description = "医院查询条件")
public class HospitalQuery {

    @Schema(description = "租户ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long tenantId;
}