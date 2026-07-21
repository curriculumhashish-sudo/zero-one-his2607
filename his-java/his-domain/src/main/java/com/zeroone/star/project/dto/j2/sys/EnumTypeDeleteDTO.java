package com.zeroone.star.project.dto.j2.sys;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举类型删除对象")
public class EnumTypeDeleteDTO {
    @Schema(description = "枚举类型唯一键",example = "BloodTypeAbo",requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumKey;
}
