package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.math.BigDecimal;

@Data
@Schema(description = "药房盘点单据列表条目VO")
public class PharmacyInventoryItemVO {
    @Schema(description = "单据号", example = "CST2026-07-230004")
    private String busNo;

    @Schema(description = "状态编码", example = "AGREE")
    private String statusEnum;

    @Schema(description = "状态名称", example = "同意")
    private String statusEnumText;

    @Schema(description = "目标库房ID")
    private String purposeLocationId;

    @Schema(description = "目标库房名称", example = "中药库")
    private String purposeLocationName;

    @Schema(description = "明细条目数量", example = "1")
    private Integer itemCount;

    @Schema(description = "损益总数量")
    private BigDecimal profitLossQuantitySum;

    @Schema(description = "实际总金额")
    private BigDecimal actualAmountSum;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称", example = "admin")
    private String applicantName;

    @Schema(description = "申请时间", example = "2026-07-23 16:16:57")
    private String applyTime;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称", example = "admin")
    private String approverName;

    @Schema(description = "审批时间")
    private String approvalTime;

    @Schema(description = "业务发生时间")
    private String occurrenceTime;
}
