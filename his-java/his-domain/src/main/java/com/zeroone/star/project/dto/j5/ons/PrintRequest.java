package com.zeroone.star.project.dto.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "打印请求")
public class PrintRequest {

    @Schema(description = "执行记录ID列表", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"1\", \"2\"]")
    private List<String> record_ids;

    @Schema(description = "打印类型", example = "EXECUTION_RECORD")
    private String print_type;
}