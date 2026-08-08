package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 疾病分布查询 — 对齐 J8 BaseStatQueryDTO。
 */
@Data
@Schema(description = "疾病分布查询")
public class DiseaseStatisticQuery {
    @Schema(description = "开始日期", example = "2026-01-01")
    private String dateFrom;

    @Schema(description = "结束日期", example = "2026-12-31")
    private String dateTo;

    @Schema(description = "统计粒度", example = "month")
    private String granularity;

    @Schema(description = "机构ID", example = "322663105141149696")
    private String organizationId;
}
