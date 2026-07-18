package com.zeroone.star.project.dto.j3.fee;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.io.Serializable;

/**
 * 描述：费用性质数据传输对象
 * @author 白文儿
 * @version 1.0.0
 */
@Data
@Schema(description = "费用性质数据对象")
public class FeeNatureDTO implements Serializable {

    @Schema(description = "费用性质ID", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Long id;

    @NotBlank(message = "名称不能为空")
    @Schema(description = "费用性质名称", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @NotBlank(message = "编码不能为空")
    @Schema(description = "费用性质编码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;
}
