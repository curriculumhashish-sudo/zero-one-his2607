package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

import java.util.List;

@Schema(description = "报表导出查询条件")
@Data
public class FeeDetailExportQuery {
    @Schema(description = "开始日期", example = "2026-01-01")
    private String dateFrom;

    @Schema(description = "结束日期", example = "2026-12-31")
    private String dateTo;

    @Schema(description = "收退方向 (ALL/CHARGE/REFUND)", example = "ALL")
    private String tradeDirection;

    @Schema(description = "挂号科室ID", example = "322663218269917184")
    private String registrationDepartmentId;

    @Schema(description = "费用大类", example = "[\"MEDICATION\"]")
    private List<String> feeCategoryCodes;

    @Schema(description = "费用细分类", example = "[\"1004\"]")
    private List<String> subCategoryCodes;

    @Schema(description = "患者（姓名/门诊号）", example = "张三")
    private String patientKeyword;

    @Schema(description = "项目名称", example = "阿莫西林")
    private String productNameKeyword;

    @Schema(description = "项目编码", example = "YP0001")
    private String productCodeKeyword;

    @Schema(description = "收费员ID", example = "308791351298756608")
    private String cashierId;

    // 向后兼容
    public String getSettlementDateFrom() { return dateFrom; }
    public String getSettlementDateTo() { return dateTo; }
}
