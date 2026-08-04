package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "批量删除号源参数")
public class BatchDeleteSlotDTO {
    @NotEmpty(message = "号源ID列表不能为空")
    @Schema(description = "号源id数组", example = "[\"342654375705448448\"]")
    private List<String> slotIds;
}