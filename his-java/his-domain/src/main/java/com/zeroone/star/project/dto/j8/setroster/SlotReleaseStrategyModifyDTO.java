package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：修改放号策略DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "修改放号策略参数")
public class SlotReleaseStrategyModifyDTO extends SlotReleaseStrategyAddDTO {

    @NotNull(message = "放号策略编号不能为空")
    @Schema(
            description = "放号策略编号",
            example = "1",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Long id;
}
