package com.zeroone.star.project.dto.j7.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：盘点单保存请求
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单保存请求")
public class StocktakeSaveDTO {

    @Schema(description = "盘点单业务单号；新增保存时为空，编辑保存时必传", example = "PD202607230001")
    private String busNo;

    @NotBlank(message = "盘点位置不能为空")
    @Schema(description = "盘点库房/药房位置ID", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String purposeLocationId;

    @NotBlank(message = "经手人不能为空")
    @Schema(description = "经手人ID", example = "2001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String practitionerId;

    @Schema(description = "供应类别，默认 STOCK_SUPPLY", example = "STOCK_SUPPLY", defaultValue = "STOCK_SUPPLY")
    private String categoryEnum = "STOCK_SUPPLY";

    @Schema(description = "单据备注", example = "期末库存盘点")
    private String remark;

    @Valid
    @NotEmpty(message = "盘点明细不能为空")
    @Schema(description = "盘点明细行", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<StocktakeLineDTO> lines;
}
