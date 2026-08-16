package com.zeroone.star.project.dto.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "执行请求")
public class ExecuteRequest {

    @NotEmpty(message = "执行记录ID列表不能为空")
    @Schema(description = "执行记录ID列表（支持批量）", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"1\", \"2\", \"3\"]")
    private List<String> record_ids;

    @NotBlank(message = "就诊ID不能为空")
    @Schema(description = "就诊ID（encounterId）", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private String encounter_id;

    @NotBlank(message = "执行者ID不能为空")
    @Schema(description = "执行者ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private String executor_id;

    @Schema(description = "执行者姓名", example = "护士小王")
    private String executor_name;

    @Schema(description = "执行时间（为空则使用当前时间）", example = "2024-01-15 10:30:00")
    private String execute_time;

    @Schema(description = "幂等性Key（用于防止重复提交，建议使用UUID）", example = "550e8400-e29b-41d4-a716-446655440000")
    private String idempotency_key;

    @Schema(description = "执行类型", example = "NORMAL")
    private String execute_type;

    @Schema(description = "备注", example = "执行完成")
    private String remark;
}