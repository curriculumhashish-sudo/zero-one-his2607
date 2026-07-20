package com.zeroone.star.project.dto.j5.ons;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "患者队列查询条件")
public class PatientQueueQuery extends PageQuery {

    @Schema(description = "科室ID", example = "1")
    private String department_id;

    @Schema(description = "医生ID", example = "1")
    private String doctor_id;

    @Schema(description = "患者姓名", example = "张三")
    private String patient_name;

    @Schema(description = "就诊状态（0-待就诊，1-就诊中，2-已完成）", example = "0")
    private Integer status;

    @Schema(description = "就诊日期", example = "2024-01-15")
    private String visit_date;
}