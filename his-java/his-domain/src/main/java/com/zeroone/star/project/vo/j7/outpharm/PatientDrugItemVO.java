package com.zeroone.star.project.vo.j7.outpharm;

import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 获取患者药品项目 返回前端VO
 * 处方明细 + 药品基础信息 联合返回
 */
@Data
public class PatientDrugItemVO {
    // ========== 处方明细表 medication_dispense 字段 ==========
    private String id;
    private String tenantId;
    private String busNo;
    private String statusEnum;
    private String dispenseEnum;
    private String medicationId;
    private String patientId;
    private String encounterId;
    private String medReqId;
    // 开单数量
    private BigDecimal quantity;
    // 单位编码
    private String unitCode;
    // 已发药数量
    private BigDecimal dispenseQuantity;
    // 批号
    private String lotNumber;
    // 配药人ID、配药时间
    private String preparerId;
    private LocalDateTime prepareTime;
    // 发药人、发药时间
    private String practitionerId;
    private LocalDateTime dispenseTime;
    // 用法用量
    private String dosageInstruction;
    private String methodCode;
    private String frequencyCode;
    private BigDecimal dose;
    private String doseUnitCode;
    private String summaryNo;

    // ========== 药品基础表 medication_definition 联查字段 ==========
    /** 药品名称 */
    private String name;
    /** 商品名 */
    private String merchandiseName;
    /** 药品分类编码 */
    private String categoryCode;
    /** 规格 */
    private String comprisedText;
    /** 成分 */
    private String ingredient;
    /** 国药准字 */
    private String approvalNumber;
    /** 是否皮试 0否1是 */
    private Integer skinTestFlag;
    /** 是否注射剂 */
    private Integer injectFlag;
    /** 生产厂家 */
    private String manufacturerText;
    /** 是否抗生素 */
    private Integer antibioticFlag;
    /** 是否处方药 */
    private Integer rxFlag;
    /** 剂型编码 */
    private String doseFormCode;
    /** 包装单位 */
    private String packageUnitCode;
    /** 药品拼音 */
    private String pyStr;
}
