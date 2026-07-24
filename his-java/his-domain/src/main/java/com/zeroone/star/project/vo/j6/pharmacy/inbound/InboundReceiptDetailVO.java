package com.zeroone.star.project.vo.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(name = "InboundReceiptDetailVO", description = "药库进货单详情")
public class InboundReceiptDetailVO {

    @Schema(description = "业务单号")
    private String busNo;

    @Schema(description = "来源单号")
    private String originalBusNo;

    @Schema(description = "类型")
    private String typeEnum;

    @Schema(description = "状态")
    private String statusEnum;

    @Schema(description = "供应商ID")
    private String supplierId;

    @Schema(description = "出库方库房ID")
    private String sourceLocationId;

    @Schema(description = "入库方库房ID")
    private String purposeLocationId;

    @Schema(description = "出库方类型")
    private String sourceTypeEnum;

    @Schema(description = "入库方类型")
    private String purposeTypeEnum;

    @Schema(description = "类别")
    private String categoryEnum;

    @Schema(description = "版本号")
    private Integer rowVersion;

    @Schema(description = "申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "原因")
    private String reason;

    @Schema(description = "明细行列表")
    private List<InboundReceiptDetailLineVO> lines;
}
