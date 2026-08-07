package com.zeroone.star.project.query.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "药库盘点单据详情查询条件")
public class PharmacyInventoryDetailQuery {
    @Schema(description = "盘点单号", example = "CST202608070001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String stocktakeNo;
}
