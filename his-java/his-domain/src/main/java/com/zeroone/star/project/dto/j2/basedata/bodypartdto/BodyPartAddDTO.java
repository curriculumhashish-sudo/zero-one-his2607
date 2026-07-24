package com.zeroone.star.project.dto.j2.basedata.bodypartdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 新增部位传输对象
 */
@Schema(description = "新增部位传输对象")
@Data
public class BodyPartAddDTO {

    @NotBlank(message = "部位名称不能为空")
    @Schema(description = "部位名称", example = "头部", requiredMode = Schema.RequiredMode.REQUIRED)
    private String partName;

    @Schema(description = "上级部位ID（不选则为顶级）", example = "322663105141149695")
    private String parentId;
}
