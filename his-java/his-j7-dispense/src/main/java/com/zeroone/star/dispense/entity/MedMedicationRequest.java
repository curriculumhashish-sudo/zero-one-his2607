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

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String tenantId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    @TableLogic
    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;

    private String busNo;

    private String prescriptionNo;

    private String groupId;

    private String packageId;

    private String statusEnum;

    private Integer statusReason;

    private LocalDateTime statusChangedTime;

    private String intentEnum;

    private String categoryEnum;

    private String priorityEnum;

    private Boolean performFlag;

    private String medicationId;

    private String patientId;

    private String encounterId;

    private String requesterId;

    private String locationId;

    private String orgId;

    private Long performLocation;

    private Long performOrg;

    private String performerEnum;

    private String performerId;

    private String deviceDefId;

    private String recorderId;

    private LocalDateTime reqAuthoredTime;

    private Boolean reportFlag;

    private String supportInfo;

    private String reasonJson;

    private String therapyEnum;

    private LocalDateTime effectiveDoseStart;

    private BigDecimal quantity;

    private Integer executeNum;

    private String unitCode;

    private String methodCode;

    private String rateCode;

    private BigDecimal dose;

    private String doseUnitCode;

    private BigDecimal maxDose;

    private BigDecimal firstDose;

    private String firstDoseUnitCode;

    private String firstDuration;

    private String dispenseInterval;

    private Integer dispensePerQuantity;

    private Integer dispensePerDuration;

    private String lotNumber;

    private String traceNo;

    private Boolean skinTestFlag;

    private Boolean infusionFlag;

    private String speedCode;

    private String contractId;

    private String conditionDefinitionId;

    private String encounterDiagnosisId;

    private String performerCheckId;

    private LocalDateTime checkTime;

    private LocalDateTime effectiveDoseEnd;

    /** 关联原发药申请ID */
    private String refundMedicineId;

    private String rxTypeCode;

    private String ybClassEnum;

    private String dosageInstruction;

    private String basedOnTable;

    private String basedOnId;

    private Boolean tcmFlag;

    private String contentJson;

    private BigDecimal chineseHerbsDoseQuantity;

    private Boolean sufferingFlag;

    private Integer sortNumber;

    private String generateSourceEnum;

    private String remark;
}
