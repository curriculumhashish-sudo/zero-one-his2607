package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 诊断医保对照显示对象。
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊断医保对照显示对象")
public class MedicalInsuranceMatchVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "诊断ID", example = "101")
    private String id;

    @Schema(description = "是否医保诊断", example = "true")
    private Boolean ybFlag;

    @Schema(description = "医保目录编码", example = "A00012345")
    private String ybNo;

    @Schema(description = "是否完成医保对照", example = "true")
    private Boolean ybMatchFlag;

    @Schema(description = "医保诊断类型", example = "OUTPATIENT")
    private String typeCode;

    @Schema(description = "疾病编码", example = "TB-001")
    private String diseaseCode;

    @Schema(description = "数据版本号", example = "3")
    private Long version;
}
