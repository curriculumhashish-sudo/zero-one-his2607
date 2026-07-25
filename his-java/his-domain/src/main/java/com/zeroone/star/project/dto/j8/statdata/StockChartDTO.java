package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "库存出入库图表数据DTO")
public class StockChartDTO {

    @Schema(description = "月份列表")
    private List<String> xAxis;

    @Schema(description = "系列数据")
    private List<SeriesItem> series;

    @Data
    @Schema(description = "系列数据项")
    public static class SeriesItem {
        @Schema(description = "系列名称", example = "药品入库")
        private String name;

        @Schema(description = "数据列表", example = "123.456")
        private List<Double> data;

        @Schema(description = "堆叠名称", example = "入库")
        private String stack;

    }
}