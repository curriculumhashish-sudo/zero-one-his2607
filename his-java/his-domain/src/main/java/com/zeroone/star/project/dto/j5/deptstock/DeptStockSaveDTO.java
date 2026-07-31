package com.zeroone.star.project.dto.j5.deptstock;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

/**
 * 取药科室保存DTO
 */
@Data
@Schema(description = "取药科室保存DTO")
public class DeptStockSaveDTO {

    @Valid
    @NotEmpty(message = "科室项目列表不能为空")
    @Schema(description = "科室项目列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<DeptStockItemDTO> items = new ArrayList<>();

    @Data
    @Schema(description = "科室项目DTO")
    public static class DeptStockItemDTO {

        @Schema(description = "记录ID（更新时必填）", example = "1816800000000000001")
        private String id;

        @Schema(description = "科室ID", example = "1816800000000000001")
        private String deptId;

        @Schema(description = "物品ID", example = "ITEM001")
        private String itemId;

        @Schema(description = "物品名称", example = "阿莫西林")
        private String itemName;

        @Schema(description = "物品编码", example = "AMOXICILLIN001")
        private String itemCode;

        @Schema(description = "物品类型", example = "DRUG")
        private String itemType;

        @Schema(description = "库存数量", example = "100")
        private java.math.BigDecimal stockQuantity;

        @Schema(description = "安全库存", example = "10")
        private java.math.BigDecimal safetyStock;

        @Schema(description = "备注")
        private String remark;
    }
}
