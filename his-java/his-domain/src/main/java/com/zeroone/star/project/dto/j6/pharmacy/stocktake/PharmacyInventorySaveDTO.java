package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>描述：新增单据对象</p>
 * <p>版权: ©copy;01星球</p>
 * <p>地址: 01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "新增药房盘点单提交DTO")
public class PharmacyInventorySaveDTO {

    @Schema(description = "盘点药库ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "xyk")
    private String pharmacyId;

    @Schema(description = "项目类型", requiredMode = Schema.RequiredMode.REQUIRED, example = "药品")
    private String itemType;

    @Schema(description = "医保契约编号", requiredMode = Schema.RequiredMode.REQUIRED, example = "省医保(H22010402403-229900)")
    private String medicalInsuranceNo;
}