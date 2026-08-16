package com.zeroone.star.project.query.j6.pharmacy.inbound;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import lombok.ToString;

import java.time.LocalDateTime;

@Data
@ToString
@Schema(description = "进货单列表查询参数")
public class InboundReceiptQuery extends PageQuery {

    @Schema(description = "单据号（模糊搜索）", example = "CIR202608070001")
    private String keyword;

    @Schema(description = "申请开始时间", example = "2026-07-01T00:00:00")
    private LocalDateTime applyTimeFrom;

    @Schema(description = "申请结束时间", example = "2026-07-31T23:59:59")
    private LocalDateTime applyTimeTo;

    @Schema(description = "库房ID（选择药库）", example = "322954885380706304")
    private String purposeLocationId;
}
