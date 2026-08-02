package com.zeroone.star.project.dto.j8.calendar;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：批量解封号源请求
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@Schema(description = "批量解封号源请求")
public class SlotUnblockDTO {

    @Schema(description = "要解封的号源主键列表", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"322663218269917184\"]")
    @NotEmpty(message = "号源ID列表不能为空")
    private List<String> slotIds;
}
