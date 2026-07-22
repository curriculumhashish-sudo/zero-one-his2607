package com.zeroone.star.project.query.j7.stocktake;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：盘点单列表/详情查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "盘点单查询条件")
public class StocktakeDocumentQuery extends PageQuery {

    @Schema(description = "盘点单业务单号", example = "PD202607230001")
    private String busNo;

    @Schema(description = "盘点库房/药房位置ID", example = "1001")
    private String purposeLocationId;

    @Schema(description = "单据状态：DRAFT-草稿，PENDING_APPROVAL-待审批，APPROVAL-审批中，AGREE-已同意，REJECT-已驳回", example = "PENDING_APPROVAL")
    private String statusEnum;

    @Schema(description = "申请开始日期", example = "2026-07-01")
    private String applyStartDate;

    @Schema(description = "申请结束日期", example = "2026-07-31")
    private String applyEndDate;

    @Schema(description = "关键字，支持单号/药品名称/批号模糊查询", example = "阿莫西林")
    private String keyword;
}
