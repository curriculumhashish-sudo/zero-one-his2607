package com.zeroone.star.project.vo.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：盘点单明细项显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单明细项")
public class StocktakeItemVO {

    @Schema(description = "明细ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "药品/物品标识", example = "1001")
    private String itemId;

    @Schema(description = "药品名称", example = "阿司匹林肠溶片")
    private String itemName;

    @Schema(description = "药品名称（drugName 别名，兼容前端字段命名）", example = "阿司匹林肠溶片")
    private String drugName;

    @Schema(description = "规格", example = "100mg*30片")
    private String specification;

    @Schema(description = "规格（spec 别名，兼容前端字段命名）", example = "100mg*30片")
    private String spec;

    @Schema(description = "厂家", example = "拜耳医药保健有限公司")
    private String manufacturer;

    @Schema(description = "批号", example = "20260101")
    private String lotNumber;

    @Schema(description = "账面数量（拆零单位）", example = "100")
    private BigDecimal bookQuantity;

    @Schema(description = "实盘数量（拆零单位）", example = "95")
    private BigDecimal totalQuantity;

    @Schema(description = "盈亏数量（盘盈为正，盘亏为负）", example = "-5")
    private BigDecimal itemQuantity;

    @Schema(description = "单位编码", example = "box")
    private String unitCode;

    @Schema(description = "单位名称", example = "盒")
    private String unitName;

    @Schema(description = "单价", example = "12.50")
    private BigDecimal price;

    @Schema(description = "实盘金额", example = "1187.50")
    private BigDecimal totalPrice;

    @Schema(description = "盈亏金额（盘盈为正，盘亏为负）", example = "-62.50")
    private BigDecimal profitLossPrice;

    @Schema(description = "备注", example = "正常损耗")
    private String remark;
}
