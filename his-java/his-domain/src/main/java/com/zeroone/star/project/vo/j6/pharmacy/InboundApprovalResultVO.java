package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：审批操作通用结果
 * </p>
 * @author AB
 * @version 1.0.0
 */
@Schema(description = "审批操作结果")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class InboundApprovalResultVO {

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
