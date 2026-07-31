package com.zeroone.star.project.query.j5.deptstock;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 取药科室查询条件
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "取药科室查询条件")
public class DeptStockQuery extends PageQuery {

    @Schema(description = "科室ID", example = "1816800000000000001")
    private String deptId;

    @Schema(description = "科室名称", example = "门诊药房")
    private String deptName;

    @Schema(description = "物品名称", example = "阿莫西林")
    private String itemName;

    @Schema(description = "物品编码", example = "AMOXICILLIN001")
    private String itemCode;

    @Schema(description = "物品类型", example = "DRUG")
    private String itemType;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;
}
