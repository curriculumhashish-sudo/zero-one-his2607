package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Schema(description = "审批操作结果")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class StockOutApprovalResultVO {
    /**
     * 操作是否成功
     */
    @Schema(description = "操作是否成功", example = "true")
    private Boolean success;

    /**
     * 操作后单据最新状态枚举码
     */
    @Schema(description = "操作后单据最新状态（APPROVAL/WITHDRAW/AGREE/REJECT）",example = "APPROVAL")
    private String statusEnum;
}
