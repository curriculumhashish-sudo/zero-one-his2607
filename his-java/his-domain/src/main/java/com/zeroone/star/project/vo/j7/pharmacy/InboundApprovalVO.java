package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
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
    @Schema(description = "流转后的审批状态", example = "AGREE")
    private String status;
    @Schema(description = "操作结果说明", example = "审批通过")
    private String message;
}
