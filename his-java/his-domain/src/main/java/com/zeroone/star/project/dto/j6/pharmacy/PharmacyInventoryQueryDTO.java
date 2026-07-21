package com.zeroone.star.project.dto.j6.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>描述：单据列表详情对象 </p>
 * <p>版权: ©copy;01星球</p>
 * <p>地址: 01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "药房盘点单列表查询条件")
public class PharmacyInventoryQueryDTO extends PageQuery {

    @Schema(description = "单据号检索", example = "CST2026-07-190002", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String stocktakeNo;

    @Schema(description = "开始日期", example = "2026-07-01", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String startDate;

    @Schema(description = "结束日期", example = "2026-07-19", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String endDate;

    @Schema(description = "盘点药库", example = "西药库", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pharmacy;


}