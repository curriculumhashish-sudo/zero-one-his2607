package com.zeroone.star.project.query.j7.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;
import java.time.LocalDateTime;

/**
 * 药房管理-历史入库单-获取单据列表查询对象
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "入库单查询对象")
public class PurchaseInboundQuery extends PageQuery {

    @Schema(description = "单据编号检索 (对应 wkf_supply_request.bus_no)", example = "PSI2026-06-300001")
    private String busNo;

    @Schema(description = "来源库房ID (对应 wkf_supply_request.source_location_id)", example = "322954885380706304")
    private String sourceLocationId;

    @Schema(description = "目的库房ID (对应 wkf_supply_request.purpose_location_id)", example = "322954949549363200")
    private String purposeLocationId;

    @Schema(description = "单据状态 (对应 wkf_supply_request.status_enum)：PENDING_APPROVAL-待审批 APPROVAL-审批中 AGREE-已通过 WITHDRAW-已撤回", example = "AGREE")
    private String statusEnum;

    @Schema(description = "开始时间 (对应 wkf_supply_request.apply_time)", example = "2026-06-30 00:00:00")
    private LocalDateTime startTime;

    @Schema(description = "结束时间 (对应 wkf_supply_request.apply_time)", example = "2026-06-30 23:59:59")
    private LocalDateTime endTime;
}