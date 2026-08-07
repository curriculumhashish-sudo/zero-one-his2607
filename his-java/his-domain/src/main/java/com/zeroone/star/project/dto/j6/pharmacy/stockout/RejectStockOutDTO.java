package com.zeroone.star.project.dto.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 驳回出库审批 DTO。
 */
@Data
@Schema(description = "驳回出库审批对象")
public class RejectStockOutDTO {

    @Schema(description = "业务单号", example = "CSO202608070001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "业务单号不能为空")
    private String busNo;

    @Schema(description = "审批人ID", example = "308791351298756608")
    private String approverId;

    @Schema(description = "审批时间（不传则服务端写入当前时间）", example = "2026-08-07T15:00:00")
    private String approvalClock;

    @Schema(description = "驳回原因", example = "药品库存不足", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "驳回原因不能为空")
    private String rejectReason;
}
