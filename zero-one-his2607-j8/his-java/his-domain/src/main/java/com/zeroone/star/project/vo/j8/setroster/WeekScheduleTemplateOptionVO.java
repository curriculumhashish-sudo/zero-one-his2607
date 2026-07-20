package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：周排班模板下拉选项
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Data
@Schema(description = "周排班模板下拉选项")
public class WeekScheduleTemplateOptionVO {

    @Schema(description = "模板ID", example = "wk-tpl-001")
    private String id;

    @Schema(description = "模板名称", example = "内科周排班模板")
    private String templateName;
}
