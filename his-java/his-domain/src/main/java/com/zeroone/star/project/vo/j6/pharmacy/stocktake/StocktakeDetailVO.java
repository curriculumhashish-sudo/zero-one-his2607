package com.zeroone.star.project.vo.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 盘点单详情 — 新增返回 / 详情查询共用。
 */
@Data
@Schema(name = "StocktakeDetailVO", description = "药库盘点单详情（新增返回 + 详情查询）")
public class StocktakeDetailVO {

    // ==================== 头信息 ====================

    @Schema(description = "盘点单号", example = "CST202608070001")
    private String busNo;

    @Schema(description = "单据状态（DRAFT/PENDING_APPROVAL/AGREE/REJECT）", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "库房ID", example = "322954885380706304")
    private String purposeLocationId;

    @Schema(description = "库房名称", example = "西药库")
    private String purposeLocationName;

    @Schema(description = "盘点人ID", example = "322932096108597248")
    private String practitionerId;

    @Schema(description = "盘点人名称", example = "库管")
    private String practitionerName;

    @Schema(description = "制单人ID")
    private String creatorId;

    @Schema(description = "制单人名称", example = "库管")
    private String creatorName;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称", example = "库管")
    private String applicantName;

    @Schema(description = "制单/申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称", example = "admin")
    private String approverName;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "盘点类型", example = "PHARMACY_STOCKTAKE")
    private String categoryEnum;

    @Schema(description = "单据总金额", example = "2510.00")
    private BigDecimal totalAmount;

    // ==================== 明细行 ====================

    @Schema(description = "明细行列表")
    private List<StocktakeDetailLineVO> lines;

    // ==================== 内嵌类 ====================

    @Data
    @Schema(name = "StocktakeDetailLineVO", description = "药库盘点单明细行")
    public static class StocktakeDetailLineVO {

        @Schema(description = "明细行ID", example = "323016347713802240")
        private String lineId;

        @Schema(description = "物品定义表名", example = "med_medication_definition")
        private String itemTable;

        @Schema(description = "物品定义ID", example = "322994478289784832")
        private String itemId;

        @Schema(description = "物品名称", example = "替米沙坦片")
        private String productName;

        @Schema(description = "规格", example = "40mg")
        private String specification;

        @Schema(description = "生产厂家", example = "上海信谊天平药业有限公司")
        private String manufacturer;

        @Schema(description = "批号", example = "001-002")
        private String lotNumber;

        @Schema(description = "效期", example = "2027-07-31 00:00:00")
        private String expirationDate;

        @Schema(description = "计量单位编码", example = "1")
        private String unitCode;

        @Schema(description = "计量单位名称（展示用）", example = "盒")
        private String unitName;

        @Schema(description = "账面数量", example = "100.00")
        private BigDecimal bookQuantity;

        @Schema(description = "盈亏数量（正=盘盈，负=盘亏）", example = "0.00")
        private BigDecimal profitLossQuantity;

        @Schema(description = "实盘数量", example = "100.00")
        private BigDecimal actualQuantity;

        @Schema(description = "实盘金额（actualQuantity × price）", example = "1336.00")
        private BigDecimal actualAmount;

        @Schema(description = "盈亏金额", example = "0.00")
        private BigDecimal profitLossAmount;

        @Schema(description = "单价", example = "13.36")
        private BigDecimal price;

        @Schema(description = "库存项ID（指向 wkf_inventory_item.id）", example = "323016347713802240")
        private String inventoryItemId;

        @Schema(description = "供应商标识", example = "4100000000000000004")
        private String supplierId;

        @Schema(description = "追溯码", example = "TR2026080100001")
        private String traceNo;

        @Schema(description = "备注", example = "盘点正常")
        private String remake;

        @Schema(description = "盘点发生时间", example = "2026-08-07 10:00:00")
        private LocalDateTime occurrenceTime;
    }
}
