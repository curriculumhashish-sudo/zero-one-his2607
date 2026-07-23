package com.zeroone.star.project.vo.j2.basedata.bodypart;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 部位列表视图对象
 */
@Schema(description = "部位列表视图对象")
@Data
public class BodyPartListVO {

    @Schema(description = "部位ID", example = "322663105141149696")
    private String id;

    @Schema(description = "部位编码", example = "HEAD")
    private String partCode;

    @Schema(description = "部位名称", example = "头部")
    private String partName;

    @Schema(description = "父级ID", example = "322663105141149695")
    private String parentId;

    @Schema(description = "上级部位名称", example = "顶级部位")
    private String parentName;

    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;

    @Schema(description = "状态（ACTIVE-启用、INACTIVE-停用）", example = "ACTIVE")
    private String statusEnum;
}
