package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * <p>
 * 描述：诊疗启/停用请求DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊疗启/停用请求DTO")
public class TreatmentStatusDTO {

    @NotBlank(message = "诊疗项目ID不能为空")
    @Size(max = 64, message = "诊疗项目ID不能超过64个字符")
    @Schema(description = "诊疗项目ID", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotBlank(message = "状态不能为空")
    @Pattern(regexp = "ACTIVE|INACTIVE", message = "状态值仅支持 ACTIVE 或 INACTIVE")
    @Schema(description = "状态值：ACTIVE(启用) / INACTIVE(停用)", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;
}
