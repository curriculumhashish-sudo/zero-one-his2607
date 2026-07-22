package com.zeroone.star.project.query.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "库存图表查询参数")
public class StockChartQuery {

    @Schema(description = "起始月份(yyyy-MM)", example = "2026-01", requiredMode = Schema.RequiredMode.REQUIRED)
    private String startMonth;

    @Schema(description = "结束月份(yyyy-MM)", example = "2026-06", requiredMode = Schema.RequiredMode.REQUIRED)
    private String endMonth;

    @Schema(description = "物资类型筛选列表(DRUG-药品, CONSUMABLE-耗材, INSTRUMENT-器械)", 
            example = "[\"DRUG\", \"CONSUMABLE\", \"INSTRUMENT\"]")
    private List<String> materialTypes;
}