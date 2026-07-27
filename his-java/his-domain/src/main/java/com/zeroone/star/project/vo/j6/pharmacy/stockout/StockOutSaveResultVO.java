package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 出库单保存结果VO
 */
@Data
@Schema(description = "出库单保存结果VO")
public class StockOutSaveResultVO {

    @Schema(description = "单据号")
    private String busNo;

    @Schema(description = "状态（DRAFT-草稿，PENDING_APPROVAL-待审批）")
    private String status;

    @Schema(description = "操作消息")
    private String message;

    @Schema(description = "版本号")
    private Integer version;
}