package com.zeroone.star.project.vo.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：药品项目视图对象
 * </p>
 * <p>
 * 功能说明：用于展示药品明细信息，包含药品编码、名称、规格、单价、数量等。
 * 数据来源于 dispensing_item 表或 return_item 表。
 * 在查看发药药品项目或退药明细时，将数据库记录转换为该 VO 返回给前端。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "药品项目VO")
public class DrugItemVO {
    @Schema(description = "明细ID")
    private String itemId;

    @Schema(description = "药品编码")
    private String drugCode;

    @Schema(description = "药品名称")
    private String drugName;

    @Schema(description = "规格")
    private String spec;

    @Schema(description = "单价")
    private BigDecimal unitPrice;

    @Schema(description = "数量")
    private Integer quantity;

    @Schema(description = "单位")
    private String unit;

    @Schema(description = "金额")
    private BigDecimal amount;

    @Schema(description = "生产批号")
    private String batchNo;

    @Schema(description = "有效期")
    private String expireDate;
}
