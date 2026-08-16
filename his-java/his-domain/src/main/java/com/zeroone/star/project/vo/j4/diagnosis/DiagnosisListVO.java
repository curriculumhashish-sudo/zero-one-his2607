package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 诊断管理列表显示对象。
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊断管理列表显示对象")
public class DiagnosisListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "诊断ID", example = "327720343745597440")
    private String id;

    @Schema(description = "编码", example = "CDF00000004")
    private String conditionCode;

    @Schema(description = "诊断名称", example = "高血压")
    private String name;

    @Schema(description = "医保编码", example = "YB-I10")
    private String ybNo;

    @Schema(description = "状态编码", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "说明", example = "原发性高血压")
    private String description;
}
