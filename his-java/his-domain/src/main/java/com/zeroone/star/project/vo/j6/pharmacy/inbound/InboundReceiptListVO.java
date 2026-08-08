package com.zeroone.star.project.vo.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@Schema(name = "InboundReceiptListVO", description = "药库进货单列表行")
public class InboundReceiptListVO {
    @Schema(description = "业务单号", example = "CIR202608070001")
    private String busNo;

    @Schema(description = "供应商名称", example = "华中药品配送有限公司")
    private String supplierName;

    @Schema(description = "供应状态编码（PENDING_APPROVAL=待审核, APPROVAL=审核中, AGREE=已通过, REJECT=已驳回, WITHDRAW=已撤回）", example = "PENDING_APPROVAL")
    private String statusEnum;

    @Schema(description = "创建人/制单人名称", example = "库管")
    private String creatorName;

    @Schema(description = "审批人名称", example = "admin")
    private String approverName;

    @Schema(description = "单据总金额", example = "17147.00")
    private BigDecimal totalAmount;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "库房名称", example = "西药库")
    private String purposeLocationName;
}
