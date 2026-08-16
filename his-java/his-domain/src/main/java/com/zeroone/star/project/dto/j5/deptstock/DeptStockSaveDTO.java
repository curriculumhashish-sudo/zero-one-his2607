package com.zeroone.star.project.dto.j5.deptstock;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

/**
 * 取药科室库存保存DTO（映射wkf_inventory_item表）
 */
@Data
@Schema(description = "取药科室库存保存DTO")
public class DeptStockSaveDTO {

    @Valid
    @NotEmpty(message = "库存项目列表不能为空")
    @Schema(description = "库存项目列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<DeptStockItemDTO> items = new ArrayList<>();

    @Data
    @Schema(description = "库存项目DTO")
    public static class DeptStockItemDTO {

        @Schema(description = "记录ID（更新时必填）")
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
        private java.math.BigDecimal quantity;

        @Schema(description = "批号")
        private String lotNumber;

        @Schema(description = "库存地点ID")
        private String locationStoreId;

        @Schema(description = "执行科室ID")
        private String locationId;
    }
}
