package com.zeroone.star.outpatient.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 管理域 设备定义
 * </p>
 *
 * @author Zzen
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("adm_device_definition")
public class AdmDeviceDefinition implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
    private Boolean isDeleted;

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
    @Version
    private Integer version;

    /**
     * 器材定义聚合根（DM-ADM-008 §3.1，SDD §4.2）
     */
    private String name;

    /**
     * 器材业务编码（院内唯一）
     */
    private String busNo;

    /**
     * 拼音码（用于检索）
     */
    private String pyStr;

    /**
     * 五笔码（用于检索）
     */
    private String wbStr;

    /**
     * 器材分类编码
     */
    private String categoryCode;

    /**
     * 器材种类编码
     */
    private String typeCode;

    /**
     * 采购单位（5类单位之一）
     */
    private String purchaseUnitCode;

    /**
     * 包装规格描述（与药品 total_volume 语义对齐）
     */
    private String totalVolume;

    /**
     * 最小库存警戒量
     */
    private BigDecimal itemMinQuantity;

    /**
     * 最大库存警戒量
     */
    private BigDecimal itemMaxQuantity;

    /**
     * 拆零单位（5类单位之一）
     */
    private String splitUnitCode;

    /**
     * 商品名称
     */
    private String merchandiseName;

    /**
     * 商品名拼音检索码
     */
    private String merchandisePyStr;

    /**
     * 商品名五笔检索码
     */
    private String merchandiseWbStr;

    /**
     * 默认门诊/就诊单位编码
     */
    private String defEncounterUnitCode;

    /**
     * 默认住院单位编码
     */
    private String defInhospitalUnitCode;

    /**
     * 所属科室标识
     */
    private String orgId;

    /**
     * 所在位置标识
     */
    private String locationId;

    /**
     * 产品型号
     */
    private String modelNumber;

    /**
     * 高值器材标志
     */
    private Boolean hvcmFlag;

    /**
     * 剂量单位（5类单位之一）
     */
    private String doseUnitCode;

    /**
     * 销售单位（5类单位之一）
     */
    private String packageUnitCode;

    /**
     * 库存单位（5类单位之一）
     */
    private String stockUnitCode;

    /**
     * 批准文号
     */
    private String approvalNumber;

    /**
     * 生产厂家标识
     */
    private String manufacturerId;

    /**
     * 生产厂家名称
     */
    private String manufacturerText;

    /**
     * 供应商标识
     */
    private String supplyId;

    /**
     * 器材说明
     */
    private String description;

    /**
     * 适用范围描述
     */
    private String jurisdiction;

    /**
     * 器材版本
     */
    private String deviceVersion;

    /**
     * 主要成分描述
     */
    private String substanceText;

    /**
     * 过敏标记
     */
    private Boolean allergenFlag;

    /**
     * 处方标志
     */
    private Boolean rxFlag;

    /**
     * 医保标记
     */
    private Boolean ybFlag;

    /**
     * 医保编码
     */
    private String ybNo;

    /**
     * 医药机构目录编码
     */
    private String ybOrgNo;

    /**
     * 医保对码标记
     */
    private Boolean ybMatchFlag;

    /**
     * 医保等级
     */
    private Integer chrgitmLv;

    /**
     * 发布状态（存储为整数码）
     */
    private String statusEnum;

    /**
     * 是否启用标记
     */
    private Boolean activeFlag;

    /**
     * 医保对码标识
     */
    private String ybMappingCode;

    /**
     * 剂量单位→拆零单位换算倍数（拆零单位为1）
     */
    private BigDecimal doseUnitRatio;

    /**
     * 采购单位→拆零单位换算倍数（拆零单位为1）
     */
    private BigDecimal purchaseUnitRatio;

    /**
     * 库存单位→拆零单位换算倍数（拆零单位为1）
     */
    private BigDecimal stockUnitRatio;

    /**
     * 销售单位→拆零单位换算倍数（拆零单位为1）
     */
    private BigDecimal packageUnitRatio;
}
