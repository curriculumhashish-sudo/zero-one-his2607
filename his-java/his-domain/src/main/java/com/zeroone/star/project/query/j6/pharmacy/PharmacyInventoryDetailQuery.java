package com.zeroone.star.project.query.j6.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述： 获取单据列表(条件+分页)
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "获取单据列表(条件+分页)")
public class PharmacyInventoryDetailQuery extends PageQuery {

    @Schema(description = "单据号", example = "CST2026-07-190002", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String stockID;

    @Schema(description = "盘点开始日期", example = "2026-06-01", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String startDate;

    @Schema(description = "盘点结束日期", example = "2026-06-30", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String endDate;

    @Schema(description = "药房ID", example = "西药库", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pharmacyId;
}