package com.zeroone.star.basedata.entity.medicine;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 药品 medication dispense
 * </p>
 *
 * @author xxm
 * @since 2026-07-18
 */
@Getter
@Setter
@ToString
@TableName("med_medication_dispense")
public class MedMedicationDispenseDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(value = "created_at",fill = FieldFill.INSERT)
    private LocalDateTime createTime;

    /**
     * 最后更新时间
     */
    @TableField(value = "updated_at",fill = FieldFill.INSERT)
    private LocalDateTime updateTime;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
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
     * 药品发放聚合根
     */
    private String busNo;

    /**
     * status 枚举值
     */
    private String statusEnum;

    /**
     * notPerformedReason 枚举值
     */
    private String notPerformedReasonEnum;

    /**
     * statusChanged 时间
     */
    private LocalDateTime statusChangedTime;

    /**
     * dispense 枚举值
     */
    private String dispenseEnum;

    /**
     * medication 标识
     */
    private String medicationId;

    /**
     * 患者 标识
     */
    private String patientId;

    /**
     * 就诊 标识
     */
    private String encounterId;

    /**
     * 地点 标识
     */
    private String locationId;

    /**
     * medReq 标识
     */
    private String medReqId;

    /**
     * 数量
     */
    private BigDecimal quantity;

    /**
     * unit 代码
     */
    private String unitCode;

    /**
     * 发药数量
     */
    private BigDecimal dispenseQuantity;

    /**
     * 批号
     */
    private String lotNumber;

    /**
     * trace 编号
     */
    private String traceNo;

    /**
     * preparer 标识
     */
    private String preparerId;

    /**
     * prepare 时间
     */
    private LocalDateTime prepareTime;

    /**
     * practitioner 标识
     */
    private String practitionerId;

    /**
     * dispense 时间
     */
    private LocalDateTime dispenseTime;

    /**
     * limit 时间
     */
    private LocalDateTime limitTime;

    /**
     * plannedDispense 时间
     */
    private LocalDateTime plannedDispenseTime;

    /**
     * 手术/操作 标识
     */
    private String procedureId;

    /**
     * summary 编号
     */
    private String summaryNo;

    /**
     * desLocation 标识
     */
    private String desLocationId;

    /**
     * recPractitioner 标识
     */
    private String recPractitionerId;

    /**
     * 用法说明
     */
    private String dosageInstruction;

    /**
     * method 代码
     */
    private String methodCode;

    /**
     * 频次 代码
     */
    private String frequencyCode;

    /**
     * 剂量
     */
    private BigDecimal dose;

    /**
     * doseUnit 代码
     */
    private String doseUnitCode;

    /**
     * max单元
     */
    private BigDecimal maxUnit;

    /**
     * 辅助信息
     */
    private String supportInfo;

    /**
     * partType 代码
     */
    private String partTypeCode;

    /**
     * dispenseFrequency 代码
     */
    private String dispenseFrequencyCode;
}
