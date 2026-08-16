package com.zeroone.star.project.dto.j7.pharmacy.approval;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

@Schema(description = "操作员DTO")
@Data
public class OperatorDTO {

    @Schema(description = "业务编号", example = "PST2026-06-300001")
    @NotBlank(message = "busNo不能为空")
    @Size(max = 255, message = "busNo长度不能超过255个字符")
    private String busNo;

    @Schema(description = "申请人编号", example = "308791351298756608")
    @NotBlank(message = "operatorId不能为空")
    @Size(max = 64, message = "operatorId长度不能超过64个字符")
    private String operatorId;
}
