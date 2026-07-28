package com.zeroone.star.configuration.query;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.AllArgsConstructor;

/**
 * <p>
 * 组套名称下拉查询条件（接口 1）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class OrderTemplateNameQuery {

    @NotBlank(message = "适用场景不能为空")
    @Schema(description = "适用场景（OUTPATIENT/INPATIENT/GENERAL）", example = "OUTPATIENT")
    private String sceneEnum = "OUTPATIENT";

    @NotBlank(message = "组套种类不能为空")
    @Schema(description = "组套种类（GENERAL/TCM/PRICING）", example = "GENERAL")
    private String templateKindEnum = "GENERAL";

    @Schema(description = "当前医生ID（传则按绑定可见性过滤）")
    private String practitionerId;

    @Schema(description = "当前科室 ID（传则按绑定可见性过滤）")
    private String deptId;
}