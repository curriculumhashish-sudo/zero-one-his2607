package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "停诊执行结果")
public class StopClinicResultVO {
    @Schema(description = "排班日程ID")
    private String scheduleId;

    @Schema(description = "排班数量")
    private Integer scheduleCount;

    @Schema(description = "号源数量")
    private Integer slotCount;

    @Schema(description = "预约数量")
    private Integer appointmentCount;

    @Schema(description = "已挂号数量")
    private Integer registeredCount;

    @Schema(description = "已签到数量")
    private Integer checkedInCount;

    @Schema(description = "渠道数量")
    private Integer channelCount;

    @Schema(description = "未处理时是否停用")
    private Boolean blockDeactivateIfUnhandled;

    @Schema(description = "默认处置模式")
    private String defaultDispositionMode;

    @Schema(description = "转交匹配规则")
    private String transferMatchRule;

    @Schema(description = "源号源是否冻结")
    private Boolean sourceSlotsFrozen;

    @Schema(description = "冻结号源数量")
    private Integer frozenSlotCount;

    @Schema(description = "冻结过期时间")
    private String freezeExpiresAt;

    @Schema(description = "处置项目列表")
    private List<StopClinicItemVO> items;

    @Schema(description = "成功数量")
    private Integer successCount;

    @Schema(description = "失败数量")
    private Integer failedCount;

    @Schema(description = "排班是否已停用")
    private Boolean scheduleDeactivated;

    @Schema(description = "已停用的号源数量")
    private Integer blockedSlotCount;

    @Schema(description = "成功项目列表")
    private List<StopClinicSuccessItemVO> successItems;

    @Schema(description = "失败项目列表")
    private List<StopClinicFailedItemVO> failedItems;
}