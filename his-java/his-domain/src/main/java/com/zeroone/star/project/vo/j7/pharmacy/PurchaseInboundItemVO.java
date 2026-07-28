package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;

/**
 * <p>
 * 描述：药房入库单-明细响应对象
 * </p>
 * <p>
 * 功能说明：用于在【获取单据详情】接口中，展示入库单包含的具体药品/物资行信息。
 * </p>
 */
@Getter
@Setter
@Schema(description = "入库单明细项")
public class PurchaseInboundItemVO {

    @Schema(description = "明细主键ID", example = "101")
    private String id;

    @Schema(description = "药品/物资ID", example = "MED_001")
    private String itemId;

    @Schema(description = "药品/物资名称", example = "阿莫西林胶囊")
    private String itemName;

    @Schema(description = "规格", example = "0.25g*24粒")
    private String spec;

    @Schema(description = "批号", example = "20260701")
    private String lotNumber;

    @Schema(description = "生产日期", example = "2026-01-01")
    private LocalDate productionDate;

    @Schema(description = "有效期至", example = "2028-01-01")
    private LocalDate expirationDate;

    @Schema(description = "入库数量", example = "100")
    private BigDecimal quantity;

    @Schema(description = "单位", example = "盒")
    private String unit;

    @Schema(description = "单价", example = "15.00")
    private BigDecimal price;

    @Schema(description = "小计金额", example = "1500.00")
    private BigDecimal subTotal;
}