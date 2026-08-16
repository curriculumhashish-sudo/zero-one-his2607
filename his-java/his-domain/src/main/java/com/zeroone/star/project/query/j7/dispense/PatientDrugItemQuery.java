package com.zeroone.star.project.query.j7.dispense;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 患者药品项目 数据库查询条件
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "患者药品项目数据库查询条件")
public class PatientDrugItemQuery extends PageQuery {

    @Schema(description = "租户ID", example = "1")
    private String tenantId;

    @Schema(description = "患者ID", example = "326940849401761792")
    private String patientId;

    @Schema(description = "就诊ID", example = "329874660147597312")
    private String encounterId;

    @Schema(description = "发放类型枚举(dispense_enum)：OUTPATIENT-门诊发药 REFUND-退药", example = "REFUND")
    private String dispenseEnum;

    @Schema(description = "执行状态枚举(status_enum)：PENDING/COMPLETED/REFUNDED/DECLINED", example = "REFUNDED")
    private String statusEnum;

    @Schema(description = "开单业务号(退药单号)", example = "MDP2026-06-290009")
    private String busNo;
}