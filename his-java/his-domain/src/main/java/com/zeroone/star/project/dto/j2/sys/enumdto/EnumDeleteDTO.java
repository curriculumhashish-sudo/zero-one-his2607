package com.zeroone.star.project.dto.j2.sys.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举类型删除对象")
public class EnumDeleteDTO {
    @Schema(description = "枚举序号(枚举值的唯一标识)",example = "1",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer enumCode;
    @Schema(description = "枚举存储字符/编码(枚举值的唯一标识)",example = "A",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String enumString;
}

