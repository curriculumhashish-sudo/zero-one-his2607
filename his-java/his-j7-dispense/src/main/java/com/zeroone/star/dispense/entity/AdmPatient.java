package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.LocalDateTime;
@Getter
@Setter
@TableName("adm_patient")
public class AdmPatient implements Serializable {
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
    private String name;
    private String nameJson;
    private String genderEnum;
    private LocalDate birthDate;
    private String statusEnum;
    private Boolean activeFlag;
    private Boolean tempFlag;
    private String idCard;
    private String maritalStatusEnum;
    private String prfsEnum;
    private String phone;
    private String address;
    private String addressProvince;
    private String addressCity;
    private String addressDistrict;
    private String addressStreet;
    private String addressExtendJson;
    private String nationalityCode;
    private String pyStr;
    private String wbStr;
    private Integer bloodTypeAbo;
    private Integer bloodTypeRh;
    private String workCompany;
    private String nativePlace;
    private String countryCode;
    private String linkName;
    private Integer linkRelationCode;
    private String linkTelcom;
    private String linkJsons;
    private LocalDateTime deceasedDate;
    private String deceasedReason;
    private String organizationId;
    private String mergedTargetPatientId;
    private LocalDateTime mergedAt;
}
