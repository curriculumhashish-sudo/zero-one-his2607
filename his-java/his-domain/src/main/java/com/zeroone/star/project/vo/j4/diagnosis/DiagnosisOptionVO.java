package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 选择诊断目录列表显示对象。
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "选择诊断目录列表显示对象")
public class DiagnosisOptionVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "诊断目录编码", example = "general_dx")
    private String code;

    @Schema(description = "诊断目录名称", example = "疾病与诊断")
    private String display;
}
