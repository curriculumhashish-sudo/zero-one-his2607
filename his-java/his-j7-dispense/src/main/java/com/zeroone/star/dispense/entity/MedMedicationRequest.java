package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 药品申请/退药申请（med_medication_request）
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("med_medication_request")
public class MedMedicationRequest implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
    private String updatedBy;

    /** 软删标记 */
    @TableLogic
    private Boolean isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    private Integer version;

    /** 业务编号 */
    private String busNo;

    /** 处方编号 */
    private String prescriptionNo;

    /** 分组标识 */
    private String groupId;

    /** 套餐/包装标识 */
    private String packageId;

    /** 状态枚举 */
    private String statusEnum;

    /** 状态原因 */
    private Integer statusReason;

    /** 状态变更时间 */
    private LocalDateTime statusChangedTime;

    /** 意图枚举（开立意图） */
    private String intentEnum;

    /** 分类枚举 */
    private String categoryEnum;

    /** 优先级枚举 */
    private String priorityEnum;

    /** 是否执行标记 */
    private Boolean performFlag;

    /** 药品标识（药品定义ID） */
    private String medicationId;

    /** 患者标识 */
    private String patientId;

    /** 就诊标识 */
    private String encounterId;

    /** 申请人标识 */
    private String requesterId;

    /** 位置/科室标识 */
    private String locationId;

    /** 开立科室组织标识 */
    private String orgId;

    /** 发放药房库位标识 */
    private Long performLocation;

    /** 执行机构标识 */
    private Long performOrg;

    /** 执行人类型枚举 */
    private String performerEnum;

    /** 执行人标识 */
    private String performerId;

    /** 设备定义标识 */
    private String deviceDefId;

    /** 记录人标识 */
    private String recorderId;

    /** 申请开立时间 */
    private LocalDateTime reqAuthoredTime;

    /** 报表标志 */
    private Boolean reportFlag;

    /** 辅助信息 */
    private String supportInfo;

    /** 原因 JSON */
    private String reasonJson;

    /** 治疗类型枚举 */
    private String therapyEnum;

    /** 有效剂量开始时间 */
    private LocalDateTime effectiveDoseStart;

    /** 数量 */
    private BigDecimal quantity;

    /** 执行次数 */
    private Integer executeNum;

    /** 数量单位编码 */
    private String unitCode;

    /** 用法编码 */
    private String methodCode;

    /** 频次编码 */
    private String rateCode;

    /** 单次剂量 */
    private BigDecimal dose;

    /** 剂量单位编码 */
    private String doseUnitCode;

    /** 最大剂量 */
    private BigDecimal maxDose;

    /** 首次剂量 */
    private BigDecimal firstDose;

    /** 首次剂量单位编码 */
    private String firstDoseUnitCode;

    /** 首次用药时长 */
    private String firstDuration;

    /** 发药间隔 */
    private String dispenseInterval;

    /** 每次发药数量 */
    private Integer dispensePerQuantity;

    /** 每次发药时长 */
    private Integer dispensePerDuration;

    /** 批号 */
    private String lotNumber;

    /** 追溯码/追踪编号 */
    private String traceNo;

    /** 是否皮试 */
    private Boolean skinTestFlag;

    /** 是否输液 */
    private Boolean infusionFlag;

    /** 滴速编码 */
    private String speedCode;

    /** 合同标识 */
    private String contractId;

    /** 诊断定义标识 */
    private String conditionDefinitionId;

    /** 就诊诊断标识 */
    private String encounterDiagnosisId;

    /** 核对人标识 */
    private String performerCheckId;

    /** 核对时间 */
    private LocalDateTime checkTime;

    /** 有效剂量结束时间 */
    private LocalDateTime effectiveDoseEnd;

    /** 关联原发药申请ID（退药申请时指向原申请） */
    private String refundMedicineId;

    /** 处方类型编码 */
    private String rxTypeCode;

    /** 医保分类枚举 */
    private String ybClassEnum;

    /** 用法说明 */
    private String dosageInstruction;

    /** 依据来源表名 */
    private String basedOnTable;

    /** 依据来源记录标识 */
    private String basedOnId;

    /** 是否中药 */
    private Boolean tcmFlag;

    /** 内容 JSON */
    private String contentJson;

    /** 中药剂量数量 */
    private BigDecimal chineseHerbsDoseQuantity;

    /** 是否代煎等加工标记 */
    private Boolean sufferingFlag;

    /** 排序号 */
    private Integer sortNumber;

    /** 生成来源枚举 */
    private String generateSourceEnum;

    /** 备注 */
    private String remark;
}
