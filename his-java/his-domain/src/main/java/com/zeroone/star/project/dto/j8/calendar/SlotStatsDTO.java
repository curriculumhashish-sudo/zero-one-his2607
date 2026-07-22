package com.zeroone.star.project.dto.j8.calendar;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：号源列表聚合统计（顶部统计文案用）
 * </p>
 * <p>示例：<code>2026-07-19 周日 本页1个时段 容量30 已约0 剩余30</code></p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@Schema(description = "号源列表聚合统计")
public class SlotStatsDTO {

    @Schema(description = "当前页总时段数量", example = "12")
    private Integer totalSlots;

    @Schema(description = "当前页合计总容量", example = "360")
    private Integer totalCapacity;

    @Schema(description = "当前页合计已预约人数", example = "85")
    private Integer totalBooked;

    @Schema(description = "当前页合计剩余号量", example = "275")
    private Integer totalRemaining;
}
