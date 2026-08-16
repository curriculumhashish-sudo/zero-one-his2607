package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(description = "日程项信息")
public class ScheduleItemVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "日程ID")
    private String scheduleId;

    @Schema(description = "日程名称")
    private String name;

    @Schema(description = "业务状态")
    private String statusEnum;

    @Schema(description = "规划窗口起始时间")
    private LocalDateTime planningHorizonStart;

    @Schema(description = "规划窗口结束时间")
    private LocalDateTime planningHorizonEnd;

    @Schema(description = "医疗服务ID")
    private String healthcareServiceId;

    @Schema(description = "参与者列表")
    private List<ScheduleActorVO> actors;

    @Schema(description = "是否来源模板")
    private Boolean fromTemplate;

    @Schema(description = "周排班模板ID")
    private String weekScheduleTemplateId;

    @Schema(description = "日排班模板ID")
    private String dayScheduleTemplateId;

    @Schema(description = "来源日程模板ID")
    private String sourceScheduleTemplateId;

    @Schema(description = "号源模板ID")
    private String slotProvisionTemplateId;

    @Schema(description = "是否可预约")
    private Boolean bookable;

    @Schema(description = "替诊医师ID")
    private String substitutePractitionerId;

    @Schema(description = "原医师ID")
    private String originalPractitionerId;

    @Schema(description = "替诊显示文本")
    private String substituteDisplay;

    @Schema(description = "医师名称")
    private String practitionerName;

    @Schema(description = "地点名称")
    private String locationName;

    @Schema(description = "医疗服务名称")
    private String healthcareServiceName;
}
