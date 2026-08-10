package com.zeroone.star.project.dto.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 获取患者药品项目 前端请求入参DTO
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "患者药品项目查询请求参数")
public class PatientDrugItemDTO extends PageDTO {

    @Schema(description = "租户ID", example = "1")
    private String tenantId;

    @Schema(description = "患者ID（必传，关联患者表）",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "326940849401761792")
    private String patientId;

    @Schema(description = "就诊ID（可选，精确筛选单次就诊处方）", example = "329874660147597312")
    private String encounterId;

    @Schema(description = "发药状态枚举(dispense_enum)：OUTPATIENT-门诊发药 REFUND-退药 INPATIENT-住院发药", example = "REFUND")
    private String dispenseEnum;

    @Schema(description = "处方状态枚举(status_enum)：PENDING-待执行 COMPLETED-已完成 REFUNDED-已退药 DECLINED-退药拒绝", example = "REFUNDED")
    private String statusEnum;

    @Schema(description = "开单业务号(bus_no,退药单号)", example = "MDP2026-06-290009")
    private String busNo;
}
