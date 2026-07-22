package com.zeroone.star.project.query.j7.refundmedicine;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：住院退药-患者退药明细查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j7
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@Schema(description = "住院退药-患者退药明细查询对象")
public class InpatientRefundDetailQuery {

    /**
     * 患者ID（对应 med_medication_request.patient_id）
     */
    @NotBlank(message = "患者ID不能为空")
    @Schema(description = "患者ID", example = "326940849401761792", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    /**
     * 就诊ID（对应 med_medication_request.encounter_id）
     */
    @NotBlank(message = "就诊ID不能为空")
    @Schema(description = "就诊ID", example = "329874660147597312", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterId;

    /**
     * 医嘱类型（对应 med_medication_request.therapy_enum / category_enum）
     */
    @Schema(description = "医嘱类型", example = "TEMPORARY")
    private String therapyEnum;

    /**
     * 药品分类（对应 med_medication_definition.category_code）
     */
    @Schema(description = "药品分类编码", example = "western")
    private String categoryCode;
}
