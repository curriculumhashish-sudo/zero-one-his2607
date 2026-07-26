package com.zeroone.star.project.dto.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "切换器材状态请求（支持批量）")
public class EquipmentStatusDTO {

    @NotEmpty(message = "主键列表不能为空")
    @Schema(description = "主键列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<@NotBlank(message = "主键不能为空") String> ids;
}
