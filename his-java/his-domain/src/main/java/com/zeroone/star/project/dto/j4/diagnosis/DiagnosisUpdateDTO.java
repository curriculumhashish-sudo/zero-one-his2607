package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：修改诊断传输数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "修改诊断传输数据对象")
public class DiagnosisUpdateDTO extends DiagnosisSaveDTO {

    @NotBlank(message = "主键ID不能为空")
    @Size(max = 64, message = "主键ID长度不能超过64个字符")
    @Schema(description = "主键ID", example = "317932056501817344", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}