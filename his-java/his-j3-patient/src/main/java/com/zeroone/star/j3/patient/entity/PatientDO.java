package com.zeroone.star.j3.patient.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.FieldStrategy;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * 患者档案数据库实体。
 *
 * @author 照夜清
 */
@Data
@TableName("adm_patient")
public class PatientDO {

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String tenantId;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    @TableLogic(value = "0", delval = "1")
    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;

    private String busNo;

    private String name;

    private String genderEnum;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private LocalDate birthDate;

    private String statusEnum;

    private Boolean activeFlag;

    private Boolean tempFlag;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String idCard;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String maritalStatusEnum;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String prfsEnum;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String phone;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String address;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String addressProvince;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String addressCity;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String addressDistrict;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String addressStreet;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String nationalityCode;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String pyStr;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String wbStr;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private Integer bloodTypeAbo;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private Integer bloodTypeRh;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String workCompany;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String nativePlace;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String countryCode;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String linkName;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private Integer linkRelationCode;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String linkTelcom;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private LocalDateTime deceasedDate;

    private String organizationId;
}
