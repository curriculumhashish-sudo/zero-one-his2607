package com.zeroone.star.project.dto.j5.bindingMapping;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * <p>
 * 描述：诊疗绑定数据传输对象
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@Schema(description = "诊疗绑定数据传输对象")
public class TreatmentBindingDTO {

    @Schema(description = "主键ID（编辑时必传）")
    private String id;

    @NotNull(message = "诊疗项目ID不能为空")
    @Schema(description = "诊疗项目ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long treatmentId;

    @NotNull(message = "绑定目标类型不能为空")
    @Schema(description = "绑定目标类型：1=诊疗，2=耗材", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer targetType;

    @NotNull(message = "绑定目标名称不能为空")
    @Schema(description = "绑定目标名称", requiredMode = Schema.RequiredMode.REQUIRED)
    private String targetName;

    @Schema(description = "数量", example = "1")
    private Integer quantity;

    @Schema(description = "单位")
    private String unit;

    @Schema(description = "状态：1启用，0停用", example = "1")
    private Integer status;
}
