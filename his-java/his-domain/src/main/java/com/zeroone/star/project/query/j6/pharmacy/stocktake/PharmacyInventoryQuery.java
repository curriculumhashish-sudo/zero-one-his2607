package com.zeroone.star.project.query.j6.pharmacy.stocktake;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述： 获取单据列表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "单据列表(条件+分页)")
public class PharmacyInventoryQuery extends PageQuery {

    @Schema(description = "盘点单据号检索", example = "CST2026-07-190004")
    private String stocktakeNo;

    @Schema(description = "开始日期", example = "2026-07-01")
    private String startDate;

    @Schema(description = "结束日期", example = "2026-07-27")
    private String endDate;

    @Schema(description = "盘点药库", example = "西药库")
    private String pharmacy;
}