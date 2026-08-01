package com.zeroone.star.project.dto.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

import java.io.Serializable;
import java.util.List;

/**
 * <p>
 * 描述：科室批量操作数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Data
@Schema(description = "科室批量操作数据对象")
public class DepartmentBatchDTO implements Serializable {

    @NotEmpty(message = "科室ID列表不能为空")
    @Schema(description = "科室ID列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> ids;

    @NotNull(message = "状态不能为空")
    @Schema(description = "是否启用, 1-启用, 0-禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    @Pattern(regexp = "^[01]$", message = "状态只能是0-1之间的数字")
    private Integer isEnabled;
}
