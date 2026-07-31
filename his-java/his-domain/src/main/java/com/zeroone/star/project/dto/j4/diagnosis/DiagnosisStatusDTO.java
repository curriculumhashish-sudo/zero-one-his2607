package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：诊断启停状态传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊断启停状态传输对象")
public class DiagnosisStatusDTO {

    @NotEmpty(message = "诊断ID列表不能为空")
    @Schema(description = "诊断ID列表", example = "[\"id1\", \"id2\", \"id3\"]", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> ids;

    @NotBlank(message = "状态值不能为空")
    @Size(max = 20, message = "状态值长度不能超过20个字符")
    @Schema(description = "状态值：ACTIVE/INACTIVE", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String status;
}