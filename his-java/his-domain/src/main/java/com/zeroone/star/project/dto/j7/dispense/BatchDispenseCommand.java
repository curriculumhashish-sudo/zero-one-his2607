package com.zeroone.star.project.dto.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "门诊批量发药请求数据")
public class BatchDispenseCommand {

    @Schema(description = "发药记录ID列表", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotEmpty(message = "发药记录ID列表不能为空")
    private List<String> dispenseIds;

    @Schema(description = "发药人ID（不传则取当前登录用户）", example = "prac_001")
    private String practitionerId;
}
