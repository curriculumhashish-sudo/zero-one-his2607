package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "受影响号源视图")
public class AffectedSlotVO {
    @Schema(description = "号源ID", example = "2001")
    private Long slotId;

    @Schema(description = "号源时间段", example = "09:00-10:00")
    private String slotTime;

    @Schema(description = "总容量", example = "20")
    private Integer capacity;

    @Schema(description = "已预约数", example = "15")
    private Integer bookedCount;

    @Schema(description = "活跃预约列表")
    private List<AppointmentVO> activeAppointments;
}