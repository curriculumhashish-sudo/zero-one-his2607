package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;

/**
 * <p>
 * 描述：药房入库单-明细提交对象
 * </p>
 * <p>
 * 功能说明：用于接收前端提交的入库单明细行数据。
 * 对应数据库表 wkf_supply_request 中的明细字段（聚合根模式下，这些字段存在于主表中）。
 * </p>
 */
@Getter
@Setter
@Schema(description = "入库单明细提交对象")
public class PurchaseInboundItemDTO {

    @Schema(description = "药品/物资ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "MED_001")
    private String itemId;

    @Schema(description = "批号", example = "20260701")
    private String lotNumber;

    @Schema(description = "生产日期", example = "2026-01-01")
    private LocalDate productionDate;

    @Schema(description = "有效期至", example = "2028-01-01")
    private LocalDate expirationDate;

    @Schema(description = "入库数量", requiredMode = Schema.RequiredMode.REQUIRED, example = "100")
    private BigDecimal quantity;

    @Schema(description = "单位编码", example = "BOX")
    private String unitCode;

    @Schema(description = "单价", example = "15.00")
    private BigDecimal price;

    @Schema(description = "备注", example = "需冷藏")
    private String remark;
}