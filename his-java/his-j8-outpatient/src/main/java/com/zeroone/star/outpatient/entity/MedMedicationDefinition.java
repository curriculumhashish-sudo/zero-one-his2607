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
 * 药品 medication 定义
 * </p>
 *
 * @author Zzen
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("med_medication_definition")
public class MedMedicationDefinition implements Serializable {

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
     * 药品定义聚合根
     */
    private String busNo;

    /**
     * 药品名称
     */
    private String name;

    /**
     * 适用范围编码
     */
    private String domainEnum;

    /**
     * 药品版本
     */
    private String medVersion;

    /**
     * 英文药名
     */
    private String nameEn;

    /**
     * 药品名称拼音码
     */
    private String pyStr;

    /**
     * 药品名称五笔码
     */
    private String wbStr;

    /**
     * 药品分类编码
     */
    private String categoryCode;

    /**
     * 商品名称
     */
    private String merchandiseName;

    /**
     * 商品名拼音码
     */
    private String merchandisePyStr;

    /**
     * 商品名五笔码
     */
    private String merchandiseWbStr;

    /**
     * 采购单位（5类单位之一）
     */
    private String purchaseUnitCode;

    /**
     * 拆零单位（5类单位之一）
     */
    private String splitUnitCode;

    /**
     * 所含耗材描述
     */
    private String comprisedText;

    /**
     * 成分
     */
    private String ingredient;

    /**
     * 剂量形式编码
     */
    private Integer doseFrom;

    /**
     * 批准文号
     */
    private String approvalNumber;

    /**
     * 医保是否已对码
     */
    private Boolean ybMatchFlag;

    /**
     * 医保编码
     */
    private String ybNo;

    /**
     * 药理作用分类编码
     */
    private String pharmacologyCategoryCode;

    /**
     * 是否皮试药
     */
    private Boolean skinTestFlag;

    /**
     * 是否注射药物
     */
    private Boolean injectFlag;

    /**
     * 生产厂家标识
     */
    private String manufacturerId;

    /**
     * 生产厂商文本
     */
    private String manufacturerText;

    /**
     * 供应商标识
     */
    private String supplyId;

    /**
     * 是否限制使用
     */
    private Boolean restrictedFlag;

    /**
     * 限制使用范围
     */
    private String restrictedScope;

    /**
     * 启用标记（true=启用，false=停用）
     */
    private Boolean activeFlag;

    /**
     * 儿童用药标志
     */
    private Boolean childrenFlag;

    /**
     * 产品特性编码
     */
    private Integer characteristic;

    /**
     * 最小库存警戒数量
     */
    private BigDecimal itemMinQuantity;

    /**
     * 最大库存警戒数量
     */
    private BigDecimal itemMaxQuantity;

    /**
     * 默认门诊单位编码
     */
    private String defEncounterUnitCode;

    /**
     * 默认住院单位编码
     */
    private String defInhospitalUnitCode;

    /**
     * 国家药品贯标编码
     */
    private String nationalDrugCode;

    /**
     * 拆分属性编码
     */
    private String partAttributeEnum;

    /**
     * 抗生素分类编码
     */
    private String antibioticCode;

    /**
     * 权限限制枚举编码
     */
    private String restrictedEnum;

    /**
     * 是否自制
     */
    private Boolean selfFlag;

    /**
     * 是否抗生素
     */
    private Boolean antibioticFlag;

    /**
     * 是否基药
     */
    private Boolean basicFlag;

    /**
     * 住院临时医嘱拆分属性编码
     */
    private String thoPartAttributeEnum;

    /**
     * 医保等级
     */
    private Integer chrgitmLv;

    /**
     * 处方药标志
     */
    private Boolean rxFlag;

    /**
     * 用药说明
     */
    private String dosageInstruction;

    /**
     * 药品 69 码
     */
    private String drug69Code;

    /**
     * 规格
     */
    private String totalVolume;

    /**
     * 剂型编码
     */
    private String doseFormCode;

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
     * 默认用法编码
     */
    private String methodCode;

    /**
     * 默认频次编码
     */
    private String rateCode;

    /**
     * 单次剂量
     */
    private BigDecimal dose;

    /**
     * 单次最大剂量
     */
    private BigDecimal maxUnit;

    /**
     * 用量限定
     */
    private BigDecimal usageLimit;

    /**
     * 采购入库位置（他域事实列）
     */
    private String locationId;

    /**
     * 生命周期状态编码（DRAFT/ACTIVE/INACTIVE/DISCARDED）
     */
    private String statusEnum;

    /**
     * 机构标识
     */
    private String orgId;

    /**
     * 成分项
     */
    private String ingredientItem;

    /**
     * 药品说明
     */
    private String medicationdefinition;

    /**
     * DDD（日限定剂量）编码
     */
    private String dddCode;

    /**
     * DDD（日限定剂量）单位编码
     */
    private String dddUnitCode;

    /**
     * 最小频次编码
     */
    private String minRateCode;

    /**
     * 最大频次编码
     */
    private String maxRateCode;

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
