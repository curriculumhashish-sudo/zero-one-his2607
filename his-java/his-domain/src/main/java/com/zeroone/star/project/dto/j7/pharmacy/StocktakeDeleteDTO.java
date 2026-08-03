package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

@Data
@Schema(description = "盘点单删除请求对象")
public class StocktakeDeleteDTO {

    @NotBlank(message = "单据号不能为空")
    @Size(max = 255, message = "单据号长度不能超过255个字符")
    @Schema(description = "盘点单业务单号", example = "PST20260630120000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Size(max = 255, message = "删除原因长度不能超过255个字符")
    @Schema(description = "删除原因", example = "盘点数据错误")
    private String reason;
}
