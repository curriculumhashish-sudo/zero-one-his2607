package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 盘点单提交审批入参。
 */
@Data
@Schema(description = "药库盘点单提交审批入参")
public class StocktakeSubmitApprovalDTO {

    @Schema(description = "盘点单号", example = "CST202608070001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "业务单号不能为空")
    private String busNo;

    @Schema(description = "审批人ID", example = "308791351298756608", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "审批人不能为空")
    private String approverId;
}
