package com.zeroone.star.project.dto.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 进货单删除数据对象。
 */
@Data
@Schema(name = "InboundReceiptDeleteDTO", description = "药库进货单删除请求")
public class InboundReceiptDeleteDTO {
    @NotBlank(message = "业务单号不能为空")
    @Schema(description = "业务单号", example = "CIR202608070001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;
}
