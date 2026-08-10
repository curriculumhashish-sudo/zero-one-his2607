package com.zeroone.star.project.query.j7.dispense;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalDateTime;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "患者队列查询条件")
public class PatientQueueQuery extends PageQuery {

    @Schema(description = "患者ID", example = "326940849401761792")
    private String patientId;

    @Schema(description = "就诊ID", example = "329874660147597312")
    private String encounterId;

    @Schema(description = "发药状态(COMPLETED/PENDING)", example = "PENDING")
    private String statusEnum;

    @Schema(description = "发放类型(OUTPATIENT/REFUND)", example = "OUTPATIENT")
    private String dispenseEnum;

    @Schema(description = "患者姓名(模糊查询)", example = "周女士")
    private String name;

    @Schema(description = "查询开始时间", example = "2026-06-29T00:00:00")
    private LocalDateTime startTime;

    @Schema(description = "查询结束时间", example = "2026-06-29T23:59:59")
    private LocalDateTime endTime;
}
