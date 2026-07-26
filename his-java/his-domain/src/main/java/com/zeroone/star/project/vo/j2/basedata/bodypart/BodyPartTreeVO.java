package com.zeroone.star.project.vo.j2.basedata.bodypart;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;


import java.util.List;

@Schema(description = "部位树节点视图对象")
@Data
public class BodyPartTreeVO {

    @Schema(description = "部位ID", example = "328090067094999040")
    private String id;

    @Schema(description = "部位编码", example = "BS0001")
    private String partCode;

    @Schema(description = "部位名称", example = "头")
    private String partName;

    @Schema(description = "子节点列表")
    private List<BodyPartTreeVO> children;
}