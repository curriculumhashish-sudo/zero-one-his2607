package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：日排班下拉选项
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */

@Data
@Schema(description = "日排班下拉选项")
public class DailyScheduleOptionVO {
    @Schema(description = "排班模板ID", example = "tpl-001")
    private String id;
    @Schema(description = "排班模板名称", example = "门诊医生")
    private String templateName;
}
