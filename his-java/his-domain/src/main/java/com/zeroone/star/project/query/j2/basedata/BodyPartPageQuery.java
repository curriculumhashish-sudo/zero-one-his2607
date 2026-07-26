package com.zeroone.star.project.query.j2.basedata;

import io.swagger.v3.oas.annotations.media.Schema;

import lombok.Data;
import lombok.ToString;

@Schema(description = "部位分页查询对象")
@Data
public class BodyPartPageQuery {

    @Schema(description = "关键字（部位编码/名称模糊匹配）", example = "头")
    private String keyword;

    @Schema(description = "父级ID（0=顶级，>0=具体父级）", example = "0")
    private String parentId;

    @Schema(description = "状态（ACTIVE=启用 INACTIVE=停用）", example = "")
    private String status;

    @Schema(description = "当前页码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer pageIndex = 1;

    @Schema(description = "每页条数", example = "20", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer pageSize = 20;
}