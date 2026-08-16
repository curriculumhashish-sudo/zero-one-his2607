package com.zeroone.star.project.dto.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 新增药库进货单 — 简洁请求。
 * 只传库房、供应商等基本上下文，服务端生成空白草稿并返回完整文档结构。
 */
@Data
@Schema(name = "InboundReceiptAddDTO", description = "新增单据的简洁请求数据传输对象")
public class InboundReceiptAddDTO {

    @NotBlank(message = "目的库房不能为空")
    @Schema(
            description = "入库目标库房ID（西药库/中药库/耗材库等）",
            example = "322954885380706304",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String purposeLocationId;

    @NotBlank(message = "供应商不能为空")
    @Schema(
            description = "供应商ID",
            example = "4100000000000000004",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String supplierId;

    @Schema(
            description = "制单人ID（不传则服务端从当前登录用户取）",
            example = "322932096108597248"
    )
    private String practitionerId;

    @Schema(
            description = "关联来源单号（如药库订货单号），可选",
            example = "DHD202607180001"
    )
    private String originalBusNo;

    @Schema(
            description = "备注原因",
            example = "按药库订货单到货入库"
    )
    private String reason;
}
