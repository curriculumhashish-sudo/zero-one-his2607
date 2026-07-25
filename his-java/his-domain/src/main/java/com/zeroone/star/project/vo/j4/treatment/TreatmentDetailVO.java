package com.zeroone.star.project.vo.j4.treatment;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.ser.std.ToStringSerializer;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.List;

@Data
@Schema(description = "诊疗项目详情显示对象")
public class TreatmentDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @JsonSerialize(using = ToStringSerializer.class)
    @Schema(description = "主键ID", example = "1890000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "分类编码", example = "22", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String categoryCode;

    @Schema(description = "业务编号", example = "AD00000001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String busNo;

    @Schema(description = "项目名称", example = "肝功13项", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String name;

    @Schema(description = "拼音检索码", example = "gg13x", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pyStr;

    @Schema(description = "五笔检索码", example = "af13c", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String wbStr;

    @Schema(description = "销售/计价单位编码", example = "次", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String packageUnitCode;

    @Schema(description = "诊疗业务版本", example = "V1.0", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String activityVersion;

    @Schema(description = "商品名称", example = "肝功13项套餐", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandiseName;

    @Schema(description = "商品名拼音检索码", example = "gg13xtc", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandisePyStr;

    @Schema(description = "商品名五笔检索码", example = "af13wdc", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandiseWbStr;

    @Schema(description = "默认门诊单位编码", example = "次", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defEncounterUnitCode;

    @Schema(description = "默认住院单位编码", example = "次", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defInhospitalUnitCode;

    @Schema(description = "医保标记 0-否 1-是", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ybFlag;

    @Schema(description = "划价标记 0-否 1-是", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer pricingFlag;

    @Schema(description = "医保编码", example = "C010101001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybNo;

    @Schema(description = "医保对码标记 0-未对码 1-已对码", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ybMatchFlag;

    @Schema(description = "状态枚举值（ACTIVE-启用/INACTIVE-停用）", example = "ACTIVE", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String statusEnum;

    @Schema(description = "身体部位编码", example = "ARM_VEIN", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String bodySiteCode;

    @Schema(description = "标本编码", example = "BLOOD", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String specimenCode;

    @Schema(description = "说明文本", example = "常规肝功能检测项目", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String descriptionText;

    @Schema(description = "计费规则ID", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ruleId;

    @Schema(description = "计费规则展示名", example = "默认计费规则", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ruleId_dictText;

    @Schema(description = "删除标志 0-未删除 1-已删除", example = "0", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer deleteFlag;

    @Schema(description = "类型枚举值（0-非药品收费/1-检验/2-检查/3-影像/4-治疗/5-手术/6-护理/7-膳食/8-其他）", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String typeEnum;

    @Schema(description = "所属机构ID", example = "1001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String orgId;

    @Schema(description = "所在位置ID", example = "2001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String locationId;

    @Schema(description = "收费项目医保等级（1-甲类/2-乙类/3-丙类）", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer chrgitmLv;

    @Schema(description = "允许的角色枚举值", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer permittedRoleEnum;

    @Schema(description = "医保类别编码", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybType;

    @Schema(description = "医保类别展示名", example = "甲类", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybType_dictText;

    @Schema(description = "财务类别编码", example = "1001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String itemTypeCode;

    @Schema(description = "项目购入价（元）", example = "44.40", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private BigDecimal purchasePrice;

    @Schema(description = "项目零售价（元）", example = "44.40", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private BigDecimal retailPrice;

    @Schema(description = "项目最高零售价（元）", example = "44.40", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private BigDecimal maximumRetailPrice;

    @Schema(description = "诊疗子项列表", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private List<WkfActivityDefinitionChildVO> children;
}
