package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.util.List;

@Data
@Schema(description = "库存物资有效期预警查询")
public class ExpirationWarningQuery {
    @Schema(description = "开始日期", example = "2026-01-01")
    private String dateFrom;

    @Schema(description = "结束日期", example = "2026-12-31")
    private String dateTo;

    @Schema(description = "统计粒度", example = "month")
    private String granularity;

    @Schema(description = "机构ID", example = "322663105141149696")
    private String organizationId;

    @Schema(description = "剩余天数阈值（预警天数内）", example = "90")
    private Integer expireWithinDays;

    @Schema(description = "仓库ID列表筛选", example = "[\"322954885380706304\"]")
    private List<String> warehouseIds;
}
