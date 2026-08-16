package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 作废日程 请求DTO
 */
@Data
@Schema(description = "作废日程入参")
public class RetireScheduleDTO {
    @NotBlank(message = "排班ID不能为空")
    @Schema(description = "排班主键ID", example = "335684971163291648")
    private String scheduleId;

    @NotBlank(message = "作废原因不能为空")
    @Schema(description = "作废原因", example = "作废日程")
    private String reason;


}
