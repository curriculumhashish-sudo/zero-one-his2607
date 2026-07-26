package com.zeroone.star.project.query.j2.basedata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举值名字列表查询对象")
public class EnumListQuery {
    @Schema(description = "枚举值序号(枚举值的唯一标识)",example = "1",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer enumCode;
    @Schema(description = "枚举类型编码",example = "MALE",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String enumKey;
    @Schema(description = "枚举值名称",example = "男性",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String label;
}
