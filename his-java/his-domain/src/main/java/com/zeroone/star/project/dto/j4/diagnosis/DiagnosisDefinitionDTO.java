package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：诊断定义增/改传输数据对象，对应 cln_condition_definition 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "诊断定义增改数据对象")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class DiagnosisDefinitionDTO {

    /**
     * 主键（编辑时必填，新增时不填）
     */
    @NotBlank(message = "主键ID不能为空")
    @Size(max = 64, message = "主键ID长度不能超过64个字符")
    @Schema(description = "诊断定义主键（编辑时必填）", example = "317932056501817344")
    private String id;

    /**
     * 来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)
     */
    @NotBlank(message = "来源枚举不能为空")
    @Size(max = 50, message = "来源枚举长度不能超过50个字符")
    @Schema(description = "来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)",
            example = "general_dx", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceEnum;

    /**
     * 病情编码，格式 CDF + 数字序号，后端自动生成，前端可不传
     */
    @Size(max = 50, message = "病情编码长度不能超过50个字符")
    @Schema(description = "病情编码（后端自动生成）", example = "CDF00000005")
    private String conditionCode;

    /**
     * 诊断名称
     */
    @NotBlank(message = "诊断名称不能为空")
    @Size(max = 255, message = "诊断名称长度不能超过255个字符")
    @Schema(description = "诊断名称", example = "高血压", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 拼音检索码，后端根据 name 自动生成
     */
    @Size(max = 100, message = "拼音码长度不能超过100个字符")
    @Schema(description = "拼音码（后端自动生成）", example = "GXY")
    private String pyStr;

    /**
     * 五笔检索码，后端根据 name 自动生成
     */
    @Size(max = 100, message = "五笔码长度不能超过100个字符")
    @Schema(description = "五笔码（后端自动生成）", example = "YTD")
    private String wbStr;

    /**
     * 诊断类型编码 → 关联字典 diag_type(309)
     */
    @Size(max = 50, message = "诊断类型编码长度不能超过50个字符")
    @Schema(description = "诊断类型编码", example = "1")
    private String typeCode;

    /**
     * 疾病编码（ICD 映射），可选
     */
    @Size(max = 100, message = "疾病编码长度不能超过100个字符")
    @Schema(description = "疾病编码（ICD 映射）", example = "1.002")
    private String diseaseCode;

    /**
     * 医保标记：0 否 / 1 是
     */
    @Schema(description = "医保标记：0 否 / 1 是", example = "1")
    private Boolean ybFlag;

    /**
     * 医保编号
     */
    @Size(max = 100, message = "医保编号长度不能超过100个字符")
    @Schema(description = "医保编号", example = "YB00123456")
    private String ybNo;

    /**
     * 医保匹配标记：0 未匹配 / 1 已匹配
     */
    @Schema(description = "医保匹配标记：0 未匹配 / 1 已匹配", example = "1")
    private Boolean ybMatchFlag;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    @NotBlank(message = "状态枚举不能为空")
    @Size(max = 20, message = "状态枚举长度不能超过20个字符")
    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    /**
     * 描述说明
     */
    @Size(max = 500, message = "描述说明长度不能超过500个字符")
    @Schema(description = "描述说明", example = "慢性血压升高性疾病")
    private String description;
}
