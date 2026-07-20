package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：进货单 — 撤回审批入参
 * </p>
 * @author AB
 * @version 1.0.0
 */
@Schema(description = "进货单撤回审批入参")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class WithdrawInboundReceiptApprovalDTO {

    @Schema(description = "单据号", example = "IR202607180001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "单据号不能为空")
    private String busNo;
}
