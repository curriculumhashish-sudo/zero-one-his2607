package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDate;

/**
 * 药房管理-药房入库单-入库单明细响应对象
 */
@Data
@Schema(description = "入库单明细项")
public class PurchaseInboundItemVO {

    @Schema(description = "明细ID (对应 wkf_supply_request.id，如果是主子表结构)", example = "101")
    private String id;

    @Schema(description = "物资/药品ID (对应 wkf_supply_request.item_id)", example = "MED_001")
    private String itemId;

    @Schema(description = "物资名称", example = "阿莫西林胶囊")
    private String itemName;

    @Schema(description = "规格 (需关联 med_medication_definition.total_volume)", example = "0.25g*24粒")
    private String spec;

    @Schema(description = "批号 (对应 wkf_supply_request.lot_number)", example = "20260701")
    private String lotNumber;

    @Schema(description = "生产日期 (对应 wkf_supply_request.start_time)", example = "2026-01-01")
    private LocalDate productionDate;

    @Schema(description = "有效期至 (对应 wkf_supply_request.end_time)", example = "2028-01-01")
    private LocalDate expirationDate;

    @Schema(description = "数量 (对应 wkf_supply_request.item_quantity)", example = "100")
    private BigDecimal quantity;

    @Schema(description = "单位 (对应 wkf_supply_request.unit_code)", example = "盒")
    private String unit;

    @Schema(description = "单价 (对应 wkf_supply_request.price)", example = "15.00")
    private BigDecimal price;

    @Schema(description = "小计金额 (对应 wkf_supply_request.total_price)", example = "1500.00")
    private BigDecimal subTotal;
}