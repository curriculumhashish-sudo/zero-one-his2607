package com.zeroone.star.project.vo.j4.frequency;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.io.Serializable;

/**
 * <p>
 * 描述：频次数据显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "频次显示对象")
@Data
public class FrequencyVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID", example = "320010001000000014", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "频次代号", example = "Q12H", requiredMode = Schema.RequiredMode.REQUIRED)
    private String rateCode;

    @Schema(description = "频次名称", example = "每12小时一次", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "周期类型", example = "weekly", requiredMode = Schema.RequiredMode.REQUIRED)
    private String cycleType;

    @Schema(description = "日计数", example = "1")
    private Integer dayCount;

    @Schema(description = "日间隔", example = "0")
    private Integer dayInterval;

    @Schema(description = "日执行时点串", example = "08:00,20:00")
    private String dayTimes;

    @Schema(description = "周周期标记", example = "1")
    private Byte weekCycleFlag;

    @Schema(description = "总执行次数", example = "2")
    private Integer totalExecutionCount;

    @Schema(description = "执行周期", example = "12")
    private Integer executionPeriod;

    @Schema(description = "执行周期单位", example = "h")
    private String executionPeriodUnit;

    @Schema(description = "适用场景", example = "all", requiredMode = Schema.RequiredMode.REQUIRED)
    private String applicableScenes;

    @Schema(description = "允许跨日", example = "cross-day")
    private String allowCrossDay;

    @Schema(description = "启用标记 0 禁用 1 启用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Byte activeFlag;

    @Schema(description = "排序号", example = "140")
    private Integer sortNo;
}
