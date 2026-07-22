package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;


@Data
@Schema(description = "科室查询请求参数")
public class DeptQueryDTO {

    @NotNull(message = "医院ID不能为空")
    @Schema(description = "医院ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1001")
    private String hospitalId;
}
