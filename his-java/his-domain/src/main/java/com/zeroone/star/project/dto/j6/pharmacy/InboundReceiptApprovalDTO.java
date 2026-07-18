package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：进货单审批操作入参（提交/撤回/通过/驳回）
 * </p>
 * @author AB
 * @version 1.0.0
 */
@Schema(description = "进货单审批操作入参（提交/撤回/通过/驳回共用）")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class InboundReceiptApprovalDTO {

    /**
     * 单据号 — 四个操作均必填
     */
    @Schema(description = "单据号", example = "IR202607180001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "单据号不能为空")
    private String busNo;

    /**
     * 申请时间 — 仅「提交审批」使用；前端不传时由服务端写入当前时间
     */
    @Schema(description = "申请时间（提交审批时填写，格式 yyyy-MM-ddTHH:mm:ss；不传则由服务端写入当前时间）", example = "2026-07-18T10:30:00")
    private String applyClock;

    /**
     * 审批人标识 — 「提交」「通过」「驳回」均使用：提交时指定审批人，通过/驳回时校验匹配
     */
    @Schema(description = "审批人ID（提交时指定审批人，通过/驳回时校验匹配）", example = "1001")
    private String approverId;

    /**
     * 审批时刻 — 「通过」和「驳回」使用；前端不传时由服务端写入当前时间
     */
    @Schema(description = "审批时间（通过/驳回时填写，格式 yyyy-MM-ddTHH:mm:ss；不传则由服务端写入当前时间）", example = "2026-07-18T14:00:00")
    private String approvalClock;

    /**
     * 医保契约单号 — 仅「审批通过且需上传医保」时传入
     */
    @Schema(description = "医保契约单号（审批通过且需上传医保时填写）", example = "YB202607180001")
    private String contractBusNo;

    /**
     * 驳回原因 — 仅「驳回审批」使用
     */
    @Schema(description = "驳回原因（驳回审批时必填）", example = "单据信息不完整，请补充供应商信息")
    private String rejectReason;
}
