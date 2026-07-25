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
@TableName("adm_practitioner")
public class AdmPractitioner implements Serializable {
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
    private Long version;
    private String name;
    private String nameJson;
    private String pyStr;
    private String wbStr;
    private String genderEnum;
    private LocalDate birthDate;
    private LocalDateTime deceasedDate;
    private String phone;
    private String addressProvince;
    private String addressCity;
    private String addressDistrict;
    private String addressStreet;
    private String addressJson;
    private String statusEnum;
    private Boolean activeFlag;
    private String busNo;
    private String ybNo;
    private String userId;
    private String orgId;
    private Integer drProfttlCode;
    private String pharPracCertNo;
    private String prscDrCertCode;
    private String signature;
    private String kpdCode;
    private String posNo;
}