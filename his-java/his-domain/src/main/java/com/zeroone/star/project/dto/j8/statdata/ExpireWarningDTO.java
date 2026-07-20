package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "过期预警饼图DTO")
public class ExpireWarningDTO {

    @Schema(description = "类别名称", example = "expireInOneMonth")
    private String name;

    @Schema(description = "数量", example = "12")
    private Integer value;
}