package com.zeroone.star.project.vo.j2.basedata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.ToString;

/**
 * 部位列表视图对象
 */
@Schema(description = "部位列表视图对象")
@Data
@ToString
public class BodyPartListVO {

    @Schema(description = "部位ID", example = "1")
    private Long id;

    @Schema(description = "部位编码", example = "HEAD")
    private String partCode;

    @Schema(description = "部位名称", example = "头部")
    private String partName;

    @Schema(description = "父级ID", example = "")
    private Long parentId;

    @Schema(description = "状态（0-停用 1-启用）", example = "1")
    private Integer status;

    @Schema(description = "是否有子节点（用于前端控制展开箭头显示）", example = "true")
    private Boolean hasChildren;
}