package com.zeroone.star.project.dto.j7.dispense;

import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;


@Data
public class DispenseFlatDTO {

    /** 处方号 */
    private String prescriptionNo;

    /** 记录类型名（发药/退药） */
    private String recordTypeName;

    /** 患者姓名 */
    private String patientName;

    /** 门诊号 */
    private String clinicNo;

    /** 开单科室 */
    private String deptName;

    /** 发药时间 */
    private LocalDateTime dispenseTime;

    /** 药房名称 */
    private String pharmacyName;

    /** 发药人姓名 */
    private String dispenserName;

    /** 操作账号 */
    private String operatorAccount;

    /** 药品名称 */
    private String drugName;

    /** 规格 */
    private String spec;

    /** 发药数量 */
    private BigDecimal quantity;

    /** 单位 */
    private String unit;
}
