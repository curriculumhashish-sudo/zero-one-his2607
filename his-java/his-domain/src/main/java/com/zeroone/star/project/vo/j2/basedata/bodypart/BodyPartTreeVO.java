package com.zeroone.star.project.vo.j2.basedata.bodypart;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.util.List;

/**
 * 部位树节点视图对象
 */
@Schema(description = "部位树节点视图对象")
@Data
public class BodyPartTreeVO {

    @Schema(description = "部位ID", example = "322663105141149696")
    private String id;

    @Schema(description = "部位编码", example = "HEAD")
    private String partCode;

    @Schema(description = "部位名称", example = "头部")
    private String partName;

    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;

    @Schema(description = "子节点列表")
    private List<BodyPartTreeVO> children;
}
