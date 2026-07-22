package com.zeroone.star.basedata.entity.medicine;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 药品 medication request
 * </p>
 *
 * @author xxm
 * @since 2026-07-18
 */
@Getter
@Setter
@ToString
@TableName("med_medication_request")
public class MedMedicationRequestDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    private Integer version;

    /**
     * 业务编号
     */
    private String busNo;

    /**
     * 处方编号
     */
    private String prescriptionNo;

    /**
     * 分组标识
     */
    private String groupId;

    /**
     * package标识
     */
    private String packageId;

    /**
     * 状态枚举值
     */
    private String statusEnum;

    /**
     * 状态原因
     */
    private Integer statusReason;

    /**
     * 状态changed时间
     */
    private LocalDateTime statusChangedTime;

    /**
     * intent枚举值
     */
    private String intentEnum;

    /**
     * category枚举值
     */
    private String categoryEnum;

    /**
     * priority枚举值
     */
    private String priorityEnum;

    /**
     * perform标志位
     */
    private Boolean performFlag;

    /**
     * 药品标识
     */
    private String medicationId;

    /**
     * 患者标识
     */
    private String patientId;

    /**
     * 就诊标识
     */
    private String encounterId;

    /**
     * 药品申请聚合根
     */
    private String requesterId;

    /**
     * 位置/科室标识
     */
    private String locationId;

    /**
     * 开立科室组织主键（{ adm_机构.id}）
     */
    private String orgId;

    /**
     * 发放药房库位主键（{ adm_地点.id}）；与 { #orgId} 分列，禁止混存
     */
    private Long performLocation;

    /**
     * 执行机构
     */
    private Long performOrg;

    /**
     * performer枚举值
     */
    private String performerEnum;

    /**
     * performer标识
     */
    private String performerId;

    /**
     * 设备def标识
     */
    private String deviceDefId;

    /**
     * 记录人标识
     */
    private String recorderId;

    /**
     * reqauthored时间
     */
    private LocalDateTime reqAuthoredTime;

    /**
     * 报表标志位
     */
    private Boolean reportFlag;

    /**
     * 辅助信息
     */
    private String supportInfo;

    /**
     * 原因JSON
     */
    private String reasonJson;

    /**
     * therapy枚举值
     */
    private String therapyEnum;

    /**
     * effectivedose开始
     */
    private LocalDateTime effectiveDoseStart;

    /**
     * 数量
     */
    private BigDecimal quantity;

    /**
     * 执行次数
     */
    private Integer executeNum;

    /**
     * 单元编码
     */
    private String unitCode;

    /**
     * method编码
     */
    private String methodCode;

    /**
     * rate编码
     */
    private String rateCode;

    /**
     * 剂量
     */
    private BigDecimal dose;

    /**
     * dose单元编码
     */
    private String doseUnitCode;

    /**
     * 最大剂量
     */
    private BigDecimal maxDose;

    /**
     * 字段：first dose
     */
    private BigDecimal firstDose;

    /**
     * firstdose单元编码
     */
    private String firstDoseUnitCode;

    /**
     * first耗时
     */
    private String firstDuration;

    /**
     * 发药interval
     */
    private String dispenseInterval;

    /**
     * 发药per数量
     */
    private Integer dispensePerQuantity;

    /**
     * 发药per耗时
     */
    private Integer dispensePerDuration;

    /**
     * 批号
     */
    private String lotNumber;

    /**
     * 追踪编号
     */
    private String traceNo;

    /**
     * skintest标志位
     */
    private Boolean skinTestFlag;

    /**
     * infusion标志位
     */
    private Boolean infusionFlag;

    /**
     * speed编码
     */
    private String speedCode;

    /**
     * 合同标识
     */
    private String contractId;

    /**
     * 诊断定义主键（列 { 病情_定义_id}，即 { cln_病情_定义.id}）
     */
    private String conditionDefinitionId;

    /**
     * 就诊诊断标识
     */
    private String encounterDiagnosisId;

    /**
     * performercheck标识
     */
    private String performerCheckId;

    /**
     * check时间
     */
    private LocalDateTime checkTime;

    /**
     * effectivedose结束
     */
    private LocalDateTime effectiveDoseEnd;

    /**
     * refundmedicine标识
     */
    private String refundMedicineId;

    /**
     * rx类型编码
     */
    private String rxTypeCode;

    /**
     * 医保class枚举值
     */
    private String ybClassEnum;

    /**
     * 用法说明
     */
    private String dosageInstruction;

    /**
     * 依据表名
     */
    private String basedOnTable;

    /**
     * basedon标识
     */
    private String basedOnId;

    /**
     * tcm标志位
     */
    private Boolean tcmFlag;

    /**
     * 内容JSON
     */
    private String contentJson;

    /**
     * chineseherbsdose数量
     */
    private BigDecimal chineseHerbsDoseQuantity;

    /**
     * suffering标志位
     */
    private Boolean sufferingFlag;

    /**
     * 排序号
     */
    private Integer sortNumber;

    /**
     * generate来源枚举值
     */
    private String generateSourceEnum;

    /**
     * 备注
     */
    private String remark;
}
