package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：日排班模板ID响应
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "日排班模板ID响应")
public class DailyScheduleIdVO {

    @Schema(description = "日排班模板ID", example = "341903147124858888")
    private String id;

}
