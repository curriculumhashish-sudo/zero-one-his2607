package com.zeroone.star.project.vo.j6.pharmacy;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.math.BigDecimal;

/**
 * <p>
 * 描述：药库盘点单明细行VO
 * </p>
 * @author 01星球
 */
@Data
@Schema(description = "药库盘点单明细行")
public class PharmacyInventoryItemVO {
    @Schema(description = "明细ID")
    private String lineId;

    @Schema(description = "药品主表名称")
    private String itemTable;

    @Schema(description = "药品ID")
    private String itemId;

    @Schema(description = "药品名称", example = "黄芪")
    private String productName;

    @Schema(description = "规格", example = "饮片")
    private String specification;

    @Schema(description = "生产厂家", example = "安国市中药材市场")
    private String manufacturer;

    @Schema(description = "批号", example = "999-010")
    private String lotNumber;

    @Schema(description = "有效期")
    private String expirationDate;

    @Schema(description = "单位编码")
    private String unitCode;

    @Schema(description = "账面数量")
    private BigDecimal bookQuantity;

    @Schema(description = "盈亏数量")
    private BigDecimal profitLossQuantity;

    @Schema(description = "实际盘点数量")
    private BigDecimal actualQuantity;

    @Schema(description = "实际金额")
    private BigDecimal actualAmount;

    @Schema(description = "盈亏金额")
    private BigDecimal profitLossAmount;

    @Schema(description = "单价")
    private BigDecimal price;

    @Schema(description = "备注")
    private String remake;

    @Schema(description = "发生时间", example = "2026-07-19 20:57:00")
    private String occurrenceTime;

    @Schema(description = "药品品类信息")
    private ProductCatalogVO productCatalog;
}
