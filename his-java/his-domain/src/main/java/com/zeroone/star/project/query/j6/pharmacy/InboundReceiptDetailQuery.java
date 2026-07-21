package com.zeroone.star.project.query.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(name = "InboundReceiptDetailQuery", description = "药库进货单详情查询条件")
public class InboundReceiptDetailQuery {
    @NotBlank(message = "业务单号不能为空")
    @Schema(description = "业务单号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;
}
