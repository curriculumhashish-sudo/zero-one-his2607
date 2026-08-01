package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 新增诊疗项目DTO
 * <p>字段对齐表 {@code wkf_activity_definition}（详见 zo_his.sql）。</p>
 */
@Data
@Schema(description = "新增诊疗项目DTO")
public class TreatmentAddDTO {

    @NotBlank(message = "诊疗项目名称不能为空")
    @Size(max = 255, message = "诊疗项目名称不能超过255个字符")
    @Schema(description = "诊疗项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Size(max = 255, message = "拼音检索码不能超过255个字符")
    @Schema(description = "拼音检索码", example = "XGCJC")
    private String pyStr;

    @Size(max = 255, message = "五笔检索码不能超过255个字符")
    @Schema(description = "五笔检索码", example = "XGI")
    private String wbStr;

    @Size(max = 255, message = "销售/计价单位编码不能超过255个字符")
    @Schema(description = "销售/计价单位编码", example = "UNIT_TIMES")
    private String packageUnitCode;

    @NotBlank(message = "诊疗类型不能为空")
    @Size(max = 255, message = "诊疗类型枚举编码不能超过255个字符")
    @Schema(description = "诊疗类型枚举编码", example = "EXAM", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @NotBlank(message = "发布状态不能为空")
    @Size(max = 255, message = "发布状态枚举编码不能超过255个字符")
    @Schema(description = "发布状态枚举编码：DRAFT发布中/ACTIVE启用/INACTIVE停用", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @NotBlank(message = "分类编码不能为空")
    @Size(max = 255, message = "分类编码不能超过255个字符")
    @Schema(description = "分类编码（诊疗项目定义聚合根）", example = "检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    @NotBlank(message = "业务编号不能为空")
    @Size(max = 255, message = "业务编号不能超过255个字符")
    @Schema(description = "业务编号（全局唯一）", example = "CG001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Size(max = 255, message = "说明文本不能超过255个字符")
    @Schema(description = "说明文本", example = "常规检查项目")
    private String descriptionText;

    @Size(max = 255, message = "身体部位编码不能超过255个字符")
    @Schema(description = "身体部位编码", example = "BODY_BLOOD")
    private String bodySiteCode;

    @Size(max = 255, message = "标本编码不能超过255个字符")
    @Schema(description = "标本编码", example = "SPEC_BLOOD")
    private String specimenCode;

    @Schema(description = "所属机构标识", example = "1001")
    private String orgId;

    @Schema(description = "位置标识", example = "2001")
    private String locationId;

    @Schema(description = "计费规则标识", example = "1")
    private Integer ruleId;

    @Schema(description = "划价标记", example = "1")
    private Integer pricingFlag;

    @Schema(description = "医保标记", example = "1")
    private Integer ybFlag;

    @Size(max = 255, message = "医保编码不能超过255个字符")
    @Schema(description = "医保编码", example = "C010101001")
    private String ybNo;

    @Schema(description = "医保对码标记", example = "0")
    private Integer ybMatchFlag;

    @Schema(description = "医保收费等级", example = "1")
    private Integer chrgitmLv;

    @Schema(description = "子项标记（需与子项子表事实一致）", example = "0")
    private Integer childrenFlag;

    @Size(max = 255, message = "诊疗项目业务版本不能超过255个字符")
    @Schema(description = "诊疗项目业务版本（非乐观锁 version）", example = "v1.0")
    private String activityVersion;

    @Size(max = 255, message = "商品名称不能超过255个字符")
    @Schema(description = "商品名称（可与项目名称分列维护）", example = "血常规(商品名)")
    private String merchandiseName;

    @Size(max = 255, message = "商品名拼音检索码不能超过255个字符")
    @Schema(description = "商品名拼音检索码", example = "XGCSP")
    private String merchandisePyStr;

    @Size(max = 255, message = "商品名五笔检索码不能超过255个字符")
    @Schema(description = "商品名五笔检索码", example = "XGSP")
    private String merchandiseWbStr;

    @Size(max = 255, message = "默认门诊/急诊单位编码不能超过255个字符")
    @Schema(description = "默认门诊/急诊单位编码", example = "ORG_OUTPATIENT")
    private String defEncounterUnitCode;

    @Size(max = 255, message = "默认住院单位编码不能超过255个字符")
    @Schema(description = "默认住院单位编码", example = "ORG_INPATIENT")
    private String defInhospitalUnitCode;

    @Size(max = 255, message = "专业科室编码不能超过255个字符")
    @Schema(description = "专业科室编码", example = "CARDIOLOGY")
    private String specialtyCode;

    @Schema(description = "是否有效标记", example = "true")
    private Boolean activeFlag;

    @Schema(description = "是否需要预约标记", example = "false")
    private Boolean appointmentRequiredFlag;

    @Size(max = 500, message = "备注说明不能超过500个字符")
    @Schema(description = "备注说明", example = "常规检查项目")
    private String comment;

    @Size(max = 2000, message = "扩展详情不能超过2000个字符")
    @Schema(description = "扩展详情", example = "{\"detail\":\"血常规全套\"}")
    private String extraDetails;

    @Size(max = 500, message = "联系方式不能超过500个字符")
    @Schema(description = "联系方式", example = "{\"phone\":\"12345678\"}")
    private String contact;
}
