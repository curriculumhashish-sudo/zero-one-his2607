package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>描述：单据详情对象 </p>
 * <p>版权: ©copy;01星球</p>
 * <p>地址: 01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "药房盘点单详情查询")
public class PharmacyInventoryDetailQueryDTO {

    @Schema(description = "单据号ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "CST2026-07-190004")
    private Long id;
}