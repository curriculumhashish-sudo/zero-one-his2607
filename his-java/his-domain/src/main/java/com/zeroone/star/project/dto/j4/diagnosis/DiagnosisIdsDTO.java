package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：诊断ID列表传输对象（用于批量操作）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊断ID列表传输对象")
public class DiagnosisIdsDTO {

    @NotEmpty(message = "诊断ID列表不能为空")
    @Schema(description = "诊断ID列表", example = "[\"id1\", \"id2\", \"id3\"]", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> ids;
}