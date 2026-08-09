package com.zeroone.star.project.dto.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
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
    @Size(max = 64, message = "物品标识长度不能超过64个字符")
    @Schema(description = "物品标识", example = "322998385762111488", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemId;

    @Schema(description = "项目名称（药品名称）", example = "阿莫西林胶囊")
    private String itemName;

    @Schema(description = "规格", example = "0.25g*24粒")
    private String specification;

    @Schema(description = "厂家", example = "哈药集团")
    private String manufacturer;

    @Size(max = 255, message = "批号长度不能超过255个字符")
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
    @Size(max = 255, message = "单位编码长度不能超过255个字符")
    @Schema(description = "单位编码", example = "box", requiredMode = Schema.RequiredMode.REQUIRED)
    private String unitCode;

    @Schema(description = "单价", example = "12.50")
    private BigDecimal price;

    @Schema(description = "行合计金额", example = "1187.50")
    private BigDecimal totalPrice;

    @Schema(description = "实盘金额（实盘数量 × 单价）", example = "1187.50")
    private BigDecimal actualAmount;

    @Schema(description = "盘点盈亏金额", example = "-62.50")
    private BigDecimal profitLossPrice;

    @Schema(description = "明细备注", example = "正常损耗")
    private String remark;
}
