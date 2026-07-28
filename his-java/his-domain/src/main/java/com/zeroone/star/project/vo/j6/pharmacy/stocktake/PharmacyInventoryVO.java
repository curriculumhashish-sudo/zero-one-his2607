package com.zeroone.star.project.vo.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：药房盘点单据列表行VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "药房盘点单据列表(条件+分页)")
public class PharmacyInventoryVO {
    @Schema(description = "盘点业务单号", example = "CST2026-07-190004")
    private String busNo;

    @Schema(description = "状态编码", example = "AGREE")
    private String statusEnum;

    @Schema(description = "状态名称", example = "同意")
    private String statusEnumText;

    @Schema(description = "目标库房ID")
    private String purposeLocationId;

    @Schema(description = "目标库房名称", example = "西药库")
    private String purposeLocationName;

    @Schema(description = "药品条目数量", example = "1")
    private Integer itemCount;

    @Schema(description = "盈亏总数量", example = "20.00")
    private BigDecimal profitLossQuantitySum;

    @Schema(description = "实际总金额", example = "6.99")
    private BigDecimal actualAmountSum;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称", example = "admin")
    private String applicantName;

    @Schema(description = "申请时间", example = "2026-07-19 21:06:20")
    private String applyTime;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称", example = "admin")
    private String approverName;

    @Schema(description = "审批时间", example = "2026-07-19 21:06:21")
    private String approvalTime;

    @Schema(description = "盘点发生时间", example = "2026-07-19 21:06:14")
    private String occurrenceTime;
}
