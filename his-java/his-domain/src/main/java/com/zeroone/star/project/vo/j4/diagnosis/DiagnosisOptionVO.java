package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 可选择诊断列表显示对象。
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "可选择诊断列表显示对象")
public class DiagnosisOptionVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "诊断ID", example = "327720343745597440")
    private String id;

    @Schema(description = "诊断编码", example = "I10")
    private String conditionCode;

    @Schema(description = "诊断名称", example = "原发性高血压")
    private String name;

    @Schema(description = "诊断目录编码", example = "general_dx")
    private String sourceEnum;

    @Schema(description = "拼音助记", example = "YFXGXY")
    private String pyStr;

    @Schema(description = "五笔助记", example = "YTD")
    private String wbStr;

    @Schema(description = "医保标记，1表示是，0表示否", example = "1")
    private Integer ybFlag;

    @Schema(description = "医保编码", example = "YB-I10")
    private String ybNo;
}
