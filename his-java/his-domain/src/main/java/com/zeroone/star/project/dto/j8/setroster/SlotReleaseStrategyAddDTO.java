package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.util.Map;

/**
 * <p>
 * 描述：新增放号策略DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@Schema(description = "新增放号策略参数")
public class SlotReleaseStrategyAddDTO {

    @NotBlank(message = "策略名称不能为空")
    @Size(max = 50, message = "策略名称不能超过50个字符")
    @Schema(
            description = "放号策略名称",
            example = "门诊默认放号",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String strategyName;

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
    private Integer slotDurationMinutes;

    @NotNull(message = "单段容量不能为空")
    @Min(value = 1, message = "单段容量必须大于0")
    @Max(value = 10000, message = "单段容量不能超过10000")
    @Schema(
            description = "每个时段可放出的号源数量",
            example = "10",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Integer slotCapacity;

    @Valid
    @Size(max = 20, message = "渠道数量不能超过20个")
    @Schema(
            description = "各渠道配额百分比，渠道编码为键，百分比为值；配置时百分比总和应为100",
            example = "{\"WINDOW\":60,\"MINIAPP\":40}"
    )
    private Map<
            @NotBlank(message = "渠道编码不能为空") String,
            @NotNull(message = "渠道配额不能为空")
            @Min(value = 0, message = "渠道配额不能小于0")
            @Max(value = 100, message = "渠道配额不能超过100")
                    Integer> channelQuota;
}