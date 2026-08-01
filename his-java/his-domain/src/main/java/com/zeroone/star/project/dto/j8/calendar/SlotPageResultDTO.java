package com.zeroone.star.project.dto.j8.calendar;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：号源分页查询结果（含聚合统计）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@Schema(description = "号源分页查询结果")
public class SlotPageResultDTO {

    @Schema(description = "当前页码", example = "1")
    private long pageIndex;

    @Schema(description = "每页显示条数", example = "10")
    private long pageSize;

    @Schema(description = "总条数", example = "100")
    private long totalCount;

    @Schema(description = "总页数", example = "10")
    private long pages;

    @Schema(description = "当前页数据列表")
    private List<SlotDTO> content;

    @Schema(description = "聚合统计（顶部统计文案用）")
    private SlotStatsDTO stats;
}
