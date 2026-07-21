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

    /* ==================== 1. 通用基本信息 ==================== */

    @Schema(description = "频次代号/编码", example = "QD2", requiredMode = Schema.RequiredMode.REQUIRED)
    private String rateCode;

    @Schema(description = "频次名称", example = "每日两次", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "周期类型: daily(日周期)/weekly(周周期)/interval(间隔周期)/as-needed(必要时)/fixed-count(固定次数)", example = "daily", requiredMode = Schema.RequiredMode.REQUIRED)
    private String cycleType;

    /* ==================== 2. 执行规则条件字段（依据 cycleType 动态使用） ==================== */

    /**
     * 使用场景：
     * - 日周期 (daily)：日执行时点（如：08:00,12:00,18:00）
     * - 周周期 (weekly)：周内执行时点（如：08:00,14:00）
     * - 必要时 (as-needed)：参考时点（可选，如 PRN 额外标注的时点）
     * - 固定次数 (fixed-count)：执行时点（可选，如：08:00）
     */
    @Schema(description = "执行时点串/参考时点，逗号分隔", example = "12:00,18:00")
    private String dayTimes;

    /**
     * 使用场景：
     * - 日周期 (daily)：日间隔（天，隔日=2）
     * - 间隔周期 (interval)：按天间隔填写（如 QOD=2）
     */
    @Schema(description = "日间隔(天)", example = "1")
    private Integer dayInterval;

    /**
     * 使用场景：
     * - 日周期 (daily)：日计数（如「每 N 日」中的 N）
     */
    @Schema(description = "日计数", example = "2")
    private Integer dayCount;

    /**
     * 使用场景：
     * - 周周期 (weekly)：按周周期标识 (0:否, 1:是)
     */
    @Schema(description = "周周期标记 (0:否, 1:是)", example = "1")
    private Byte weekCycleFlag;

    /**
     * 使用场景：
     * - 间隔周期 (interval)：执行周期间隔数值（如 Q8H 填 8）
     */
    @Schema(description = "执行周期间隔数", example = "8")
    private Integer executionPeriod;

    /**
     * 使用场景：
     * - 间隔周期 (interval)：执行周期单位（分钟/小时/天/周/月，如 h/m 等）
     */
    @Schema(description = "执行周期单位(h/m/d/w/M)", example = "h")
    private String executionPeriodUnit;

    /**
     * 使用场景：
     * - 固定次数 (fixed-count)：总执行次数（如 ST=1），必填
     */
    @Schema(description = "总执行次数", example = "1")
    private Integer totalExecutionCount;

    /**
     * 使用场景：
     * - 日周期 (daily) / 周周期 (weekly) / 间隔周期 (interval)：跨日策略
     */
    @Schema(description = "允许跨日: same-day(当天)/cross-day(跨天)", example = "same-day")
    private String allowCrossDay;

    /* ==================== 3. 适用范围及配置 ==================== */

    @Schema(description = "适用场景(多场景逗号分隔或all)", example = "inpatient,outpatient", requiredMode = Schema.RequiredMode.REQUIRED)
    private String applicableScenes;

    @Schema(description = "启用标记 0 禁用 1 启用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Byte activeFlag;

    @Schema(description = "排序号（数值越小越靠前）", example = "10")
    private Integer sortNo;
}