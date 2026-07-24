package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：日排班模板传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */

@Data
@Schema(description = "日排班模板传输对象")
public class DailyScheduleDTO extends AddDailyScheduleDTO{
    @Schema(description = "排班ID（修改时必填，新增时忽略）", example = "328058080426201088", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
