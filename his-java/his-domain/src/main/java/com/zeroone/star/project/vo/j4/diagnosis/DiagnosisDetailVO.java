package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 诊断详情显示对象，用于编辑窗口数据回显。
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊断详情显示对象")
public class DiagnosisDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "诊断ID", example = "327720343745597440")
    private String id;

    @Schema(description = "诊断目录编码", example = "general_dx")
    private String sourceEnum;

    @Schema(description = "诊断目录", example = "疾病与诊断")
    private String sourceEnum_enumText;

    @Schema(description = "诊断名称", example = "原发性高血压")
    private String name;

    @Schema(description = "特慢病编码", example = "TB-001")
    private String diseaseCode;

    @Schema(description = "诊断编码", example = "CDF00000003")
    private String conditionCode;

    @Schema(description = "诊断类型编码", example = "1")
    private String typeCode;

    @Schema(description = "诊断类型", example = "西医诊断")
    private String typeCode_dictText;

    @Schema(description = "医保编码", example = "YB-I10")
    private String ybNo;

    @Schema(description = "状态", example = "已启用")
    private String statusEnum_enumText;

    @Schema(description = "医保标记，1表示是，0表示否", example = "1")
    private Integer ybFlag;

    @Schema(description = "医保对码，1表示已对码，0表示未对码", example = "1")
    private Integer ybMatchFlag;

    @Schema(description = "拼音助记", example = "YFXGXY")
    private String pyStr;

    @Schema(description = "五笔助记", example = "YTD")
    private String wbStr;

    @Schema(description = "说明", example = "高血压病")
    private String description;

    @Schema(description = "状态编码", example = "ACTIVE")
    private String statusEnum;
}
