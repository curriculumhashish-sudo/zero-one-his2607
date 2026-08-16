package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 出库单明细行 VO。
 */
@Data
@Schema(name = "StockOutLineVO", description = "药库出库单明细行")
public class StockOutLineVO {

    @Schema(description = "明细主键", example = "330263864072474624")
    private String id;

    @Schema(description = "项目物理表", example = "med_medication_definition")
    private String itemTable;

    @Schema(description = "项目定义主键", example = "322994478289784832")
    private String itemId;

    @Schema(description = "项目名称", example = "替米沙坦片")
    private String productName;

    @Schema(description = "规格", example = "40mg")
    private String specification;

    @Schema(description = "生产厂家", example = "上海信谊天平药业有限公司")
    private String manufacturer;

    @Schema(description = "行数量（拆零最小单位）", example = "100.00")
    private BigDecimal itemQuantity;

    @Schema(description = "单位编码", example = "1")
    private String unitCode;

    @Schema(description = "单位编码显示文本", example = "盒")
    private String unitCodeDictText;

    @Schema(description = "批号", example = "001-002")
    private String lotNumber;

    @Schema(description = "追溯码", example = "TR2026080100001")
    private String traceNo;

    @Schema(description = "单价", example = "13.36")
    private BigDecimal price;

    @Schema(description = "行合计金额", example = "1336.00")
    private BigDecimal totalPrice;

    @Schema(description = "发生时间", example = "2026-08-07T10:30:00")
    private LocalDateTime occurrenceTime;

    @Schema(description = "效期区间起点（生产日期）", example = "2026-06-01T00:00:00")
    private LocalDateTime startTime;

    @Schema(description = "效期区间止点（有效期至）", example = "2027-07-31T00:00:00")
    private LocalDateTime endTime;

    @Schema(description = "行备注", example = "首批调拨")
    private String remake;
}
