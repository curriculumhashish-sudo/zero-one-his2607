package com.zeroone.star.project.dto.j7.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：盘点单保存明细行
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单保存明细行")
public class StocktakeLineDTO {

    @NotBlank(message = "物品来源表不能为空")
    @Schema(description = "物品来源表名", example = "med_medication_definition", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemTable;

    @NotBlank(message = "物品ID不能为空")
    @Schema(description = "药品/耗材定义ID", example = "3001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemId;

    @NotBlank(message = "计量单位不能为空")
    @Schema(description = "计量单位编码", example = "tablet", requiredMode = Schema.RequiredMode.REQUIRED)
    private String unitCode;

    @NotNull(message = "实盘数量不能为空")
    @DecimalMin(value = "0", message = "实盘数量不能为负数")
    @Schema(description = "实盘数量，不能为负数", example = "95", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal actualQuantity;

    @NotNull(message = "盈亏数量不能为空")
    @Schema(description = "盈亏数量，盘盈为正、盘亏为负、无差异为0", example = "-5", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal profitLossQuantity;

    @Schema(description = "实盘金额", example = "190.00")
    private BigDecimal actualAmount;

    @Schema(description = "盈亏金额，盘盈为正、盘亏为负", example = "-10.00")
    private BigDecimal profitLossAmount;

    @Schema(description = "选品时关联的库存项ID", example = "4001")
    private String inventoryItemId;

    @Schema(description = "批号", example = "BATCH-20260723")
    private String lotNumber;

    @Schema(description = "单价", example = "2.00")
    private BigDecimal price;

    @Schema(description = "行备注", example = "期末盘点")
    private String remark;
}
