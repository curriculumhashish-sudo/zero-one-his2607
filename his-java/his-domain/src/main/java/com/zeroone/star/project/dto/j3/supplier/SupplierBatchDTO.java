package com.zeroone.star.project.dto.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import lombok.Data;
import java.io.Serializable;
import java.util.List;

/**
 * <p>
 * 描述：供应商批量操作数据对象
 * </p>
 * @author 业之
 * @version 1.0.0
 */
@Data
@Schema(description = "供应商批量操作数据对象")
public class SupplierBatchDTO implements Serializable {

    @NotEmpty(message = "供应商ID列表不能为空")
    @Schema(description = "供应商ID列表", requiredMode = Schema.RequiredMode.REQUIRED,example = "[4100000000000000002,4100000000000000003,4100000000000000004]")
    private List<String> ids;

    @NotNull(message = "状态不能为空")
    @Schema(description = "状态枚举值（ENABLED-启用/DISABLED-停用）", example = "ENABLED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;
}