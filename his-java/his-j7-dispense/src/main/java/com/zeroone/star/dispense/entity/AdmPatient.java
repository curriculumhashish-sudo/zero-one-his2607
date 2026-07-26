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

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
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

    /** 患者业务编号 */
    private String busNo;

    /** 患者姓名 */
    private String name;

    /** 姓名扩展 JSON */
    private String nameJson;

    /** 性别编码 */
    private String genderEnum;

    /** 出生日期 */
    private LocalDate birthDate;

    /** 患者状态编码 */
    private String statusEnum;

    /** 启用标记 */
    private Boolean activeFlag;

    /** 临时建档标记 */
    private Boolean tempFlag;

    /** 身份证号 */
    private String idCard;

    /** 婚姻状态编码 */
    private String maritalStatusEnum;

    /** 职业编码 */
    private String prfsEnum;

    /** 联系电话 */
    private String phone;

    /** 地址 */
    private String address;

    /** 地址-省 */
    private String addressProvince;

    /** 地址-市 */
    private String addressCity;

    /** 地址-区/县 */
    private String addressDistrict;

    /** 地址-街道 */
    private String addressStreet;

    /** 地址扩展 JSON */
    private String addressExtendJson;

    /** 民族编码 */
    private String nationalityCode;

    /** 拼音码 */
    private String pyStr;

    /** 五笔码 */
    private String wbStr;

    /** ABO 血型 */
    private Integer bloodTypeAbo;

    /** RH 血型 */
    private Integer bloodTypeRh;

    /** 工作单位 */
    private String workCompany;

    /** 籍贯 */
    private String nativePlace;

    /** 国家/地区编码 */
    private String countryCode;

    /** 联系人姓名 */
    private String linkName;

    /** 联系人关系编码 */
    private Integer linkRelationCode;

    /** 联系人电话 */
    private String linkTelcom;

    /** 主要联系人 JSON */
    private String linkJsons;

    /** 死亡时间 */
    private LocalDateTime deceasedDate;

    /** 死亡原因 */
    private String deceasedReason;

    /** 建档机构标识 */
    private String organizationId;

    /** 合并目标患者标识 */
    private String mergedTargetPatientId;

    /** 合并时间 */
    private LocalDateTime mergedAt;
}
