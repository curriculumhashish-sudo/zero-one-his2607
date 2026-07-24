package com.zeroone.star.project.dto.j4.frequency;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：修改频次配置传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "修改频次上传数据对象")
@Data
@EqualsAndHashCode(callSuper = true)
@AllArgsConstructor
@NoArgsConstructor
public class FrequencyUpdateDTO extends FrequencySaveDTO {

    private static final long serialVersionUID = 1L;

    @NotBlank(message = "主键ID不能为空")
    @Size(max = 64, message = "主键ID长度不能超过64个字符")
    @Schema(description = "主键ID", example = "320010001000000014", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}