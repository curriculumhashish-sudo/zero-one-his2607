package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;

@Schema(description = "操作员DTO")
public class OperatorDTO {

    @Schema(description = "业务编号")
    @NotBlank(message = "busNo不能为空")
    private String busNo;

    @Schema(description = "申请人编号")
    @NotBlank(message = "operatorId不能为空")
    private String operatorId;
}
