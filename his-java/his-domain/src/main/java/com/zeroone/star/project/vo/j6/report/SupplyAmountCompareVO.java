package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 物资出入库金额对比 — 对齐 J8 StockInOutVO 结构（categories + series + unit）。
 */
@Data
@Schema(description = "物资出入库金额对比")
public class SupplyAmountCompareVO {

    @Schema(description = "月份列表", example = "[\"2026-01\", \"2026-02\"]")
    private List<String> categories;

    @Schema(description = "系列数据")
    private List<SupplySeriesItem> series;

    @Schema(description = "单位", example = "元")
    private String unit;

    @Data
    @Schema(description = "出入库系列项")
    public static class SupplySeriesItem {

        @Schema(description = "名称", example = "药品")
        private String name;

        @Schema(description = "方向：入库/出库", example = "入库")
        private String direction;

        @Schema(description = "数据值列表", example = "[123.45, 456.78]")
        private List<Double> values;
    }
}
