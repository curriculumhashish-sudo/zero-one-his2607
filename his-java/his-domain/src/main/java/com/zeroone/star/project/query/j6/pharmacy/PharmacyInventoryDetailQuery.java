package com.zeroone.star.project.query.j6.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述： 获取单据详情
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "药库盘点单据详情查询条件")
public class PharmacyInventoryDetailQuery {
    @Schema(description = "盘点单据ID", requiredMode = Schema.RequiredMode.REQUIRED,example = "CST2026-07-190004")
    private String stocktakeNo;

}


