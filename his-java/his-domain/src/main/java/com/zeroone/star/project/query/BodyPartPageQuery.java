package com.zeroone.star.project.query;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 部位分页查询对象
 */
@Schema(description = "部位分页查询对象")
@Getter
@Setter
@ToString
public class BodyPartPageQuery {

    @Schema(description = "关键字（部位编码/名称模糊匹配）", example = "头")
    private String keyword;

    @Schema(description = "父级ID（不传查全部）", example = "")
    private Long parentId;

    @Schema(description = "状态（0-停用 1-启用，不传查全部）", example = "")
    private Integer status;

    @Schema(description = "当前页码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer pageIndex = 1;

    @Schema(description = "每页条数", example = "20", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer pageSize = 20;
}