package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 药库盘点单保存/保存并提交请求对象。
 * 结构与新增单据返回响应一致，前端编辑后原样回传。
 */
@Data
@Schema(name = "StocktakeSaveRequestDTO", description = "药库盘点单保存/保存并提交请求")
public class StocktakeSaveRequestDTO {

    @Schema(description = "盘点单号（新增时为空，保存已有单据时必传）", example = "CST202608070001")
    private String busNo;

    @Schema(description = "单据状态编码", example = "DRAFT")
    private String statusEnum;

    @NotBlank(message = "库房不能为空")
    @Schema(description = "盘点库房ID", example = "322954885380706304")
    private String purposeLocationId;

    @Schema(description = "库房名称（展示用，后端忽略）", example = "西药库")
    private String purposeLocationName;

    @Schema(description = "盘点人ID", example = "322932096108597248")
    private String practitionerId;

    @Schema(description = "盘点人名称（展示用，后端忽略）", example = "库管")
    private String practitionerName;

    @Schema(description = "制单人ID")
    private String creatorId;

    @Schema(description = "制单人名称（展示用，后端忽略）", example = "库管")
    private String creatorName;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称（展示用，后端忽略）", example = "库管")
    private String applicantName;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称（展示用，后端忽略）", example = "admin")
    private String approverName;

    @Schema(description = "制单/申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "单据总金额（展示用，后端忽略）", example = "2510.00")
    private BigDecimal totalAmount;

    @Schema(description = "盘点类型", example = "PHARMACY_STOCKTAKE")
    private String categoryEnum;

    @Valid
    @NotEmpty(message = "盘点明细不能为空")
    @Schema(description = "盘点明细行列表")
    private List<StocktakeLineDTO> lines;

    /**
     * 药库盘点单明细行。
     */
    @Data
    @Schema(name = "StocktakeLineDTO", description = "药库盘点单明细行")
    public static class StocktakeLineDTO {

        @NotBlank(message = "库存项ID不能为空")
        @Schema(description = "库存项ID（指向 wkf_inventory_item.id）", example = "323016347713802240")
        private String inventoryItemId;

        @NotNull(message = "实盘数量不能为空")
        @Schema(description = "实盘数量（≥0）", example = "100.00")
        private BigDecimal actualQuantity;

        @Schema(description = "盈亏数量（正=盘盈，负=盘亏）", example = "0.00")
        private BigDecimal profitLossQuantity;

        @NotBlank(message = "计量单位编码不能为空")
        @Schema(description = "计量单位编码", example = "1")
        private String unitCode;

        @Schema(description = "批号", example = "001-002")
        private String lotNumber;

        @Schema(description = "追溯码", example = "TR2026080100001")
        private String traceNo;

        @Schema(description = "行备注", example = "盘点正常")
        private String remake;
    }
}
