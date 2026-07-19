package com.zeroone.star.project.query.j6.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.time.LocalDateTime;

/**
 * 进货单列表查询对象。
 */
@Getter
@Setter
@ToString
@Schema(name = "InboundReceiptQuery", description = "药库进货单列表查询条件")
public class InboundReceiptQuery extends PageQuery {
    @Schema(description = "来源位置标识")
    private String sourceLocationId;

    @Schema(description = "目的库房标识")
    private String purposeLocationId;

    @Schema(description = "供应状态")
    private String statusEnum;

    @Schema(description = "关键字，支持单号/供应商/项目名称模糊查询")
    private String keyword;

    @Schema(description = "申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "供应类型", example = "INBOUND")
    private String typeEnum;
}
