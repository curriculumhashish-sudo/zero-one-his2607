package com.zeroone.star.project.query.j6.pharmacy;

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

    @Schema(description = "盘点单据号", example = "CST2026-07-230004")
    private String busNo;

    @Schema(description = "单据状态", example = "AGREE")
    private String statusEnum;

    @Schema(description = "库房ID")
    private String purposeLocationId;

    @Schema(description = "申请开始时间")
    private String applyTimeStart;

    @Schema(description = "申请结束时间")
    private String applyTimeEnd;
}