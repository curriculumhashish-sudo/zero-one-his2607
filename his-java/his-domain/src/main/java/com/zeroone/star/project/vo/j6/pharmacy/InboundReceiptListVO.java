package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDateTime;

// 进货单 列表行（对应前端 InboundReceiptListRowDto）
// 字段：busNo, supplierName, statusEnum, creatorName, approverName, totalAmount, approvalTime, applyTime
@Getter
@Setter
@Schema(description = "进货单列表行")
public class InboundReceiptListVO {

    @Schema(description = "业务单号")
    private String busNo;

    @Schema(description = "供应商名称")
    private String supplierName;

    @Schema(description = "状态")
    private String statusEnum;

    @Schema(description = "创建人")
    private String creatorName;

    @Schema(description = "审批人")
    private String approverName;

    @Schema(description = "总金额")
    private BigDecimal totalAmount;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "申请时间")
    private LocalDateTime applyTime;
}
