package com.zeroone.star.project.vo.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "盘点单审批操作结果")
public class StocktakeApprovalResultVO {

    @Schema(description = "操作是否成功", example = "true")
    private Boolean success;

    @Schema(description = "操作后单据最新状态", example = "APPROVAL")
    private String statusEnum;
}
