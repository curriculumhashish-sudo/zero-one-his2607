package com.zeroone.star.project.dto.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "取消执行请求")
public class CancelExecuteRequest {

    @NotEmpty(message = "执行记录ID列表不能为空")
    @Schema(description = "执行记录ID列表（支持批量）", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"1\", \"2\", \"3\"]")
    private List<String> record_ids;

    @NotBlank(message = "就诊ID不能为空")
    @Schema(description = "就诊ID（encounterId）", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private String encounter_id;

    @NotBlank(message = "取消原因不能为空")
    @Schema(description = "取消原因", requiredMode = Schema.RequiredMode.REQUIRED, example = "患者拒绝")
    private String cancel_reason;

    @Schema(description = "取消原因编码", example = "PATIENT_REFUSE")
    private String cancel_reason_code;

    @Schema(description = "操作者ID", example = "1")
    private String operator_id;

    @Schema(description = "操作者姓名", example = "护士小王")
    private String operator_name;

    @Schema(description = "幂等性Key（用于防止重复提交，建议使用UUID）", example = "550e8400-e29b-41d4-a716-446655440000")
    private String idempotency_key;
}