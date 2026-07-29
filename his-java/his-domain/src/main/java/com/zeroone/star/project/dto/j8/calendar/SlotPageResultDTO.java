package com.zeroone.star.project.dto.j8.calendar;

import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：号源分页查询结果（继承 PageDTO，额外携带聚合统计）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "号源分页查询结果")
public class SlotPageResultDTO extends PageDTO<SlotDTO> {

    @Schema(description = "聚合统计（顶部统计文案用）")
    private SlotStatsDTO stats;
}
