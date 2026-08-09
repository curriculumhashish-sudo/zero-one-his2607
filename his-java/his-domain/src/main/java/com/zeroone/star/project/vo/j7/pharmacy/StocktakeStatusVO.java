package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@Schema(description = "盘点单审批状态对象")
public class StocktakeStatusVO {
    /**
     * 盘点单号
     */
    @Schema(description = "盘点单号", example = "PST202608080001")
    private String busNo;
    /**
     * 类型枚举
     */
    @Schema(description = "类型枚举", example = "PHARMACY_STOCKTAKE")
    private String typeEnum;
    /**
     * 状态枚举：DRAFT / PENDING_APPROVAL / AGREE / REJECT
     */
    @Schema(description = "状态枚举：DRAFT / PENDING_APPROVAL / AGREE / REJECT", example = "PENDING_APPROVAL")
    private String statusEnum;

    /**
     * 制单人
     */
    @Schema(description = "制单人标识", example = "308791351298756608")
    private String createdBy;
    /**
     * 制单时间
     */
    @Schema(description = "制单时间", example = "2026-08-08 09:00:00")
    private LocalDateTime createdAt;
    /**
     * 提交审批人
     */
    @Schema(description = "提交审批人标识", example = "308791351298756608")
    private String applicantId;
    /**
     * 提交审批时间
     */
    @Schema(description = "提交审批时间", example = "2026-08-08 09:30:00")
    private LocalDateTime applyTime;
    /**
     * 审批人
     */
    @Schema(description = "审批人标识", example = "308791351298756609")
    private String approverId;
    /**
     * 审批时间
     */
    @Schema(description = "审批时间", example = "2026-08-08 10:00:00")
    private LocalDateTime approvalTime;

    /**
     * 盘点总价（所有明细 total_price 之和）
     */
    @Schema(description = "盘点总价（所有明细 total_price 之和）", example = "1187.50")
    private BigDecimal totalAmount;
}
