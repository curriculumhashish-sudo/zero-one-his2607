package com.zeroone.star.configuration.dto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Schema(description = "执行地点传输数据对象")
@Data
public class ExecPlaceDTO {

    @Schema(description = "主键（修改时必传）", example = "1")
    private Long id;

    @Schema(description = "执行地点编码", example = "EP001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String placeCode;

    @Schema(description = "执行地点名称", example = "门诊注射室", requiredMode = Schema.RequiredMode.REQUIRED)
    private String placeName;

    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;

    @Schema(description = "是否启用 0-禁用 1-启用", example = "1")
    private Integer activeFlag;
}
