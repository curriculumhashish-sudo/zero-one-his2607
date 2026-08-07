package com.zeroone.star.project.vo.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 药库盘点单据列表行VO。
 */
@Data
@Schema(description = "药库盘点单据列表行")
public class PharmacyInventoryVO {
    @Schema(description = "盘点单号", example = "CST202608070001")
    private String busNo;

    @Schema(description = "状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "状态名称", example = "草稿")
    private String statusEnumText;

    @Schema(description = "目标库房ID", example = "322954885380706304")
    private String purposeLocationId;

    @Schema(description = "目标库房名称", example = "西药库")
    private String purposeLocationName;

    @Schema(description = "药品条目数量", example = "2")
    private Integer itemCount;

    @Schema(description = "盈亏总数量", example = "0.00")
    private BigDecimal profitLossQuantitySum;

    @Schema(description = "实际总金额", example = "1336.00")
    private BigDecimal actualAmountSum;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称", example = "库管")
    private String applicantName;

    @Schema(description = "申请时间", example = "2026-08-07 10:00:00")
    private String applyTime;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称", example = "admin")
    private String approverName;

    @Schema(description = "审批时间", example = "2026-08-07 15:00:00")
    private String approvalTime;

    @Schema(description = "盘点发生时间", example = "2026-08-07 09:00:00")
    private String occurrenceTime;
}
