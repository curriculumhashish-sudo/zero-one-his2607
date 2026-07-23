package com.zeroone.star.project.vo.j4.treatment;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.ser.std.ToStringSerializer;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(description = "诊疗项目详情显示对象")
public class TreatmentDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 【类型修改 Long → String】
     * DO主键为String，统一改成String，前端序列化依然用ToStringSerializer保持行为一致
     */
    @Schema(description = "主键ID")
    @JsonSerialize(using = ToStringSerializer.class)
    private String id;

    @Schema(description = "分类编码")
    private String categoryCode;

    /** 业务编号。 */
    @Schema(description = "业务编号")
    private String busNo;

    /** 名称。 */
    @Schema(description = "名称")
    private String name;

    /** 拼音码。 */
    @Schema(description = "拼音码")
    private String pyStr;

    /** 五笔码。 */
    @Schema(description = "五笔码")
    private String wbStr;

    /** 销售/计价单位编码。 */
    @Schema(description = "销售/计价单位编码")
    private String packageUnitCode;

    /** 诊疗业务版本。 */
    @Schema(description = "诊疗业务版本")
    private String activityVersion;

    /** 商品名称。 */
    @Schema(description = "商品名称")
    private String merchandiseName;

    /** 商品名拼音检索码。 */
    @Schema(description = "商品名拼音检索码")
    private String merchandisePyStr;

    /** 商品名五笔检索码。 */
    @Schema(description = "商品名五笔检索码")
    private String merchandiseWbStr;

    /** 默认门诊单位编码。 */
    @Schema(description = "默认门诊单位编码")
    private String defEncounterUnitCode;

    /** 默认住院单位编码。 */
    @Schema(description = "默认住院单位编码")
    private String defInhospitalUnitCode;

    /** 医保标志。 */
    @Schema(description = "医保标志")
    private Integer ybFlag;

    /** 划价标记。 */
    @Schema(description = "划价标记")
    private Integer pricingFlag;

    /** 医保编码。 */
    @Schema(description = "医保编码")
    private String ybNo;

    /** 医保对码标志。 */
    @Schema(description = "医保对码标志")
    private Integer ybMatchFlag;

    /**
     * 【类型修改 Integer → String】
     * DO中typeEnum/statusEnum存储字符串编码
     */
    @Schema(description = "状态枚举值")
    private String statusEnum;

    /** 身体部位编码。 */
    @Schema(description = "身体部位编码")
    private String bodySiteCode;

    /** 标本编码。 */
    @Schema(description = "标本编码")
    private String specimenCode;

    /** 描述信息。 */
    @Schema(description = "描述信息")
    private String descriptionText;

    /**
     * 【类型修改 Long → Integer】
     * DO ruleId 为 Integer
     */
    @Schema(description = "规则ID")
    private Integer ruleId;
    /** 计费规则展示名（字典解析）。 */
    private String ruleId_dictText;

    /**
     * 【两处调整：
     * 1. 字段名 deleteFlag 不变；
     * 2. 类型 String → Integer，和DO.isDeleted类型统一；
     * 业务层后续展示时再自行转字符串，映射不再需要转换
     */
    @Schema(description = "删除标志")
    private Integer deleteFlag;

    /**
     * 【类型修改 Integer → String】
     */
    @Schema(description = "类型枚举值")
    private String typeEnum;

    /**
     * 【类型修改 Long → String】
     */
    @Schema(description = "所属科室ID")
    private String orgId;

    /**
     * 【类型修改 Long → String】
     */
    @Schema(description = "所在位置ID")
    private String locationId;

    /** 收费项目医保等级。 */
    @Schema(description = "收费项目医保等级")
    private Integer chrgitmLv;

    /** 允许的角色枚举值。 */
    @Schema(description = "允许的角色枚举值")
    private Integer permittedRoleEnum;

    /** 医保类别编码（med_chrgitm_type 等术语体系）。 */
    private String ybType;
    /** 医保类别展示名。 */
    private String ybType_dictText;

    /** 财务类别编码。 */
    @Schema(description = "财务类别编码")
    private String itemTypeCode;

    /** 项目购入价。 */
    @Schema(description = "项目购入价")
    private BigDecimal purchasePrice;

    /** 项目零售价。 */
    @Schema(description = "项目零售价")
    private BigDecimal retailPrice;

    /** 项目最高零售价。 */
    @Schema(description = "项目最高零售价")
    private BigDecimal maximumRetailPrice;

    /** 子项。 */
    @Schema(description = "子项")
    private List<WkfActivityDefinitionChildVO> children;
}