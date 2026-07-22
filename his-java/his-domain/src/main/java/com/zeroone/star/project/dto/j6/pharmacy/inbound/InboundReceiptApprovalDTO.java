package com.zeroone.star.project.dto.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 进货单审批流转数据对象。
 */
@Data
@Schema(name = "InboundReceiptApprovalDTO", description = "药库进货单提交/撤回/同意/驳回请求")
public class InboundReceiptApprovalDTO {
    @NotBlank(message = "业务单号不能为空")
    @Schema(description = "业务单号")
    private String busNo;

    @NotBlank(message = "审批动作不能为空")
    @Schema(description = "审批动作：SUBMIT-提交，WITHDRAW-撤回，AGREE-同意，REJECT-驳回")
    private String action;

    @Schema(description = "审批人")
    private String approverId;

    @Schema(description = "审批时间")
    private LocalDateTime approvalClock;

    @Schema(description = "申请时间")
    private LocalDateTime applyClock;

    @Schema(description = "驳回原因")
    private String rejectReason;

}
