package com.zeroone.star.project.query.j6.pharmacy.stocktake;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "单据列表(条件+分页)")
public class PharmacyInventoryQuery extends PageQuery {

    @Schema(description = "盘点单号检索", example = "CST202608070001")
    private String stocktakeNo;

    @Schema(description = "开始日期", example = "2026-07-01")
    private String startDate;

    @Schema(description = "结束日期", example = "2026-08-07")
    private String endDate;

    @Schema(description = "盘点药库", example = "西药库")
    private String pharmacy;
}
