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
    @Schema(description = "供应商ID列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<Long> ids;

    @NotNull(message = "状态不能为空")
    @Schema(description = "是否启用 1 启用 0 禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isEnabled;
}