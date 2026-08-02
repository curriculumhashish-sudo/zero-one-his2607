package com.zeroone.star.basedata.entity.medicine;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
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
    @TableField("tenant_id")
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(value = "created_at",fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(value = "updated_at",fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
    @TableField("is_deleted")
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    @TableField("deleted_by")
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    @TableField("version")
    private Integer version;

    /**
     * 药品发放聚合根
     */
    @TableField("bus_no")
    private String busNo;

    /**
     * status 枚举值
     */
    @TableField("status_enum")
    private String statusEnum;

    /**
     * notPerformedReason 枚举值
     */
    @TableField("not_performed_reason_enum")
    private String notPerformedReasonEnum;

    /**
     * statusChanged 时间
     */
    @TableField("status_changed_time")
    private LocalDateTime statusChangedTime;

    /**
     * dispense 枚举值
     */
    @TableField("dispense_enum")
    private String dispenseEnum;

    /**
     * medication 标识
     */
    @TableField("medication_id")
    private String medicationId;

    /**
     * 患者 标识
     */
    @TableField("patient_id")
    private String patientId;

    /**
     * 就诊 标识
     */
    @TableField("encounter_id")
    private String encounterId;

    /**
     * 地点 标识
     */
    @TableField("location_id")
    private String locationId;

    /**
     * medReq 标识
     */
    @TableField("med_req_id")
    private String medReqId;

    /**
     * 数量
     */
    @TableField("quantity")
    private BigDecimal quantity;

    /**
     * unit 代码
     */
    @TableField("unit_code")
    private String unitCode;

    /**
     * 发药数量
     */
    @TableField("dispense_quantity")
    private BigDecimal dispenseQuantity;

    /**
     * 批号
     */
    @TableField("lot_number")
    private String lotNumber;

    /**
     * trace 编号
     */
    @TableField("trace_no")
    private String traceNo;

    /**
     * preparer 标识
     */
    @TableField("preparer_id")
    private String preparerId;

    /**
     * prepare 时间
     */
    @TableField("prepare_time")
    private LocalDateTime prepareTime;

    /**
     * practitioner 标识
     */
    @TableField("practitioner_id")
    private String practitionerId;

    /**
     * dispense 时间
     */
    @TableField("dispense_time")
    private LocalDateTime dispenseTime;

    /**
     * limit 时间
     */
    @TableField("limit_time")
    private LocalDateTime limitTime;

    /**
     * plannedDispense 时间
     */
    @TableField("planned_dispense_time")
    private LocalDateTime plannedDispenseTime;

    /**
     * 手术/操作 标识
     */
    @TableField("procedure_id")
    private String procedureId;

    /**
     * summary 编号
     */
    @TableField("summary_no")
    private String summaryNo;

    /**
     * desLocation 标识
     */
    @TableField("des_location_id")
    private String desLocationId;

    /**
     * recPractitioner 标识
     */
    @TableField("rec_practitioner_id")
    private String recPractitionerId;

    /**
     * 用法说明
     */
    @TableField("dosage_instruction")
    private String dosageInstruction;

    /**
     * method 代码
     */
    @TableField("method_code")
    private String methodCode;

    /**
     * 频次 代码
     */
    @TableField("frequency_code")
    private String frequencyCode;

    /**
     * 剂量
     */
    @TableField("dose")
    private BigDecimal dose;

    /**
     * doseUnit 代码
     */
    @TableField("dose_unit_code")
    private String doseUnitCode;

    /**
     * max单元
     */
    @TableField("max_unit")
    private BigDecimal maxUnit;

    /**
     * 辅助信息
     */
    @TableField("support_info")
    private String supportInfo;

    /**
     * partType 代码
     */
    @TableField("part_type_code")
    private String partTypeCode;

    /**
     * dispenseFrequency 代码
     */
    @TableField("dispense_frequency_code")
    private String dispenseFrequencyCode;
}
