package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
/**
 * 入库单审批流转请求参数。
 *
 * @author lvv
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "入库单审批流转请求对象")
public class InboundApprovalDTO {
    @NotBlank(message = "入库单号不能为空")
    @Size(max = 64, message = "入库单号长度不能超过64个字符")
    @Schema(description = "入库单业务单号", example = "CIR2026-06-300001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;
    @Size(max = 500, message = "审批意见长度不能超过500个字符")
    @Schema(description = "审批意见；驳回审批、撤回审批时填写原因", example = "库存数量有误，驳回后请重新提交")
    private String remark;
}
