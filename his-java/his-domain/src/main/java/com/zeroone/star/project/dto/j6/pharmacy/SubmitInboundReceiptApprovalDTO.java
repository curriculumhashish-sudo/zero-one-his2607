package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：进货单 — 提交审批入参
 * </p>
 * @author AB
 * @version 1.0.0
 */
@Schema(description = "进货单提交审批入参")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class SubmitInboundReceiptApprovalDTO {

    @Schema(description = "单据号", example = "IR202607180001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "单据号不能为空")
    private String busNo;

    @Schema(description = "指定的审批人ID",requiredMode = Schema.RequiredMode.REQUIRED, example = "1001")
    private String approverId;

    @Schema(description = "申请时间（不传则由服务端写入当前时间）", example = "2026-07-18T10:30:00")
    private String applyClock;
}
