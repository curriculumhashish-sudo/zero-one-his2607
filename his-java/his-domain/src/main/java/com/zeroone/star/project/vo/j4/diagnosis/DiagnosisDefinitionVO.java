package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：诊断定义列表项视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "诊断定义列表项视图对象")
@Data
public class DiagnosisDefinitionVO {

    @Schema(description = "诊断定义主键", example = "317932056501817344")
    private String id;

    @Schema(description = "来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)", example = "general_dx")
    private String sourceEnum;

    @Schema(description = "病情编码", example = "CDF00000003")
    private String conditionCode;

    @Schema(description = "诊断名称", example = "高血压")
    private String name;

    @Schema(description = "拼音检索码", example = "GXY")
    private String pyStr;

    @Schema(description = "五笔检索码", example = "YTD")
    private String wbStr;

    @Schema(description = "诊断类型编码", example = "1")
    private String typeCode;

    @Schema(description = "疾病编码（ICD 映射）", example = "1.002")
    private String diseaseCode;

    @Schema(description = "医保标记：0 否 / 1 是", example = "1")
    private Boolean ybFlag;

    @Schema(description = "医保编号", example = "YB00123456")
    private String ybNo;

    @Schema(description = "医保匹配标记：0 未匹配 / 1 已匹配", example = "1")
    private Boolean ybMatchFlag;

    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "描述说明", example = "慢性血压升高性疾病")
    private String description;
}
