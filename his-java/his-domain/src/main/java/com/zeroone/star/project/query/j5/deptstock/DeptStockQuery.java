package com.zeroone.star.project.query.j5.deptstock;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 取药科室库存查询条件（映射wkf_inventory_item表）
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "取药科室库存查询条件")
public class DeptStockQuery extends PageQuery {

    @Schema(description = "分类编码")
    private String categoryCode;

    @Schema(description = "物品表（如：drug, consumable）")
    private String itemTable;

    @Schema(description = "物品名称")
    private String name;

    @Schema(description = "物品ID")
    private String itemId;

    @Schema(description = "库存状态枚举")
    private String inventoryStatusEnum;

    @Schema(description = "执行科室ID")
    private String locationId;
}
