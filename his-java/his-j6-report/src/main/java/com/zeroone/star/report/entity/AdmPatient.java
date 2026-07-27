package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDate;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 管理域 患者
 * @TableName adm_patient
 * @author wanfengzhiyunyi
 */
@TableName(value ="adm_patient")
@Data
public class AdmPatient implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Integer isDeleted;

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
     * 患者聚合根实体
     */
    private String busNo;

    /**
     * 患者姓名
     */
    private String name;

    /**
     * 姓名扩展 JSON
     */
    private String nameJson;

    /**
     * 性别编码
     */
    private String genderEnum;

    /**
     * 出生日期
     */
    private LocalDate birthDate;

    /**
     * 患者状态编码
     */
    private String statusEnum;

    /**
     * 启用标记
     */
    private Integer activeFlag;

    /**
     * 临时建档标记
     */
    private Integer tempFlag;

    /**
     * 身份证号
     */
    private String idCard;

    /**
     * 婚姻状态编码
     */
    private String maritalStatusEnum;

    /**
     * 职业编码
     */
    private String prfsEnum;

    /**
     * 联系电话
     */
    private String phone;

    /**
     * 地址
     */
    private String address;

    /**
     * 地址province
     */
    private String addressProvince;

    /**
     * 地址city
     */
    private String addressCity;

    /**
     * 地址district
     */
    private String addressDistrict;

    /**
     * 地址street
     */
    private String addressStreet;

    /**
     * 主要地址
     */
    private String addressExtendJson;

    /**
     * 民族编码
     */
    private String nationalityCode;

    /**
     * 拼音码
     */
    private String pyStr;

    /**
     * 五笔码
     */
    private String wbStr;

    /**
     * blood类型abo
     */
    private Integer bloodTypeAbo;

    /**
     * blood类型rh
     */
    private Integer bloodTypeRh;

    /**
     * 血型
     */
    private String workCompany;

    /**
     * 籍贯
     */
    private String nativePlace;

    /**
     * 国家地区编码
     */
    private String countryCode;

    /**
     * link名称
     */
    private String linkName;

    /**
     * linkrelation编码
     */
    private Integer linkRelationCode;

    /**
     * 联系电话
     */
    private String linkTelcom;

    /**
     * 主要联系人
     */
    private String linkJsons;

    /**
     * 死亡时间
     */
    private LocalDateTime deceasedDate;

    /**
     * 死亡原因
     */
    private String deceasedReason;

    /**
     * 建档机构标识
     */
    private String organizationId;

    /**
     * 合并目标患者标识
     */
    private String mergedTargetPatientId;

    /**
     * 合并时间
     */
    private LocalDateTime mergedAt;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}