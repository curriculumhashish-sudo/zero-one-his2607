package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Getter
@Setter
@Schema(description = "进货单明细行")
public class InboundReceiptDetailLineVO {

    @Schema(description = "行ID")
    private String id;

    @Schema(description = "项目表名")
    private String itemTable;

    @Schema(description = "项目ID")
    private String itemId;

    @Schema(description = "数量")
    private BigDecimal itemQuantity;

    @Schema(description = "单位编码")
    private String unitCode;

    @Schema(description = "单位名称")
    private String unitCodeDictText;

    @Schema(description = "批号")
    private String lotNumber;

    @Schema(description = "追踪号")
    private String traceNo;

    @Schema(description = "单价")
    private BigDecimal price;

    @Schema(description = "总价")
    private BigDecimal totalPrice;

    @Schema(description = "发生时间")
    private LocalDateTime occurrenceTime;

    @Schema(description = "开始时间")
    private LocalDateTime startTime;

    @Schema(description = "结束时间")
    private LocalDateTime endTime;

    @Schema(description = "备注")
    private String remake;

    @Schema(description = "商品名")
    private String productName;

    @Schema(description = "规格")
    private String specification;

    @Schema(description = "生产厂家")
    private String manufacturer;
}