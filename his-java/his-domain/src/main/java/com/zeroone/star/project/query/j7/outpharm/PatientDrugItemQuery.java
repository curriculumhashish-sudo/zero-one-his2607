package com.zeroone.star.project.query.j7.outpharm;

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

    @Schema(description = "租户ID", example = "t001")
    private String tenantId;

    @Schema(description = "患者ID", example = "p123456")
    private String patientId;

    @Schema(description = "就诊ID", example = "e789012")
    private String encounterId;

    @Schema(description = "发药状态枚举(dispense_enum)", example = "2")
    private String dispenseEnum;

    @Schema(description = "处方状态枚举(status_enum)", example = "1")
    private String statusEnum;

    @Schema(description = "开单业务号", example = "RX20260101001")
    private String busNo;
}