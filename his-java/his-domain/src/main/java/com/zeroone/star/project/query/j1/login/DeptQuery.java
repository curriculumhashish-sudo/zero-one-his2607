package com.zeroone.star.project.query.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "科室列表查询条件")
public class DeptQuery {

    @Schema(description = "组织单元名称(科室名称)模糊查询", example = "内科")
    private String displayName;

    @Schema(description = "科室编码", example = "NK001")
    private String code;

    @Schema(description = "租户ID")
    private String tenantId;

}
