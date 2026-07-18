package com.zeroone.star.project.dto.j4.frequency;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.io.Serializable;

/**
 * <p>
 * 描述：新增频次配置传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "新增频次上传数据对象")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class FrequencySaveDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "频次代号/编码", example = "QD2", requiredMode = Schema.RequiredMode.REQUIRED)
    private String rateCode;

    @Schema(description = "频次名称", example = "每日两次", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "周期类型: daily/weekly/interval/as-needed/fixed-count", example = "daily", requiredMode = Schema.RequiredMode.REQUIRED)
    private String cycleType;

    @Schema(description = "日计数", example = "2")
    private Integer dayCount;

    @Schema(description = "日间隔", example = "1")
    private Integer dayInterval;

    @Schema(description = "日执行具体时点串", example = "12:00,18:00")
    private String dayTimes;

    @Schema(description = "周周期标记 (0:否, 1:是)", example = "1")
    private Byte weekCycleFlag;

    @Schema(description = "总执行次数", example = "1")
    private Integer totalExecutionCount;

    @Schema(description = "执行周期间隔数", example = "8")
    private Integer executionPeriod;

    @Schema(description = "执行周期单位(h/m)", example = "h")
    private String executionPeriodUnit;

    @Schema(description = "适用场景(多场景逗号分隔或all)", example = "inpatient,outpatient", requiredMode = Schema.RequiredMode.REQUIRED)
    private String applicableScenes;

    @Schema(description = "允许跨日: same-day(当天)/cross-day(跨天)", example = "same-day")
    private String allowCrossDay;

    @Schema(description = "启用标记 0 禁用 1 启用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Byte activeFlag;

    @Schema(description = "排序号", example = "10")
    private Integer sortNo;
}
