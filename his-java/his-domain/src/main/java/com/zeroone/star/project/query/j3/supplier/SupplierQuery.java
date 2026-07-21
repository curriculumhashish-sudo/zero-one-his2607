package com.zeroone.star.project.query.j3.supplier;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;

import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：获取供应商请求对象
 * </p>
 * @author 业之
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
public class SupplierQuery extends PageQuery {

    @Schema(description = "关键词（名称或编号）", example = "华东")
    private String keyword;

    @Schema(description = "供应商类型", example = "生产厂家")
    private String supplierType;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;

    // ---- 以下字段用于供应商列表分页筛选 ----
    @Schema(description = "供应商名称（模糊匹配）", example = "华润")
    private String name;

    @Schema(description = "统一社会信用代码（精确匹配）", example = "91110000100000AAAA")
    private String creditCode;

    @Schema(description = "状态枚举值（ENABLED/DISABLED）", example = "ENABLED")
    private String statusEnum;

    @Schema(description = "类型枚举值（ENTERPRISE/INDIVIDUAL）", example = "ENTERPRISE")
    private String typeEnum;
}
