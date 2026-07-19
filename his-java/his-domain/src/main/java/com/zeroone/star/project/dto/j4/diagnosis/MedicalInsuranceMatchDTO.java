package com.zeroone.star.project.dto.j4.diagnosis;

import com.fasterxml.jackson.annotation.JsonIgnore;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.AssertTrue;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;
import org.springframework.util.StringUtils;

import java.io.Serializable;

/**
 * 诊断医保对照请求数据。
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊断医保对照请求数据")
public class MedicalInsuranceMatchDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @NotBlank(message = "诊断ID不能为空")
    @Schema(description = "诊断ID", example = "101",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotNull(message = "是否医保诊断不能为空")
    @Schema(description = "是否医保诊断", example = "true",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean ybFlag;

    @Schema(description = "医保目录编码", example = "A00012345")
    private String ybNo;

    @NotNull(message = "医保对照标记不能为空")
    @Schema(description = "是否完成医保对照", example = "true",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean ybMatchFlag;

    @Schema(description = "医保诊断类型", example = "OUTPATIENT")
    private String typeCode;

    @Schema(description = "疾病编码", example = "TB-001")
    private String diseaseCode;

    @Schema(description = "数据版本号", example = "3")
    private Long version;

    @JsonIgnore
    @AssertTrue(message = "已完成医保对照时，必须是医保诊断且医保目录编码不能为空")
    public boolean isMedicalInsuranceMatchValid() {
        return !Boolean.TRUE.equals(ybMatchFlag)
                || (Boolean.TRUE.equals(ybFlag) && StringUtils.hasText(ybNo));
    }
}
