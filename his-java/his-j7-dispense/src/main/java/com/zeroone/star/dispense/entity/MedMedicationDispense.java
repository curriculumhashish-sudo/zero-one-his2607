package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 药品发放记录（med_medication_dispense）
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("med_medication_dispense")
public class MedMedicationDispense implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户ID */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 更新人 */
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

    /** 发药业务编号 */
    private String busNo;

    /**
     * 执行状态 （如 COMPLETED / REFUNDED / DECLINED）
     *  COMPLETED: 发药完成
     *  REFUNDED: 退药已确认通过
     *  DECLINED: 退药被拒绝
     */
    private String statusEnum;

    /** 未执行原因枚举 */
    private String notPerformedReasonEnum;

    /** 状态变更时间 */
    private LocalDateTime statusChangedTime;

    /**
     * 发放类型枚举（如 OUTPATIENT / REFUND）
     *   OUTPATIENT: 发药
     *   REFUND: 退药
     * */
    private String dispenseEnum;

    /** 药品标识（药品定义ID） */
    private String medicationId;

    /** 患者ID */
    private String patientId;

    /** 就诊ID */
    private String encounterId;

    /** 地点/药房ID */
    private String locationId;

    /** 关联药品申请ID */
    private String medReqId;

    /** 数量 */
    private BigDecimal quantity;

    /** 数量单位编码 */
    private String unitCode;

    /** 实际发药退药数量 */
    private BigDecimal dispenseQuantity;

    /** 批号 */
    private String lotNumber;

    /** 追溯码/追踪编号 */
    private String traceNo;

    /** 配药人ID */
    private String preparerId;

    /** 配药时间 */
    private LocalDateTime prepareTime;

    /** 发药人/退药人 */
    private String practitionerId;

    /** 发药时间/退药时间 */
    private LocalDateTime dispenseTime;

    /** 限时时间 */
    private LocalDateTime limitTime;

    /** 计划发药时间 */
    private LocalDateTime plannedDispenseTime;

    /** 手术/操作ID */
    private String procedureId;

    /** 汇总单编号 */
    private String summaryNo;

    /** 目标地点标识 */
    private String desLocationId;

    /** 接收执业者ID */
    private String recPractitionerId;

    /** 用法说明 */
    private String dosageInstruction;

    /** 用法编码 */
    private String methodCode;

    /** 频次编码 */
    private String frequencyCode;

    /** 剂量 */
    private BigDecimal dose;

    /** 剂量单位编码 */
    private String doseUnitCode;

    /** 单次最大剂量 */
    private BigDecimal maxUnit;

    /** 辅助信息 */
    private String supportInfo;

    /** 发药类型编码 */
    private String partTypeCode;

    /** 发药频次编码 */
    private String dispenseFrequencyCode;
}
