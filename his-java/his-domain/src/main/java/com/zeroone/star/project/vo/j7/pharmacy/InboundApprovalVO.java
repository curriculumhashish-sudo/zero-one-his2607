package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

/**
 * 入库单审批流转响应数据。
 *
 * @author lvv
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "入库单审批流转响应对象")
public class InboundApprovalVO {
    @Schema(description = "入库单业务单号", example = "CIR2026-06-300001")
    private String busNo;
    @Schema(
            description = "流转后的审批状态：WITHDRAW-已撤回，AGREE-已通过，REJECT-已驳回",
            example = "AGREE",
            allowableValues = {"WITHDRAW", "AGREE", "REJECT"}
    )
    private String status;
    @Schema(description = "操作结果说明", example = "审批通过")
    private String message;
    @Schema(description = "审批人姓名", example = "李四")
    private String approverName;
    @Schema(description = "审批时间", example = "2026-08-09 09:15")
    private LocalDateTime approveTime;

    public InboundApprovalVO(String busNo, String status, String message) {
        this.busNo = busNo;
        this.status = status;
        this.message = message;
    }
}
