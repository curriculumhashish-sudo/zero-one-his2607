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
@Schema(description = "获取单据列表")
public class PharmacyInventoryDetailQuery{

    @Schema(description = "单据号ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "CST2026-07-190004")
    private String busNo;
}