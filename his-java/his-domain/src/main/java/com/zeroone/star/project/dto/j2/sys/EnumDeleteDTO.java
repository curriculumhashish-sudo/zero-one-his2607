package com.zeroone.star.project.dto.j2.sys;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举类型删除对象")
public class EnumDeleteDTO {
    @Schema(description = "枚举类型唯一标识",example = "1",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String enumId;

    @Schema(description = "枚举类型编码",example = "MALE",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String enumCode;
}

