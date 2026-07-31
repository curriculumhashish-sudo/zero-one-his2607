package com.zeroone.star.project.dto.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：盘点单明细保存参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单明细保存参数")
public class StocktakeItemSaveDTO {

    @Schema(description = "明细ID（编辑时可选）", example = "1")
    private String id;

    @Schema(description = "物品表名", example = "med_medication_definition")
    private String itemTable;

    @NotBlank(message = "物品标识不能为空")
    @Schema(description = "物品标识", example = "322998385762111488", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemId;

    @Schema(description = "批号", example = "001-009")
    private String lotNumber;

    @Schema(description = "追溯码", example = "TRACE-001")
    private String traceNo;

    @Schema(description = "账面数量", example = "100.00")
    private BigDecimal bookQuantity;

    @NotNull(message = "实盘数量不能为空")
    @Schema(description = "实盘数量", example = "95.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal totalQuantity;

    @Schema(description = "盈亏数量（可由实盘数量-账面数量计算）", example = "-5.00")
    private BigDecimal itemQuantity;

    @NotBlank(message = "单位编码不能为空")
    @Schema(description = "单位编码", example = "box", requiredMode = Schema.RequiredMode.REQUIRED)
    private String unitCode;

    @Schema(description = "单价", example = "12.50")
    private BigDecimal price;

    @Schema(description = "行合计金额", example = "1187.50")
    private BigDecimal totalPrice;

    @Schema(description = "盘点盈亏金额", example = "-62.50")
    private BigDecimal profitLossPrice;

    @Schema(description = "明细备注", example = "正常损耗")
    private String remark;
}
