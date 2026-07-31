package com.zeroone.star.project.dto.j5.bindingMapping;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 绑定映射初始化响应DTO
 */
@Data
@Schema(description = "绑定映射初始化响应DTO")
public class BindingInitDTO {

    @Schema(description = "诊疗类型下拉选项")
    private List<OptionItem> treatmentOptions;

    @Schema(description = "用法类型下拉选项")
    private List<OptionItem> usageOptions;

    @Schema(description = "科室下拉选项")
    private List<OptionItem> orgOptions;

    @Schema(description = "物品类型下拉选项")
    private List<OptionItem> itemTypeOptions;

    @Data
    @Schema(description = "选项项")
    public static class OptionItem {
        @Schema(description = "选项值")
        private String value;

        @Schema(description = "选项标签")
        private String label;
    }
}
