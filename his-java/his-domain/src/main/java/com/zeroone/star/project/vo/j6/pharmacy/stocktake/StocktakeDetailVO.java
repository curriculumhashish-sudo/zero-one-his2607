package com.zeroone.star.project.vo.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 盘点单 详情 VO
 *
 * @author Dome
 * @since 2026-07-18
 */
@Data
public class StocktakeDetailVO {

    // ==================== 头信息 ====================

    @Schema(description = "盘点单号", example = "CST2026-07-200001")
    private String busNo;

    @Schema(description = "单据状态（DRAFT/PENDING_APPROVAL/APPROVED/REJECTED）", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "库房ID", example = "284503862625177600")
    private String purposeLocationId;

    @Schema(description = "库房名称", example = "西药库")
    private String purposeLocationName;

    @Schema(description = "盘点人ID", example = "322733572165734400")
    private String practitionerId;

    @Schema(description = "申请人ID（提交审批时写入）", example = "322733572165734400")
    private String applicantId;

    @Schema(description = "申请时间（提交审批时写入）", example = "2026-07-18 14:00:00")
    private LocalDateTime applyTime;

    @Schema(description = "审批人ID（审批后写入）", example = "322733572165734401")
    private String approverId;

    @Schema(description = "审批时间（审批后写入）", example = "2026-07-18 15:00:00")
    private LocalDateTime approvalTime;

    @Schema(description = "项目类型(盘点类型)", example = "药品/耗材")
    private String categoryEnum;

    // ==================== 明细行 ====================

    @Schema(description = "明细行列表")
    private List<StocktakeDetailLineVO> lines;

    // ==================== 内嵌类 ====================

    /**
     * 盘点单明细行 VO
     */
    @Data
    public static class StocktakeDetailLineVO {

        @Schema(description = "明细行ID", example = "line_001")
        private String lineId;

        @Schema(description = "物品定义表名", example = "medication")
        private String itemTable;

        @Schema(description = "物品定义ID", example = "med_001")
        private String itemId;

        @Schema(description = "物品名称（冗余展示）", example = "阿莫西林胶囊")
        private String productName;

        @Schema(description = "规格", example = "0.25g")
        private String specification;

        @Schema(description = "生产厂家", example = "华北制药")
        private String manufacturer;

        @Schema(description = "批号", example = "001-003")
        private String lotNumber;

        @Schema(description = "效期", example = "2028-06-30")
        private String expirationDate;

        @Schema(description = "计量单位编码", example = "PCS")
        private String unitCode;

        @Schema(description = "计量单位名称（展示用）", example = "片")
        private String unitName;

        @Schema(description = "账面数量（保存时快照锁定的当前库存数）", example = "95.00")
        private BigDecimal bookQuantity;

        @Schema(description = "盈亏数量（正=盘盈，负=盘亏，零=平）", example = "5.00")
        private BigDecimal profitLossQuantity;

        @Schema(description = "实盘数量", example = "100.00")
        private BigDecimal actualQuantity;

        @Schema(description = "实盘金额（actualQuantity × price）", example = "2510.00")
        private BigDecimal actualAmount;

        @Schema(description = "盈亏金额", example = "125.50")
        private BigDecimal profitLossAmount;

        @Schema(description = "单价", example = "25.10")
        private BigDecimal price;

        @Schema(description = "库存项ID（指向 wkf_inventory_item.id）", example = "inv_001")
        private String inventoryItemId;

        @Schema(description = "供应商标识", example = "supplier_001")
        private String supplierId;

        @Schema(description = "追溯码", example = "TRACE001")
        private String traceNo;

        @Schema(description = "备注", example = "盘点正常")
        private String remake;

        @Schema(description = "盘点发生时间", example = "2026-07-18 10:00:00")
        private LocalDateTime occurrenceTime;
    }
}
