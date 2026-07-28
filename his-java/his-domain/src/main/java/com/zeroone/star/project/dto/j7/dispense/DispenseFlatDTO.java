package com.zeroone.star.project.dto.j7.dispense;

import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;


@Data
public class DispenseFlatDTO {

    /** 发药记录编号（业务编号） */
    private String recordNo;

    /** 执行状态 */
    private String statusEnum;

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

    /** 批号 */
    private String lotNumber;

    /** 用法说明 */
    private String dosageInstruction;

    /** 单次剂量 */
    private BigDecimal dose;

    /** 剂量单位 */
    private String doseUnitCode;

    /** 频次编码 */
    private String frequencyCode;

    /** 生产厂家 */
    private String manufacturerText;
}
