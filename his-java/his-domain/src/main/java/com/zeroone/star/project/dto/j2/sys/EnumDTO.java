package com.zeroone.star.project.dto.j2.sys;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举值对象")
public class EnumDTO {
    @Schema(description = "枚举值序号(枚举值的唯一标识)",example = "1")
    private Integer enumCode;
    @Schema(description = "枚举值名称",example = "A型")
    private String label;
    @Schema(description = "枚举值存储字符/编码(枚举值的唯一标识),example = \"A\"")
    private String enumString;
    @Schema(description = "枚举值排序权重(权重越小越靠前)",example = "0")
    private String order;
    @Schema(description = "枚举值描述",example = "枚举类型描述")
    private String enumDescription;
}
