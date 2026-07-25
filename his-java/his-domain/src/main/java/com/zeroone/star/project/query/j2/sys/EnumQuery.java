package com.zeroone.star.project.query.j2.sys;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "枚举关键词查询对象")
public class EnumQuery extends PageQuery {
    @Schema(description = "枚举值名称",example = "男性",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String display;
    @Schema(description = "枚举值编码",example = "MALE",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String enumKey;
}
