package com.zeroone.star.project.dto.j7.pharmacy.approval;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

@Data
@Schema(description = "药房盘点审批DTO")
public class PharmacyStockTakeDTO {

    @Schema(description = "盘点单明细编号", example = "PST2026-06-300001")
    @NotBlank(message = "盘点单明细编号不能为空")
    @Size(max = 255, message = "盘点单明细编号长度不能超过255个字符")
    private String busNo;

    @Schema(description = "审批人标识", example = "308791351298756608")
    @NotBlank(message = "审批人标识不能为空")
    @Size(max = 64, message = "审批人标识长度不能超过64个字符")
    private String approverId;
}
