package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：出库单审批流转DTO（提交/撤回/同意/驳回四合一）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-告别
 * @version 1.0.0
 */
@Data
@Schema(description = "出库单审批流转对象")
public class StockOutApprovalDTO {
    /**
     * 业务单号
     */
    @Schema(description = "业务单号", example = "SO202607180001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "业务单号不能为空")
    private String busNo;

    /**
     * 审批动作：SUBMIT-提交审批, WITHDRAW-撤回审批, AGREE-审批通过, REJECT-驳回审批
     */
    @Schema(description = "审批动作", example = "AGREE", allowableValues = {"SUBMIT", "WITHDRAW", "AGREE", "REJECT"},
            requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "审批动作不能为空")
    private String action;

    /**
     * 审批人ID（AGREE/REJECT时由服务端从会话解析）
     */
    @Schema(description = "审批人ID", example = "310000000000000101")
    private String approverId;

    /**
     * 审批时间（由服务端写入）
     */
    @Schema(description = "审批时间", example = "2026-07-18 10:30:00")
    private LocalDateTime approvalClock;

    /**
     * 申请时间（SUBMIT时由服务端写入）
     */
    @Schema(description = "申请时间", example = "2026-07-18 09:00:00")
    private LocalDateTime applyClock;

    /**
     * 驳回原因（REJECT时必填）
     */
    @Schema(description = "驳回原因", example = "药品库存不足")
    private String rejectReason;

    /**
     * 关联合同单号（AGREE时可选）
     */
    @Schema(description = "关联合同单号", example = "CT202607180001")
    private String contractBusNo;

    /**
     * 价格（AGREE时可选，用于更新价格）
     */
    @Schema(description = "价格", example = "12.50")
    private BigDecimal price;

    /**
     * 合计价格（AGREE时可选，用于更新合计金额）
     */
    @Schema(description = "合计价格", example = "1250.00")
    private BigDecimal totalPrice;
}
