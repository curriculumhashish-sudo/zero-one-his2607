package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 盘点单驳回审批入参。
 */
@Data
@Schema(description = "药库盘点单驳回审批入参")
public class StocktakeRejectApprovalDTO {

    @Schema(description = "盘点单号", example = "CST202608070001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "业务单号不能为空")
    private String busNo;

    @Schema(description = "审批人ID（不传则服务端从当前登录用户取）", example = "308791351298756608")
    private String approverId;

    @Schema(description = "审批时间（不传则服务端写入当前时间）", example = "2026-08-07T15:00:00")
    private String approvalClock;

    @Schema(description = "驳回原因", example = "实盘数据与账面不符，请重新盘点", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "驳回原因不能为空")
    private String changeReason;

    @Schema(description = "驳回原因编码", example = "DATA_MISMATCH")
    private String changeReasonCode;
}
