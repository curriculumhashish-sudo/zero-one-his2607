package com.zeroone.star.project.dto.j5.configuration.orderset;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.PositiveOrZero;
import lombok.Data;

/**
 * 启用或停用组套模板传输对象。
 */
@Data
@Schema(description = "启用或停用组套模板")
public class OrderSetStatusDTO {

    @NotBlank(message = "组套模板编号不能为空")
    @Schema(description = "组套模板编号", example = "329888224518344704", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotNull(message = "版本号不能为空")
    @PositiveOrZero(message = "版本号不能小于0")
    @Schema(description = "聚合乐观锁版本号", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;

    @NotBlank(message = "状态操作不能为空")
    @Pattern(regexp = "PUBLISH|RETIRE|ENABLE|DISABLE", message = "状态操作只能是PUBLISH、RETIRE、ENABLE或DISABLE")
    @Schema(description = "生命周期操作；推荐使用PUBLISH或RETIRE，兼容ENABLE或DISABLE", example = "PUBLISH", requiredMode = Schema.RequiredMode.REQUIRED)
    private String operationType;
}
