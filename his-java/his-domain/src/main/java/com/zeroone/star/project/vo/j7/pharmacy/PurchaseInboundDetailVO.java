package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 药房管理-药房入库单-获取单据详情响应对象
 */
@Data
@Schema(description = "入库单详情")
public class PurchaseInboundDetailVO {

    // --- 表头信息 ---
    @Schema(description = "主键ID", example = "1")
    private String id;

    @Schema(description = "单据编号", example = "RK20260721001")
    private String busNo;

    @Schema(description = "供应商ID", example = "SUP_1001")
    private String supplierId;

    @Schema(description = "供应商名称", example = "某某药业有限公司")
    private String supplierName;

    @Schema(description = "入库库房ID (对应 wkf_supply_request.purpose_location_id)", example = "WH_01")
    private String warehouseId;

    @Schema(description = "库房名称", example = "中心药房")
    private String warehouseName;

    @Schema(description = "总金额", example = "1500.00")
    private BigDecimal totalPrice;

    @Schema(description = "备注 (对应 wkf_supply_request.remake)", example = "紧急入库")
    private String remark;

    @Schema(description = "单据状态", example = "DRAFT")
    private String status;

    @Schema(description = "申请时间", example = "2026-07-21 10:00:00")
    private LocalDateTime applyTime;

    // --- 明细列表 ---
    @Schema(description = "入库明细列表")
    private List<PurchaseInboundItemVO> items;
}