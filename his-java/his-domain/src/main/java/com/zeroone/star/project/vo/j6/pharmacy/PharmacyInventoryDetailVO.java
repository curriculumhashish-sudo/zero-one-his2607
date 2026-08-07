package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.util.List;

@Data
@Schema(description = "药库盘点单据详情")
public class PharmacyInventoryDetailVO {
    @Schema(description = "盘点单号", example = "CST202608070001")
    private String busNo;

    @Schema(description = "单据状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "目标库房ID", example = "322954885380706304")
    private String purposeLocationId;

    @Schema(description = "目标库房名称", example = "西药库")
    private String purposeLocationName;

    @Schema(description = "执行人ID", example = "322932096108597248")
    private String practitionerId;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请时间", example = "2026-08-07 10:00:00")
    private String applyTime;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批时间", example = "2026-08-07 15:00:00")
    private String approvalTime;

    @Schema(description = "单据总金额", example = "1336.00")
    private BigDecimal totalAmount;

    @Schema(description = "盘点明细行列表")
    private List<PharmacyInventoryItemVO> lines;
}
