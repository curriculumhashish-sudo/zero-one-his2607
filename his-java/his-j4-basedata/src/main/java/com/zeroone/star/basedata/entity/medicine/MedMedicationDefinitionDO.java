package com.zeroone.star.basedata.entity.medicine;

import com.baomidou.mybatisplus.annotation.*;
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
 * @author xxm
 * @since 2026-07-18
 */
@Getter
@Setter
@ToString
@TableName("med_medication_definition")
public class MedMedicationDefinitionDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     */
    @TableField("tenant_id")
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(value = "created_at",fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(value = "updated_at",fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
    @TableField("is_deleted")
    private Boolean isDeleted;

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
    @Version
    @TableField("version")
    private Integer version;

    /**
     * 药品定义聚合根
     */
    @TableField("bus_no")
    private String busNo;

    /**
     * 药品名称
     */
    @TableField("name")
    private String name;

    /**
     * 适用范围编码
     */
    @TableField("domain_enum")
    private String domainEnum;

    /**
     * 药品版本
     */
    @TableField("med_version")
    private String medVersion;

    /**
     * 英文药名
     */
    @TableField("name_en")
    private String nameEn;

    /**
     * 药品名称拼音码
     */
    @TableField("py_str")
    private String pyStr;

    /**
     * 药品名称五笔码
     */
    @TableField("wb_str")
    private String wbStr;

    /**
     * 药品分类编码
     */
    @TableField("category_code")
    private String categoryCode;

    /**
     * 商品名称
     */
    @TableField("merchandise_name")
    private String merchandiseName;

    /**
     * 商品名拼音码
     */
    @TableField("merchandise_py_str")
    private String merchandisePyStr;

    /**
     * 商品名五笔码
     */
    @TableField("merchandise_wb_str")
    private String merchandiseWbStr;

    /**
     * 采购单位（5类单位之一）
     */
    @TableField("purchase_unit_code")
    private String purchaseUnitCode;

    /**
     * 拆零单位（5类单位之一）
     */
    @TableField("split_unit_code")
    private String splitUnitCode;

    /**
     * 所含耗材描述
     */
    @TableField("comprised_text")
    private String comprisedText;

    /**
     * 成分
     */
    @TableField("ingredient")
    private String ingredient;

    /**
     * 剂量形式编码
     */
    @TableField("dose_from")
    private Integer doseFrom;

    /**
     * 批准文号
     */
    @TableField("approval_number")
    private String approvalNumber;

    /**
     * 医保是否已对码
     */
    @TableField("yb_match_flag")
    private Boolean ybMatchFlag;

    /**
     * 医保编码
     */
    @TableField("yb_no")
    private String ybNo;

    /**
     * 药理作用分类编码
     */
    @TableField("pharmacology_category_code")
    private String pharmacologyCategoryCode;

    /**
     * 是否皮试药
     */
    @TableField("skin_test_flag")
    private Boolean skinTestFlag;

    /**
     * 是否注射药物
     */
    @TableField("inject_flag")
    private Boolean injectFlag;

    /**
     * 生产厂家标识
     */
    @TableField("manufacturer_id")
    private String manufacturerId;

    /**
     * 生产厂商文本
     */
    @TableField("manufacturer_text")
    private String manufacturerText;

    /**
     * 供应商标识
     */
    @TableField("supply_id")
    private String supplyId;

    /**
     * 是否限制使用
     */
    @TableField("restricted_flag")
    private Boolean restrictedFlag;

    /**
     * 限制使用范围
     */
    @TableField("restricted_scope")
    private String restrictedScope;

    /**
     * 启用标记（true=启用，false=停用）
     */
    @TableField("active_flag")
    private Boolean activeFlag;

    /**
     * 儿童用药标志
     */
    @TableField("children_flag")
    private Boolean childrenFlag;

    /**
     * 产品特性编码
     */
    @TableField("characteristic")
    private Integer characteristic;

    /**
     * 最小库存警戒数量
     */
    @TableField("item_min_quantity")
    private BigDecimal itemMinQuantity;

    /**
     * 最大库存警戒数量
     */
    @TableField("item_max_quantity")
    private BigDecimal itemMaxQuantity;

    /**
     * 默认门诊单位编码
     */
    @TableField("def_encounter_unit_code")
    private String defEncounterUnitCode;

    /**
     * 默认住院单位编码
     */
    @TableField("def_inhospital_unit_code")
    private String defInhospitalUnitCode;

    /**
     * 国家药品贯标编码
     */
    @TableField("national_drug_code")
    private String nationalDrugCode;

    /**
     * 拆分属性编码
     */
    @TableField("part_attribute_enum")
    private String partAttributeEnum;

    /**
     * 抗生素分类编码
     */
    @TableField("antibiotic_code")
    private String antibioticCode;

    /**
     * 权限限制枚举编码
     */
    @TableField("restricted_enum")
    private String restrictedEnum;

    /**
     * 是否自制
     */
    @TableField("self_flag")
    private Boolean selfFlag;

    /**
     * 是否抗生素
     */
    @TableField("antibiotic_flag")
    private Boolean antibioticFlag;

    /**
     * 是否基药
     */
    @TableField("basic_flag")
    private Boolean basicFlag;

    /**
     * 住院临时医嘱拆分属性编码
     */
    @TableField("tho_part_attribute_enum")
    private String thoPartAttributeEnum;

    /**
     * 医保等级
     */
    @TableField("chrgitm_lv")
    private Integer chrgitmLv;

    /**
     * 处方药标志
     */
    @TableField("rx_flag")
    private Boolean rxFlag;

    /**
     * 用药说明
     */
    @TableField("dosage_instruction")
    private String dosageInstruction;

    /**
     * 药品 69 码
     */
    @TableField("drug69_code")
    private String drug69Code;

    /**
     * 规格
     */
    @TableField("total_volume")
    private String totalVolume;

    /**
     * 剂型编码
     */
    @TableField("dose_form_code")
    private String doseFormCode;

    /**
     * 剂量单位（5类单位之一）
     */
    @TableField("dose_unit_code")
    private String doseUnitCode;

    /**
     * 销售单位（5类单位之一）
     */
    @TableField("package_unit_code")
    private String packageUnitCode;

    /**
     * 库存单位（5类单位之一）
     */
    @TableField("stock_unit_code")
    private String stockUnitCode;

    /**
     * 默认用法编码
     */
    @TableField("method_code")
    private String methodCode;

    /**
     * 默认频次编码
     */
    @TableField("rate_code")
    private String rateCode;

    /**
     * 单次剂量
     */
    @TableField("dose")
    private BigDecimal dose;

    /**
     * 单次最大剂量
     */
    @TableField("max_unit")
    private BigDecimal maxUnit;

    /**
     * 用量限定
     */
    @TableField("usage_limit")
    private BigDecimal usageLimit;

    /**
     * 采购入库位置（他域事实列）
     */
    @TableField("location_id")
    private String locationId;

    /**
     * 生命周期状态编码（DRAFT/ACTIVE/INACTIVE/DISCARDED）
     */
    @TableField("status_enum")
    private String statusEnum;

    /**
     * 所属科室标识
     */
    @TableField("org_id")
    private String orgId;

    /**
     * 成分项
     */
    @TableField("ingredient_item")
    private String ingredientItem;

    /**
     * 药品说明
     */
    @TableField("medicationdefinition")
    private String medicationdefinition;

    /**
     * DDD（日限定剂量）编码
     */
    @TableField("ddd_code")
    private String dddCode;

    /**
     * DDD（日限定剂量）单位编码
     */
    @TableField("ddd_unit_code")
    private String dddUnitCode;

    /**
     * 最小频次编码
     */
    @TableField("min_rate_code")
    private String minRateCode;

    /**
     * 最大频次编码
     */
    @TableField("max_rate_code")
    private String maxRateCode;

    /**
     * 剂量单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("dose_unit_ratio")
    private BigDecimal doseUnitRatio;

    /**
     * 采购单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("purchase_unit_ratio")
    private BigDecimal purchaseUnitRatio;

    /**
     * 库存单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("stock_unit_ratio")
    private BigDecimal stockUnitRatio;

    /**
     * 销售单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("package_unit_ratio")
    private BigDecimal packageUnitRatio;
}
