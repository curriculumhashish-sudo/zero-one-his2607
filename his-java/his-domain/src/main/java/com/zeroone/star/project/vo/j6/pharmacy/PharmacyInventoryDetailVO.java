package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.util.List;

@Data
@Schema(description = "盘点单据详情VO")
public class PharmacyInventoryDetailVO {

    @Schema(description = "单据号", example = "CST2026-07-230004")
    private String busNo;

    @Schema(description = "状态编码", example = "AGREE")
    private String statusEnum;

    @Schema(description = "目标库房ID")
    private String purposeLocationId;

    @Schema(description = "目标库房名称", example = "中药库")
    private String purposeLocationName;

    @Schema(description = "执业人员ID")
    private String practitionerId;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请时间", example = "2026-07-23 16:16:57")
    private String applyTime;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批时间")
    private String approvalTime;

    @Schema(description = "盘点明细行列表")
    private List<StocktakeDetailVO> lines;
}