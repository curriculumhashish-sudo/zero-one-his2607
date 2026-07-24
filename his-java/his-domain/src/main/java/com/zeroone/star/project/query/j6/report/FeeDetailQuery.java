package com.zeroone.star.project.query.j6.report;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Schema(description = "门诊费用明细列表查询条件")
@Data
@EqualsAndHashCode(callSuper = true)
public class FeeDetailQuery extends PageQuery {

    @Schema(description = "收退方向 (ALL:全部, CHARGE:收费, REFUND:退费)", example = "ALL")
    private String tradeDirection;

    @Schema(description = "挂号科室", example = "内科")
    private String registrationDepartmentName;

    @Schema(description = "项目类别", example = "DRUG")
    private String feeCategory;

    @Schema(description = "患者（姓名/门诊号）", example = "张三")
    private String patientKeyword;

    @Schema(description = "项目名称", example = "阿莫西林")
    private String productNameKeyword;

    @Schema(description = "项目编码", example = "YP0001")
    private String productCodeKeyword;

    @Schema(description = "收费员", example = "王收费")
    private String cashierName;

    @Schema(description = "结算日期起始 (YYYY-MM-DD)", example = "2026-01-01")
    private String settlementDateFrom;

    @Schema(description = "结算日期截至 (YYYY-MM-DD)", example = "2026-12-31")
    private String settlementDateTo;
}
