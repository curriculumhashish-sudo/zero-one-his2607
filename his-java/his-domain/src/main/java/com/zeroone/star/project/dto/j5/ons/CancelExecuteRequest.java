package com.zeroone.star.project.dto.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "取消执行请求")
public class CancelExecuteRequest {

    @NotBlank(message = "执行记录ID不能为空")
    @Schema(description = "执行记录ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private String record_id;

    @NotBlank(message = "取消原因不能为空")
    @Schema(description = "取消原因", requiredMode = Schema.RequiredMode.REQUIRED, example = "患者拒绝")
    private String cancel_reason;
}