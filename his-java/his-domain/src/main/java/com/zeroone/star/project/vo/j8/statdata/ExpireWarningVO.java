package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "过期预警饼图视图")
public class ExpireWarningVO {

    @Schema(description = "预警数据项列表")
    private List<ExpireWarningItem> items;

    @Schema(description = "单位", example = "种")
    private String unit;

    @Data
    @Schema(description = "过期预警饼图数据项")
    public static class ExpireWarningItem {

        @Schema(description = "类别名称", example = "expireInOneMonth")
        private String name;

        @Schema(description = "数量", example = "12")
        private Integer value;
    }
}