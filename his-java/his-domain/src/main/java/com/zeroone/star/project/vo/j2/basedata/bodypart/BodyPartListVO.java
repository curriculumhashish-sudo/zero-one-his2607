package com.zeroone.star.project.vo.j2.basedata.bodypart;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Schema(description = "部位列表视图对象")
@Data
public class BodyPartListVO {

    @Schema(description = "部位ID", example = "328090067094999040")
    private String id;

    @Schema(description = "部位编码", example = "BS0001")
    private String partCode;

    @Schema(description = "部位名称", example = "头")
    private String partName;

    @Schema(description = "父级ID", example = "")
    private String parentId;

    @Schema(description = "状态（ACTIVE=启用 INACTIVE=停用）", example = "ACTIVE")
    private String status;

    @Schema(description = "是否有子节点", example = "true")
    private Boolean hasChildren;
}