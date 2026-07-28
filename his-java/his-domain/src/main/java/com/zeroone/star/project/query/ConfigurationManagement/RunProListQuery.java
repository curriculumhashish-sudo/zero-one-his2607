package com.zeroone.star.project.query.ConfigurationManagement;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "获取执行项目列表（条件+分页）")
public class RunProListQuery extends PageQuery {
    @Schema(description = "关键词查询", example = "血液", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String likeName;

    @Schema(description = "分类", example = "检查", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String classify;
}
