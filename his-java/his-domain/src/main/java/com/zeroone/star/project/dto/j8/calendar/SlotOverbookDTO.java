package com.zeroone.star.project.dto.j8.calendar;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * <p>
 * 描述：标记超售请求
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@Schema(description = "标记超售请求")
public class SlotOverbookDTO {

    @Schema(description = "号源主键", requiredMode = Schema.RequiredMode.REQUIRED, example = "322663218269917184")
    @NotNull(message = "号源ID不能为空")
    private String slotId;

    @Schema(description = "是否开启超售", requiredMode = Schema.RequiredMode.REQUIRED, example = "true")
    @NotNull(message = "超售标记不能为空")
    private Boolean overbookedFlag;
}
