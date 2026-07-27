package com.zeroone.star.project.dto.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：进货单 — 驳回审批入参
 * </p>
 * @author AB
 * @version 1.0.0
 */
@Schema(description = "进货单驳回审批入参")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class RejectInboundReceiptDTO {

    @Schema(description = "单据号", example = "IR202607180001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "单据号不能为空")
    private String busNo;

    @Schema(description = "驳回原因", example = "单据信息不完整，请补充供应商信息", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "驳回原因不能为空")
    private String rejectReason;

    @Schema(description = "审批人ID（不传则由服务端从当前登录用户获取）", example = "1001")
    private String approverId;

    @Schema(description = "审批时间（不传则由服务端写入当前时间）", example = "2026-07-18T14:00:00")
    private String approvalClock;
}
