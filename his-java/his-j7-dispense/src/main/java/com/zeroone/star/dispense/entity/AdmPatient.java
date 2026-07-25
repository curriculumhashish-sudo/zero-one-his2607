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
import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * 患者（adm_patient）
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("adm_patient")
public class AdmPatient implements Serializable {

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
