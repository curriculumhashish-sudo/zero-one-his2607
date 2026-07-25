package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * <p>
 * 描述：周排班模板明细项
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Data
@Schema(description = "周排班模板明细项")
public class WeekScheduleTemplateItemDTO {

    @Schema(description = "明细主键", example = "item-001")
    private String id;

    @NotNull(message = "星期不能为空")
    @Schema(description = "星期：1=周一...7=周日（ISO）", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer weekday;

    @NotNull(message = "日排班模板ID不能为空")
    @Schema(description = "关联日排班模板ID", example = "tpl-001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dayScheduleTemplateId;

    @Schema(description = "排序号", example = "0")
    private Integer sortNo;

    @Schema(description = "是否启用", example = "true")
    private Boolean enabled;
}
