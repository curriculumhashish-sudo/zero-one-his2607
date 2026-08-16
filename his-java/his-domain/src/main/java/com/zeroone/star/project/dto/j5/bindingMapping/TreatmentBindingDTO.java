package com.zeroone.star.project.dto.j5.bindingMapping;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：诊疗绑定数据传输对象（映射cus_service_device_template表）
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

    @NotBlank(message = "范围编码不能为空")
    @Schema(description = "范围编码", requiredMode = Schema.RequiredMode.REQUIRED)
    private String rangeCode;

    @NotBlank(message = "类型编码不能为空")
    @Schema(description = "类型编码", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeCode;

    @NotBlank(message = "项目表不能为空")
    @Schema(description = "项目表（如：healthcare_service）", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemTable;

    @NotBlank(message = "项目ID不能为空")
    @Schema(description = "项目ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemId;

    @NotBlank(message = "设备行为表不能为空")
    @Schema(description = "设备行为表（如：device_definition）", requiredMode = Schema.RequiredMode.REQUIRED)
    private String devActTable;

    @NotBlank(message = "设备行为ID不能为空")
    @Schema(description = "设备行为ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String devActId;

    @NotNull(message = "数量不能为空")
    @Schema(description = "数量", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal quantity;

    @NotBlank(message = "单位编码不能为空")
    @Schema(description = "单位编码", requiredMode = Schema.RequiredMode.REQUIRED)
    private String unitCode;

    @Schema(description = "状态枚举")
    private String statusEnum;
}
