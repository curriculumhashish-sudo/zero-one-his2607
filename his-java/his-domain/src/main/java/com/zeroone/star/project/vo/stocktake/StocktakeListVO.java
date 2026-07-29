package com.zeroone.star.project.vo.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：盘点单列表显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单列表项")
public class StocktakeListVO {

    @Schema(description = "盘点单ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务单号", example = "PD20260701001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "单据状态：DRAFT-草稿，PENDING_APPROVAL-待审批，APPROVED-已审批，REJECTED-已驳回，WITHDRAWN-已撤回", example = "DRAFT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "库房名称", example = "门诊药房")
    private String warehouseName;

    @Schema(description = "盘点盈亏金额（盘盈为正，盘亏为负）", example = "1250.50")
    private BigDecimal profitLossPrice;

    @Schema(description = "申请人名称", example = "张三")
    private String applicantName;

    @Schema(description = "申请时间", example = "2026-07-01 10:30:00")
    private LocalDateTime applyTime;

    @Schema(description = "审批人名称", example = "李四")
    private String approverName;

    @Schema(description = "审批时间", example = "2026-07-01 14:00:00")
    private LocalDateTime approvalTime;
}
