package com.zeroone.star.project.query.j2.basedata;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举分页查询对象")
public class EnumQuery extends PageQuery {
    @Schema(description = "枚举值名称",example = "display",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String display;
    @Schema(description = "枚举类型id",example = "1",requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumTypeId;
    @Schema(description = "枚举值代码",example = "enumCode",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String enumCode;
}
