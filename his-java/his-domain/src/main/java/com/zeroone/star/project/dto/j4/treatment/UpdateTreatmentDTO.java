package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class UpdateTreatmentDTO implements Serializable {

    @Schema(description = "主键ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编码（全局唯一）", example = "TRT001")
    private String busNo;

    @Schema(description = "项目名称", example = "血常规检查")
    private String name;

    @Schema(description = "拼音检索码", example = "XCGJC")
    private String pyStr;

    @Schema(description = "五笔检索码", example = "TLTG")
    private String wbStr;

    @Schema(description = "销售/计价单位编码", example = "UNIT001")
    private String packageUnitCode;

    @Schema(description = "诊疗类型枚举编码", example = "CHECKUP")
    private String typeEnum;

    @Schema(description = "发布状态枚举编码", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "说明文本", example = "常规血液检查")
    private String descriptionText;

    @Schema(description = "身体部位编码", example = "BODY001")
    private String bodySiteCode;

    @Schema(description = "标本编码", example = "SPEC001")
    private String specimenCode;

    @Schema(description = "所属机构标识", example = "ORG001")
    private String orgId;

    @Schema(description = "位置标识", example = "LOC001")
    private String locationId;

    @Schema(description = "计费规则标识", example = "1")
    private Integer ruleId;

    @Schema(description = "划价标记", example = "1")
    private Integer pricingFlag;

    @Schema(description = "医保标记", example = "1")
    private Integer ybFlag;

    @Schema(description = "医保编码", example = "YB001")
    private String ybNo;

    @Schema(description = "医保对码标记", example = "1")
    private Integer ybMatchFlag;

    @Schema(description = "医保收费等级", example = "1")
    private Integer chrgitmLv;

    @Schema(description = "子项标记", example = "0")
    private Integer childrenFlag;

    @Schema(description = "诊疗项目业务版本", example = "V1.0")
    private String activityVersion;

    @Schema(description = "商品名称", example = "血常规")
    private String merchandiseName;

    @Schema(description = "商品名拼音检索码", example = "XCG")
    private String merchandisePyStr;

    @Schema(description = "商品名五笔检索码", example = "TLT")
    private String merchandiseWbStr;

    @Schema(description = "默认门诊/就诊单位编码", example = "UNIT001")
    private String defEncounterUnitCode;

    @Schema(description = "默认住院单位编码", example = "UNIT002")
    private String defInhospitalUnitCode;

    @Schema(description = "乐观锁版本号", example = "0")
    private Integer version;
}