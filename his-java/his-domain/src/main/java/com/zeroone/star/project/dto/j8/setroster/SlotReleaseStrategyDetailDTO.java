package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.util.Map;

/**
 * <p>
 * 描述：放号策略详情DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "放号策略详情数据")
public class SlotReleaseStrategyDetailDTO extends SlotReleaseStrategyDTO {

    @Schema(
            description = "各渠道配额百分比，渠道编码为键，百分比为值",
            example = "{\"WINDOW\":60,\"MINIAPP\":40}"
    )
    private Map<String, Integer> channelQuota;
}
