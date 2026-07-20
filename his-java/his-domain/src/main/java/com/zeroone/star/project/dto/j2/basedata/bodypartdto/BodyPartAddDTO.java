package com.zeroone.star.project.dto.j2.basedata.bodypartdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 新增部位传输对象
 */
@Schema(description = "新增部位传输对象")
@Getter
@Setter
@ToString
public class BodyPartAddDTO {

    @Schema(description = "部位名称", example = "头部", requiredMode = Schema.RequiredMode.REQUIRED)
    private String partName;

    @Schema(description = "上级部位ID（不选则为顶级）", example = "")
    private Long parentId;
}