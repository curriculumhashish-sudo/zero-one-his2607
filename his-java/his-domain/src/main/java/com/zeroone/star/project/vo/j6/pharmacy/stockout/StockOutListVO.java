package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 药库出库单列表行。
 */
@Data
@Schema(name = "StockOutListVO", description = "药库出库单列表行")
public class StockOutListVO {

    @Schema(description = "业务单号", example = "CSO202608070001")
    private String busNo;

    @Schema(description = "单据类型编码", example = "WAREHOUSE_STOCKOUT")
    private String typeEnum;

    @Schema(description = "状态编码（PENDING_APPROVAL=待审核, APPROVAL=审核中, AGREE=已通过, REJECT=已驳回, WITHDRAW=已撤回）", example = "PENDING_APPROVAL")
    private String statusEnum;

    @Schema(description = "状态文本", example = "草稿")
    private String statusEnumText;

    @Schema(description = "源药库ID", example = "322954885380706304")
    private String sourceLocationId;

    @Schema(description = "源药库名称", example = "西药库")
    private String sourceLocationName;

    @Schema(description = "目的药房ID", example = "322954949549363200")
    private String purposeLocationId;

    @Schema(description = "目的药房名称", example = "门诊西药房")
    private String purposeLocationName;

    @Schema(description = "申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "发生时间")
    private LocalDateTime occurrenceTime;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "制单人名称")
    private String createdBy;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称")
    private String approverName;

    @Schema(description = "单据总金额", example = "17147.00")
    private BigDecimal totalAmount;

    @Schema(description = "原始业务单号")
    private String originalBusNo;
}
