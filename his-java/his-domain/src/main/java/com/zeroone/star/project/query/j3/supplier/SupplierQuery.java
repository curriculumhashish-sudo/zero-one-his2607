package com.zeroone.star.project.query.j3.supplier;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;

import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：供应商查询请求对象（根据 SupplierFullDTO 字段设计）
 * </p>
 * @author 业之
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
public class SupplierQuery extends PageQuery {

    // ==================== 通用搜索字段 ====================

    @Schema(description = "关键词（模糊匹配名称、业务编号、信用代码）", example = "华润")
    private String keyword;

    // ==================== 精确筛选字段 ====================

    @Schema(description = "业务编号（精确匹配）", example = "SUP20250001")
    private String busNo;

    @Schema(description = "供应商名称（模糊匹配）", example = "华润医药")
    private String name;

    @Schema(description = "统一社会信用代码（精确匹配）", example = "91110000100000AAAA")
    private String creditCode;

    @Schema(description = "类型枚举值（ENTERPRISE-企业/INDIVIDUAL-个体）", example = "ENTERPRISE")
    private String typeEnum;

    @Schema(description = "状态枚举值（ACTIVE-启用/INACTIVE-停用）", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "机构标识（精确匹配）", example = "ORG001")
    private String orgId;

    @Schema(description = "拼音码（模糊匹配）", example = "HRYY")
    private String pyStr;

    @Schema(description = "五笔码（模糊匹配）", example = "WXDG")
    private String wbStr;

    @Schema(description = "联系电话（模糊匹配）", example = "010-8888")
    private String phone;
}
