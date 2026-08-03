package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

@Data
@Schema(description = "入库单删除请求对象")
public class InStockOrderDeleteDTO {

    @NotBlank(message = "单据号不能为空")
    @Size(max = 255, message = "单据号长度不能超过255个字符")
    @Schema(description = "入库单业务单号", example = "PSI2026-06-300001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Size(max = 255, message = "删除原因长度不能超过255个字符")
    @Schema(description = "删除原因", example = "录入错误")
    private String reason;
}
