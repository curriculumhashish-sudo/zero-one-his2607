package com.zeroone.star.project.dto.j8.calendar;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：号源日历列表行 DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode
@Schema(description = "号源日历列表行数据")
public class SlotDTO {

    @Schema(description = "号源主键", example = "322663218269917184")
    private String slotId;

    @Schema(description = "日程主键（前端封锁/解封按此分组）", example = "322663218269917185")
    private String scheduleId;

    @Schema(description = "科室/机构主键", example = "322663218269917186")
    private String organizationId;

    @Schema(description = "医师主键", example = "322663218269917187")
    private String practitionerId;

    @Schema(description = "号源类型主键", example = "322663218269917188")
    private String healthcareServiceId;

    @Schema(description = "科室名称", example = "内科门诊")
    private String organizationName;

    @Schema(description = "医师姓名", example = "张三")
    private String practitionerName;

    @Schema(description = "号源类型名称（普通号/专家号等）", example = "普通号")
    private String healthcareServiceName;

    @Schema(description = "时段开始时刻", example = "2026-07-20T08:00:00")
    private LocalDateTime startTime;

    @Schema(description = "时段结束时刻", example = "2026-07-20T09:00:00")
    private LocalDateTime endTime;

    @Schema(description = "号源状态码", example = "free")
    private String statusCode;

    @Schema(description = "展示状态：可约 / 已满 / 停诊", example = "可约")
    private String displayStatus;

    @Schema(description = "该时段额定容量", example = "30")
    private Integer capacity;

    @Schema(description = "已预约人数", example = "5")
    private Integer bookedCount;

    @Schema(description = "剩余可预约名额", example = "25")
    private Integer remainingCount;

    @Schema(description = "超售标记", example = "false")
    private Boolean overbookedFlag;
}
