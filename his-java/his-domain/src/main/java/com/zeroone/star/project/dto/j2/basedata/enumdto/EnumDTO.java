package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举类型分页查询对象")
public class EnumDTO {
    @Schema(description = "枚举类型序号",example = "1")
    private String enumId;
    @Schema(description = "枚举类型名称",example = "男性")
    private String enumName;
    @Schema(description = "枚举类型代码",example = "MALE")
    private String enumCode;
    @Schema(description = "枚举类型排序",example = "0")
    private String enumSort;
    @Schema(description = "枚举类型描述",example = "枚举类型描述")
    private String enumDescription;
}
