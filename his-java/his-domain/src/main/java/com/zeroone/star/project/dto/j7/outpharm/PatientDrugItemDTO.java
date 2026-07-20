package com.zeroone.star.project.dto.j7.outpharm;

import com.zeroone.star.project.dto.PageDTO;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 获取患者药品项目 前端请求入参DTO
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class PatientDrugItemDTO extends PageDTO {
    /** 租户ID */
    private String tenantId;
    /** 患者ID（必传，关联患者表） */
    private String patientId;
    /** 就诊ID（可选，精确筛选单次就诊处方） */
    private String encounterId;
    /** 发药状态枚举 dispense_enum */
    private String dispenseEnum;
    /** 处方状态 status_enum */
    private String statusEnum;
    /** 开单业务号 bus_no */
    private String busNo;
}

