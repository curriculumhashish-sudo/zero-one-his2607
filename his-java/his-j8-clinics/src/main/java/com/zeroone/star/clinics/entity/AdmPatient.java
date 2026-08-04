package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 患者信息表（跨模块引用）
 * </p>
 *
 * @author aike
 * @since 2026-08-01
 */
@Getter
@Setter
@ToString
@TableName("adm_patient")
public class AdmPatient implements Serializable {

    private static final long serialVersionUID = 1L;

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
    private java.time.LocalDate birthDate;
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
    private String allergyInfo;
    private String remark;
}