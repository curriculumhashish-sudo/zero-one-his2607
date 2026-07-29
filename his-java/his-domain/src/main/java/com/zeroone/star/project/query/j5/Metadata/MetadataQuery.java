package com.zeroone.star.project.query.j5.Metadata;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "元数据字段查询对象")
public class MetadataQuery extends PageQuery {
    @Schema(description = "关键词：匹配fieldKey / 显示名", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;

    @Schema(description = "取值来源", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String valueSource;

    @Schema(description = "状态：启用/禁用", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String status;
}
