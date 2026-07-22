package com.zeroone.star.project.dto.j7.outpharm;

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

    @Schema(description = "租户ID", example = "t001")
    private String tenantId;

    @Schema(description = "患者ID（必传，关联患者表）",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "p123456")
    private String patientId;

    @Schema(description = "就诊ID（可选，精确筛选单次就诊处方）", example = "e789012")
    private String encounterId;

    @Schema(description = "发药状态枚举(dispense_enum)：0-未发药 1-部分发药 2-已发药", example = "2")
    private String dispenseEnum;

    @Schema(description = "处方状态枚举(status_enum)：0-待发药 1-已完成 2-已取消", example = "1")
    private String statusEnum;

    @Schema(description = "开单业务号(bus_no)", example = "RX20260101001")
    private String busNo;
}
