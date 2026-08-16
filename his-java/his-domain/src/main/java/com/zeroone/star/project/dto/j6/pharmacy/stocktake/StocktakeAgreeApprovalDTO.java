package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 盘点单审批通过入参。
 */
@Data
@Schema(description = "药库盘点单审批通过入参")
public class StocktakeAgreeApprovalDTO {

    @Schema(description = "盘点单号", example = "CST202608070001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "业务单号不能为空")
    private String busNo;

    @Schema(description = "审批人ID（不传则服务端从当前登录用户取）", example = "308791351298756608")
    private String approverId;

    @Schema(description = "审批时间（不传则服务端写入当前时间）", example = "2026-08-07T15:00:00")
    private String approvalClock;

    @Schema(description = "关联合同单号（可选）", example = "CT202607180001")
    private String contractBusNo;
}
