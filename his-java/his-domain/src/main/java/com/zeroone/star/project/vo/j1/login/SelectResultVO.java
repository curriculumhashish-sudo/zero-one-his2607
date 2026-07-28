package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "选择结果")
public class SelectResultVO {

    @Schema(description = "是否成功", example = "true")
    private Boolean success;

    @Schema(description = "当前选择的租户ID", example = "1")
    private Long currentTenantId;

    @Schema(description = "当前选择的医院ID", example = "1")
    private Long currentHospitalId;
}