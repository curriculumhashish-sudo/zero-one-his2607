package com.zeroone.star.project.dto.j5.ons;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "预约队列查询条件")
public class PatientQueueQuery extends PageQuery {

    @Schema(description = "机构ID", example = "1")
    private String organization_id;

    @Schema(description = "患者ID", example = "1")
    private String patient_id;

    @Schema(description = "就诊标识", example = "1")
    private String encounter_id;

    @Schema(description = "预约分类", example = "OUTPATIENT_BOOKING")
    private String appointment_kind;

    @Schema(description = "服务类型编码", example = "GENERAL")
    private String service_type_code;

    @Schema(description = "去向诊室ID", example = "1")
    private String destination_location_id;

    @Schema(description = "预约日期", example = "2024-01-15")
    private String booking_date;

    @Schema(description = "分诊等级", example = "LEVEL1")
    private String triage_level;
}