package com.zeroone.star.project.query.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：住院退药-患者退药明细查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author jacobzp
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@Schema(description = "住院退药-患者退药明细查询对象")
public class InpatientRefundDetailQuery {

    /**
     * 就诊ID
     */
    @NotBlank(message = "就诊ID不能为空")
    @Schema(description = "就诊ID", example = "330316881840836608", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterId;

    /**
     * 退药状态：PENDING_REFUND-待退药 / REFUNDED-已退药（展示态；前置条件 dispense_enum=REFUND。
     * 兼容传入 PENDING；库表 status_enum 待退药存 PENDING）
     */
    @Schema(description = "退药状态（PENDING_REFUND/REFUNDED，兼容 PENDING）", example = "PENDING_REFUND")
    private String refundStatus;
}
