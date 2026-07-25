package com.zeroone.star.project.query.stocktake;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：盘点单查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "盘点单查询条件")
public class StocktakeQuery extends PageQuery {

    @Schema(description = "业务单号", example = "PD20260701001")
    private String busNo;

    @Schema(description = "单据状态：DRAFT-草稿，SUBMITTED-已提交，APPROVED-已审批，REJECTED-已驳回", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "库房标识", example = "1")
    private String warehouseId;

    @Schema(description = "申请人标识", example = "1")
    private String applicantId;

    @Schema(description = "开始日期", example = "2026-07-01 00:00:00")
    private LocalDateTime startTime;

    @Schema(description = "结束日期", example = "2026-07-31 23:59:59")
    private LocalDateTime endTime;
}
