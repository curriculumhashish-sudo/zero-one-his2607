package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.util.Map;

/**
 * <p>
 * 描述：放号策略列表数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@Schema(description = "放号策略列表数据")
public class SlotReleaseStrategyDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "放号策略主键ID，前端无需展示", example = "329973511193890816")
    private String id;

    @Schema(description = "放号策略编码", example = "SR000001")
    private String strategyCode;

    @Schema(description = "放号策略名称", example = "提前7天就医")
    private String strategyName;

    @Schema(description = "提前放号天数，0表示当天放号", example = "7")
    private Integer advanceDays;

    @Schema(description = "时段粒度，单位为分钟", example = "30")
    private Integer slotDurationMinutes;

    @Schema(description = "每个时段的号源容量", example = "10")
    private Integer slotCapacity;

    @Schema(
            description = "各渠道配额百分比，供编辑表单回填",
            example = "{\"WINDOW\":60,\"MINIAPP\":40}"
    )
    private Map<String, Integer> channelQuota;
}
