package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "出入库堆叠柱状图视图")
public class StockInOutVO {

    @Schema(description = "月份列表", example = "[\"2026-01\", \"2026-02\"]")
    private List<String> categories;

    @Schema(description = "系列数据")
    private List<StockInOutSeriesItem> series;

    @Schema(description = "单位", example = "元")
    private String unit;

    @Data
    @Schema(description = "堆叠柱状图系列项")
    public static class StockInOutSeriesItem {

        @Schema(description = "名称", example = "药品")
        private String name;

        @Schema(description = "方向：入库/出库", example = "入库")
        private String direction;

        @Schema(description = "数据值列表", example = "[123.456, 456.789]")
        private List<Double> values;
    }
}