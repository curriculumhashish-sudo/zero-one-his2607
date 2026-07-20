package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "预约信息视图")
public class AppointmentVO {
    @Schema(description = "预约ID", example = "1001")
    private Long appointmentId;

    @Schema(description = "患者ID", example = "5001")
    private Long patientId;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Schema(description = "患者手机号", example = "13800138000")
    private String patientPhone;
}