package com.zeroone.star.project.query.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "医院查询条件")
public class HospitalQuery {

    @Schema(description = "租户ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private Long tenantId;

    @Schema(description = "医院名称/编码模糊搜索关键字", example = "人民")
    private String keyword;
}