package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("med_medication_dispense")
public class MedicationDispense implements Serializable {
    private static final long serialVersionUID = 1L;
    @TableId
    private String id;
    private String tenantId;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    private Boolean isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;
    private Integer version;
    private String busNo;
    private String statusEnum;
    private String notPerformedReasonEnum;
    private LocalDateTime statusChangedTime;
    private String dispenseEnum;
    private String medicationId;
    private String patientId;
    private String encounterId;
    private String locationId;
    private String medReqId;
    private BigDecimal quantity;
    private String unitCode;
    private BigDecimal dispenseQuantity;
    private String lotNumber;
    private String traceNo;
    private String preparerId;
    private LocalDateTime prepareTime;
    private String practitionerId;
    private LocalDateTime dispenseTime;
    private LocalDateTime limitTime;
    private LocalDateTime plannedDispenseTime;
    private String procedureId;
    private String summaryNo;
    private String desLocationId;
    private String recPractitionerId;
    private String dosageInstruction;
    private String methodCode;
    private String frequencyCode;
    private BigDecimal dose;
    private String doseUnitCode;
    private BigDecimal maxUnit;
    private String supportInfo;
    private String partTypeCode;
    private String dispenseFrequencyCode;
}
