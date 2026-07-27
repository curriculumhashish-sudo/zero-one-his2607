package com.zeroone.star.j7.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
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
 * 患者药品项目（处方药品明细）。
 *
 * @author lvv
 * @since 2026-07-27
 */
// lvv：对应数据库表 med_medication_request，供患者处方药品明细业务使用。
@Getter
@Setter
@ToString
@TableName("med_medication_request")
public class PrescriptionDrugItem implements Serializable {

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

    /** 软删除标记 */
    @TableLogic
    @TableField("is_deleted")
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

    /** 包标识 */
    private String packageId;

    /** 状态枚举值 */
    private String statusEnum;

    /** 状态原因 */
    private Integer statusReason;

    /** 状态变更时间 */
    private LocalDateTime statusChangedTime;

    /** 意图枚举值 */
    private String intentEnum;

    /** 类别枚举值 */
    private String categoryEnum;

    /** 优先级枚举值 */
    private String priorityEnum;

    /** 是否执行 */
    private Boolean performFlag;

    /** 药品标识 */
    private String medicationId;

    /** 患者标识 */
    private String patientId;

    /** 就诊标识 */
    private String encounterId;

    /** 申请人标识 */
    private String requesterId;

    /** 位置或科室标识 */
    private String locationId;

    /** 开立科室组织主键 */
    private String orgId;

    /** 发放药房库位主键 */
    private Long performLocation;

    /** 执行机构主键 */
    private Long performOrg;

    /** 执行者类型枚举值 */
    private String performerEnum;

    /** 执行者标识 */
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

    /** 治疗类型枚举值 */
    private String therapyEnum;

    /** 有效剂量开始时间 */
    private LocalDateTime effectiveDoseStart;

    /** 数量 */
    private BigDecimal quantity;

    /** 执行次数 */
    private Integer executeNum;

    /** 单位编码 */
    private String unitCode;

    /** 用药方法编码 */
    private String methodCode;

    /** 给药频率编码 */
    private String rateCode;

    /** 剂量 */
    private BigDecimal dose;

    /** 剂量单位编码 */
    private String doseUnitCode;

    /** 最大剂量 */
    private BigDecimal maxDose;

    /** 首次剂量 */
    private BigDecimal firstDose;

    /** 首次剂量单位编码 */
    private String firstDoseUnitCode;

    /** 首次用药耗时 */
    private String firstDuration;

    /** 发药间隔 */
    private String dispenseInterval;

    /** 单次发药数量 */
    private Integer dispensePerQuantity;

    /** 单次发药耗时 */
    private Integer dispensePerDuration;

    /** 批号 */
    private String lotNumber;

    /** 追踪编号 */
    private String traceNo;

    /** 是否需要皮试 */
    private Boolean skinTestFlag;

    /** 是否输液 */
    private Boolean infusionFlag;

    /** 滴速编码 */
    private String speedCode;

    /** 合同标识 */
    private String contractId;

    /** 诊断定义主键 */
    private String conditionDefinitionId;

    /** 就诊诊断标识 */
    private String encounterDiagnosisId;

    /** 执行核对人标识 */
    private String performerCheckId;

    /** 核对时间 */
    private LocalDateTime checkTime;

    /** 有效剂量结束时间 */
    private LocalDateTime effectiveDoseEnd;

    /** 退药记录标识 */
    private String refundMedicineId;

    /** 处方类型编码 */
    private String rxTypeCode;

    /** 医保分类枚举值 */
    private String ybClassEnum;

    /** 用法说明 */
    private String dosageInstruction;

    /** 依据表名 */
    private String basedOnTable;

    /** 依据记录标识 */
    private String basedOnId;

    /** 是否中药 */
    private Boolean tcmFlag;

    /** 处方内容 JSON */
    private String contentJson;

    /** 中药剂量 */
    private BigDecimal chineseHerbsDoseQuantity;

    /** 是否代煎 */
    private Boolean sufferingFlag;

    /** 排序号 */
    private Integer sortNumber;

    /** 生成来源枚举值 */
    private String generateSourceEnum;

    /** 备注 */
    private String remark;
}
