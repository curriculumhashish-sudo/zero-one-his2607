package com.zeroone.star.project.query.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

// 进货单 详情查询（对应前端 GetInboundReceiptDetailRequest）
// 字段：busNo
@Getter
@Setter
@Schema(description = "进货单详情查询参数")
public class InboundReceiptDetailQuery {

    @Schema(description = "业务单号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;
}
