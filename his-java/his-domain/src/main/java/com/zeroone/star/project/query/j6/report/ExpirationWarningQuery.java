package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.util.List;

@Schema(description = "库存物资有效期预警查询条件")
@Data
public class ExpirationWarningQuery {
    @Schema(description = "剩余天数阈值（预警天数内）", example = "90", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer expireWithinDays;

    @Schema(description = "仓库ID列表筛选", example = "[\"322954885380706304\"]")
    private List<String> warehouseIds;
}
