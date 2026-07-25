package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.io.Serializable;

import com.zeroone.star.project.entity.BaseDO;
import lombok.*;

/**
 * <p>
 * 医务人员主数据
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("adm_practitioner")
public class Practitioner extends BaseDO {
    /**
     * 租户标识
     */
    @TableField("tenant_id")
    private String tenantId;

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
    @TableField("version")
    private Long version;

    /**
     * 人员姓名
     */
    @TableField("name")
    private String name;

    /**
     * 其他名称 JSON（曾用名、英文名等）
     */
    @TableField("name_json")
    private String nameJson;

    /**
     * 姓名拼音检索码
     */
    @TableField("py_str")
    private String pyStr;

    /**
     * 姓名五笔检索码
     */
    @TableField("wb_str")
    private String wbStr;

    /**
     * 行政性别编码
     */
    @TableField("gender_enum")
    private String genderEnum;

    /**
     * 出生日期
     */
    @TableField("birth_date")
    private LocalDate birthDate;

    /**
     * 死亡时间
     */
    @TableField("deceased_date")
    private LocalDateTime deceasedDate;

    /**
     * 联系电话
     */
    @TableField("phone")
    private String phone;

    /**
     * 地址-省
     */
    @TableField("address_province")
    private String addressProvince;

    /**
     * 地址-市
     */
    @TableField("address_city")
    private String addressCity;

    /**
     * 地址-区/县
     */
    @TableField("address_district")
    private String addressDistrict;

    /**
     * 地址-街道/详细地址
     */
    @TableField("address_street")
    private String addressStreet;

    /**
     * 地址扩展 JSON
     */
    @TableField("address_json")
    private String addressJson;

    /**
     * 人员状态（ACTIVE / INACTIVE / DISCARDED）
     */
    @TableField("status_enum")
    private String statusEnum;

    /**
     * 是否启用（status_enum == ACTIVE 时为 true）
     */
    @TableField("active_flag")
    private Boolean activeFlag;

    /**
     * 院内工号/业务编号
     */
    @TableField("bus_no")
    private String busNo;

    /**
     * 医保人员编码
     */
    @TableField("yb_no")
    private String ybNo;

    /**
     * 关联平台用户主键（与 id 独立，无库表外键）
     */
    @TableField("user_id")
    private String userId;

    /**
     * 默认登录科室/当前工作科室（Organization 主键）
     */
    @TableField("org_id")
    private String orgId;

    /**
     * 职称编码
     */
    @TableField("dr_profttl_code")
    private Integer drProfttlCode;

    /**
     * 医师/药师执业资格证编号
     */
    @TableField("phar_prac_cert_no")
    private String pharPracCertNo;

    /**
     * 执业证件类型编码
     */
    @TableField("prsc_dr_cert_code")
    private String prscDrCertCode;

    /**
     * 医生签名图片 Base64（可含 data:image/*;base64, 前缀）
     */
    @TableField("signature")
    private String signature;

    /**
     * 开票点编码
     */
    @TableField("kpd_code")
    private String kpdCode;

    /**
     * POS 机终端编号
     */
    @TableField("pos_no")
    private String posNo;


}
