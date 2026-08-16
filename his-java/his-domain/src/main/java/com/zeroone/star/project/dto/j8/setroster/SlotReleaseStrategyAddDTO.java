package com.zeroone.star.project.dto.j8.setroster;

import com.fasterxml.jackson.annotation.JsonAlias;
import com.fasterxml.jackson.annotation.JsonIgnore;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.AssertTrue;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.util.Map;

/**
 * <p>
 * 描述：新建放号策略DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@Schema(description = "新建放号策略参数")
public class SlotReleaseStrategyAddDTO {

    @NotBlank(message = "策略名称不能为空")
    @Size(max = 50, message = "策略名称不能超过50个字符")
    @Schema(
            description = "放号策略名称",
            example = "门诊默认放号",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @JsonAlias("strategyName")
    private String policyName;

    @NotNull(message = "提前天数不能为空")
    @Min(value = 0, message = "提前天数不能小于0")
    @Max(value = 365, message = "提前天数不能超过365天")
    @Schema(
            description = "提前放号天数，0表示当天放号",
            example = "7",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Integer advanceDays;

    @NotNull(message = "时段粒度不能为空")
    @Min(value = 1, message = "时段粒度必须大于0")
    @Max(value = 1440, message = "时段粒度不能超过1440分钟")
    @Schema(
            description = "每个放号时段的分钟数",
            example = "30",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @JsonAlias("slotDurationMinutes")
    private Integer slotMinutes;

    @NotNull(message = "单段容量不能为空")
    @Min(value = 1, message = "单段容量必须大于0")
    @Max(value = 10000, message = "单段容量不能超过10000")
    @Schema(
            description = "每个时段可放出的号源数量",
            example = "10",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @JsonAlias("slotCapacity")
    private Integer capacityPerSlot;

    @Schema(
            description = "渠道配额JSON",
            example = "{\"WINDOW\":60,\"MINIAPP\":40}"
    )
    @JsonAlias("channelQuota")
    private Map<String, Integer> channelQuotaJson;

    @AssertTrue(message = "渠道配额百分比总和必须为100")
    @JsonIgnore
    @Schema(hidden = true)
    public boolean isChannelQuotaTotalValid() {
        return channelQuotaJson == null
                || channelQuotaJson.isEmpty()
                || channelQuotaJson.values().stream().allMatch(value -> value != null)
                && channelQuotaJson.values().stream().mapToInt(Integer::intValue).sum() == 100;
    }
}