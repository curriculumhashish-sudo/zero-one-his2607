package com.zeroone.star.project.vo.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;

import java.math.BigDecimal;

public class MedicineDetailVO {
    /**
     * 药品名称
     */
    @Schema(description = "name",example = "当归",defaultValue = "当归")
    @NotBlank(message = "通用名称不能为空")
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
    @Schema(description = "药品分类",example = "中药",defaultValue = "中药")
    @NotBlank(message = "药品分类不能为空")
    private String categoryCode;

    /**
     * 商品名称
     */
    @Schema(description = "商品名称",example = "当归",defaultValue = "当归")
    @NotBlank(message = "商品名称不能为空")
    private String merchandiseName;
    /**
     * 财务类型 (对应图片“财务类型”，如：西药、中成药、草药)
     * 注：代码已有 categoryCode，若需区分财务核算类别需新增
     */
    private String financeTypeCode;

    /**
     * 医保类别 (对应图片“医保类别”下拉框)
     */
    private String insuranceCategoryCode;

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
    @Schema(description = "生产厂家",example = "腾讯",defaultValue = "腾讯")
    @NotBlank(message = "生产厂家不能为空")
    private String manufacturerText;

    /**
     * 供应商标识
     */
    @Schema(description = "供应商",example = "1",defaultValue = "1")
    @NotBlank(message = "供应商不能为空")
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
    @Schema(description = "规格",example = "g",defaultValue = "g")
    @NotBlank(message = "规格不能为空")
    private String totalVolume;

    /**
     * 剂型编码
     */
    @Schema(description = "剂型",example = "散剂",defaultValue = "散剂")
    @NotBlank(message = "剂型不能为空")
    private String doseFormCode;

    /**
     * 剂量单位（5类单位之一）
     */
    @Schema(description = "计量单位",example = "g",defaultValue = "g")
    @NotBlank(message = "计量单位不能为空")
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
    private String medicationDefinition;

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
    /**
     * 购入价（包装单位）
     */
    @Schema(description = "购入价",example = "0.12",defaultValue = "0.12")
    @NotNull(message = "购入价不能为空")
    private BigDecimal purchasePrice;

    /**
     * 零售价（包装单位）
     */
    @Schema(description = "零售价",example = "0.12",defaultValue = "0.12")
    @NotNull(message = "零售价不能为空")
    private BigDecimal retailPrice;

    /**
     * 最高零售价
     */
    private BigDecimal maxRetailPrice;

    /**
     * 采购/拆零倍数 (例如：1袋=1000g)
     */
    @Schema(description = "采购倍数",example = "1000",defaultValue = "1000")
    @NotNull(message = "采购倍数不能为空")
    private Integer purchaseSplitRatio;

    /**
     * 库存/拆零倍数
     */
    @Schema(description = "库存倍数",example = "1",defaultValue = "1")
    @NotNull(message = "库存倍数不能为空")
    private Integer stockSplitRatio;

    /**
     * 包装/拆零倍数
     */
    @Schema(description = "包装倍数",example = "1",defaultValue = "1")
    @NotNull(message = "包装倍数不能为空")
    private Integer packageSplitRatio;
}
