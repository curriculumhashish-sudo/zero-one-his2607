package com.zeroone.star.project.dto.j4.frequency;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：频次启停状态传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "频次启停状态传输对象")
public class FrequencyStatusDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @NotBlank(message = "频次ID不能为空")
    @Schema(
            description = "频次ID",
            example = "320010001000000014",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String id;

    @NotNull(message = "启用状态不能为空")
    @Min(value = 0, message = "启用状态只能是0或1")
    @Max(value = 1, message = "启用状态只能是0或1")
    @Schema(
            description = "启用标记：0-停用，1-启用",
            example = "1",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Byte activeFlag;
}