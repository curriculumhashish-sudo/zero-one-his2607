package com.zeroone.star.project.dto.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "执行请求")
public class ExecuteRequest {

    @Schema(description = "执行记录ID列表（支持批量）", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"1\", \"2\", \"3\"]")
    private List<String> record_ids;

    @Schema(description = "执行者ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private String executor_id;

    @Schema(description = "执行时间", requiredMode = Schema.RequiredMode.REQUIRED, example = "2024-01-15 10:30:00")
    private String execute_time;

    @Schema(description = "备注", example = "执行完成")
    private String remark;
}