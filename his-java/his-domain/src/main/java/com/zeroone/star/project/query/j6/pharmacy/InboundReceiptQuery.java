package com.zeroone.star.project.query.j6.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.time.LocalDateTime;

// 进货单 列表查询（extends PageQuery）（对应前端 PageInboundReceiptsRequest）
// 字段：sourceLocationId, purposeLocationId, statusEnum, keyword, applyTimeFrom, applyTimeTo, typeEnum
@Getter
@Setter
@ToString
@Schema(description = "进货单列表查询参数")
public class InboundReceiptQuery extends PageQuery {

    @Schema(description = "单据号（模糊搜索）")
    private String keyword;

    @Schema(description = "申请开始时间")
    private LocalDateTime applyTimeFrom;

    @Schema(description = "申请结束时间")
    private LocalDateTime applyTimeTo;

    @Schema(description = "库房ID（选择药库）")
    private String purposeLocationId;
}
