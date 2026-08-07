package com.zeroone.star.project.vo.j5.deptstock;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 取药科室库存VO（映射wkf_inventory_item表）
 */
@Data
@Schema(description = "取药科室库存VO")
public class DeptStockVO {

    @Schema(description = "记录ID")
    private String id;

    @Schema(description = "分类编码")
    private String categoryCode;

    @Schema(description = "物品表（如：drug, consumable）")
    private String itemTable;

    @Schema(description = "物品ID")
    private String itemId;

    @Schema(description = "物品名称")
    private String name;

    @Schema(description = "拼音码")
    private String pyStr;

    @Schema(description = "五笔码")
    private String wbStr;

    @Schema(description = "库存状态枚举")
    private String inventoryStatusEnum;

    @Schema(description = "供应商ID")
    private String supplierId;

    @Schema(description = "描述文本")
    private String descriptionText;

    @Schema(description = "单位编码")
    private String unitCode;

    @Schema(description = "数量")
    private BigDecimal quantity;

    @Schema(description = "特性JSON")
    private String characteristicJson;

    @Schema(description = "批号")
    private String lotNumber;

    @Schema(description = "生产日期")
    private LocalDateTime productionDate;

    @Schema(description = "有效期")
    private LocalDateTime expirationDate;

    @Schema(description = "有效期（月）")
    private Integer validityMon;

    @Schema(description = "库存地点ID")
    private String locationStoreId;

    @Schema(description = "执行科室ID")
    private String locationId;

    @Schema(description = "追踪号")
    private String traceNo;

    @Schema(description = "包装层级")
    private Integer packagingLevels;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;
}
