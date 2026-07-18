package com.zeroone.star.project.vo.j3.fee;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：费用性质视图对象
 * @author 白文儿
 * @version 1.0.0
 */
@Data
public class FeeNatureVO implements Serializable {

    @Schema(description = "费用性质ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "费用性质名称", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "费用性质编码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;

    @Schema(description = "是否启用 1 启用 0 禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isEnabled;
}
