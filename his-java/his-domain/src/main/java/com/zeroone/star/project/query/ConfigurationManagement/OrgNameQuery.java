package com.zeroone.star.project.query.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "执行科室查询字段")
public class OrgNameQuery {

    @Schema(description = "执行科室id",example = "4562136665111111", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
