package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
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
    @Schema(description = "诊断定义主键（编辑时必填）", example = "317932056501817344")
    private String id;

    /**
     * 来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)
     */
    @Schema(description = "来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)",
            example = "general_dx", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceEnum;

    /**
     * 病情编码，格式 CDF + 数字序号，后端自动生成，前端可不传
     */
    @Schema(description = "病情编码（后端自动生成）", example = "CDF00000005")
    private String conditionCode;

    /**
     * 诊断名称
     */
    @Schema(description = "诊断名称", example = "高血压", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 拼音检索码，后端根据 name 自动生成
     */
    @Schema(description = "拼音码（后端自动生成）", example = "GXY")
    private String pyStr;

    /**
     * 五笔检索码，后端根据 name 自动生成
     */
    @Schema(description = "五笔码（后端自动生成）", example = "YTD")
    private String wbStr;

    /**
     * 诊断类型编码 → 关联字典 diag_type(309)
     */
    @Schema(description = "诊断类型编码", example = "1")
    private String typeCode;

    /**
     * 疾病编码（ICD 映射），可选
     */
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
    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    /**
     * 描述说明
     */
    @Schema(description = "描述说明", example = "慢性血压升高性疾病")
    private String description;
}
