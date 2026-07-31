package com.zeroone.star.project.vo.j5.deptstock;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 取药科室VO
 */
@Data
@Schema(description = "取药科室VO")
public class DeptStockVO {

    @Schema(description = "记录ID", example = "1816800000000000001")
    private String id;

    @Schema(description = "科室ID", example = "1816800000000000001")
    private String deptId;

    @Schema(description = "科室名称", example = "门诊药房")
    private String deptName;

    @Schema(description = "物品ID", example = "ITEM001")
    private String itemId;

    @Schema(description = "物品名称", example = "阿莫西林")
    private String itemName;

    @Schema(description = "物品编码", example = "AMOXICILLIN001")
    private String itemCode;

    @Schema(description = "物品类型", example = "DRUG")
    private String itemType;

    @Schema(description = "物品类型名称", example = "药品")
    private String itemTypeName;

    @Schema(description = "规格", example = "0.25g*24片")
    private String specification;

    @Schema(description = "单位", example = "盒")
    private String unit;

    @Schema(description = "库存数量", example = "100")
    private BigDecimal stockQuantity;

    @Schema(description = "安全库存", example = "10")
    private BigDecimal safetyStock;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;

    @Schema(description = "备注")
    private String remark;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;
}
