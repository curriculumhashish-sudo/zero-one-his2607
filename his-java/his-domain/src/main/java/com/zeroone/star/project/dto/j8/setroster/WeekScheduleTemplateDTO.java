package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：周排班模板传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "周排班模板传输对象")
public class WeekScheduleTemplateDTO extends AddWeekScheduleTemplateDTO {

    @Schema(description = "主键ID", example = "wk-tpl-001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
