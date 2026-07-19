package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 进货单列表行展示对象。
 */
@Data
@Schema(name = "InboundReceiptListVO", description = "药库进货单列表行")
public class InboundReceiptListVO {
    @Schema(description = "业务单号", example = "JHD202607190001")
    private String busNo;

    @Schema(description = "供应商名称", example = "华南医疗设备制造有限公司")
    private String supplierName;

    @Schema(description = "供应状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "创建人名称", example = "药库管理员")
    private String creatorName;

    @Schema(description = "审批人名称", example = "财务审批员")
    private String approverName;

    @Schema(description = "单据总金额", example = "730.000000")
    private BigDecimal totalAmount;

    @Schema(description = "审批时间", example = "2026-07-19T11:00:00")
    private LocalDateTime approvalTime;

    @Schema(description = "申请时间", example = "2026-07-19T10:35:00")
    private LocalDateTime applyTime;
}
