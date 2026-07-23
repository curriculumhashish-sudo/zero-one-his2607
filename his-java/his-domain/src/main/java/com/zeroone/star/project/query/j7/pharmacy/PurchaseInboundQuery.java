package com.zeroone.star.project.query.j7.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalDateTime;

/**
 * 药房管理-药房入库单-获取单据列表查询对象
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "入库单查询对象")
public class PurchaseInboundQuery extends PageQuery {

    @Schema(description = "单据编号 (对应 wkf_supply_request.bus_no)", example = "RK20260721001")
    private String busNo;

    @Schema(description = "供应商ID (对应 wkf_supply_request.supplier_id)", example = "SUP_1001")
    private String supplierId;

    @Schema(description = "单据状态 (对应 wkf_supply_request.status_enum)", example = "DRAFT")
    private String status;

    @Schema(description = "开始时间 (对应 wkf_supply_request.created_at)", example = "2026-07-01 00:00:00")
    private LocalDateTime startTime;

    @Schema(description = "结束时间 (对应 wkf_supply_request.created_at)", example = "2026-07-21 23:59:59")
    private LocalDateTime endTime;
}