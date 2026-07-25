package com.zeroone.star.project.dto.j3.fee;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：费用性质数据传输对象
 * @author 白文儿
 * @version 1.0.0
 */
@Data
@Schema(description = "费用性质数据对象")
public class FeeNatureDTO implements Serializable {

    @Schema(description = "费用性质ID（新增时不传，修改时必传）", example = "abc123", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String id;

    @NotBlank(message = "名称不能为空")
    @Schema(description = "费用性质名称", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @NotBlank(message = "编码不能为空")
    @Schema(description = "费用性质编码（业务编号）", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @NotNull(message = "显示顺序不能为空")
    @Schema(description = "显示顺序", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer displayOrder;

    @NotBlank(message = "分类不能为空")
    @Schema(description = "费用性质分类(0自费、1公费、2省医保、3市医保)", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryEnum;

    @NotNull(message = "启用状态不能为空")
    @Schema(description = "是否启用 1 启用 0 禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer activeFlag;
}
