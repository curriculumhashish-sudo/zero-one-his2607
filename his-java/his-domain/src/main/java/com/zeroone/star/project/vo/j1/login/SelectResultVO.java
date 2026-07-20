package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 选择结果VO（通用返回）
 */
@Data
@Schema(description = "选择结果")
public class SelectResultVO {

    @Schema(description = "是否成功", example = "true")
    private Boolean success;

    @Schema(description = "当前选择的租户ID")
    private Long currentTenantId;

    @Schema(description = "当前选择的医院ID")
    private Long currentHospitalId;
}