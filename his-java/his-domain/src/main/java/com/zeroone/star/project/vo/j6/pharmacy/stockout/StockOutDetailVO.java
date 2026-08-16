package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 药库出库单详情 — 新增返回 / 详情查询共用。
 */
@Data
@Schema(name = "StockOutDetailVO", description = "药库出库单详情（新增返回 + 详情查询）")
public class StockOutDetailVO {

    @Schema(description = "业务单号", example = "CSO202608070001")
    private String busNo;

    @Schema(description = "关联请领单号", example = "QL202608070001")
    private String originalBusNo;

    @Schema(description = "出库单类型编码", example = "WAREHOUSE_STOCKOUT")
    private String typeEnum;

    @Schema(description = "供应状态编码", example = "PENDING_APPROVAL")
    private String statusEnum;

    @Schema(description = "供应类别编码", example = "STOCK_OUT")
    private String categoryEnum;

    @Schema(description = "源药库ID", example = "322954885380706304")
    private String sourceLocationId;

    @Schema(description = "源药库名称", example = "西药库")
    private String sourceLocationName;

    @Schema(description = "目的药房ID", example = "322954949549363200")
    private String purposeLocationId;

    @Schema(description = "目的药房名称", example = "门诊西药房")
    private String purposeLocationName;

    @Schema(description = "来源位置类型", example = "WAREHOUSE")
    private String sourceTypeEnum;

    @Schema(description = "目的位置类型", example = "PHARMACY")
    private String purposeTypeEnum;

    @Schema(description = "乐观锁版本号", example = "0")
    private Integer version;

    @Schema(description = "制单/申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "制单人员ID", example = "322932096108597248")
    private String practitionerId;

    @Schema(description = "制单人员名称", example = "库管")
    private String practitionerName;

    @Schema(description = "创建人ID")
    private String creatorId;

    @Schema(description = "创建人名称", example = "库管")
    private String creatorName;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称", example = "库管")
    private String applicantName;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称", example = "admin")
    private String approverName;

    @Schema(description = "备注", example = "门诊药房调拨出库")
    private String remark;

    @Schema(description = "单据总金额", example = "17147.00")
    private BigDecimal totalAmount;

    @Schema(description = "出库单明细行列表")
    private List<StockOutLineVO> lines;
}
