package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 新增药库盘点单 — 简洁请求。
 * 只传库房、盘点人、盘点类型，服务端生成空白草稿并返回完整文档结构。
 */
@Data
@Schema(name = "StocktakeAddDTO", description = "新增盘点单的简洁请求数据传输对象")
public class StocktakeAddDTO {

    @NotBlank(message = "库房不能为空")
    @Schema(
            description = "盘点库房ID",
            example = "322954885380706304",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String purposeLocationId;

    @Schema(
            description = "盘点人ID（不传则服务端从当前登录用户取）",
            example = "322932096108597248"
    )
    private String practitionerId;

    @NotBlank(message = "盘点类型不能为空")
    @Schema(
            description = "盘点类型（PHARMACY_STOCKTAKE）",
            example = "PHARMACY_STOCKTAKE",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String categoryEnum;
}
