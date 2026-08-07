package com.zeroone.star.project.vo.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 药库进货单详情 — 新增返回 / 列表详情查询共用。
 */
@Data
@Schema(name = "InboundReceiptDetailVO", description = "药库进货单详情（新增返回 + 详情查询）")
public class InboundReceiptDetailVO {

    @Schema(description = "业务单号", example = "CIR202608070001")
    private String busNo;

    @Schema(description = "来源单号", example = "DHD202607180001")
    private String originalBusNo;

    @Schema(description = "供应类型编码", example = "CABINET_INBOUND_RECEIPT")
    private String typeEnum;

    @Schema(description = "供应状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "供应商ID", example = "4100000000000000004")
    private String supplierId;

    @Schema(description = "供应商名称", example = "华中药品配送有限公司")
    private String supplierName;

    @Schema(description = "出库方库房ID")
    private String sourceLocationId;

    @Schema(description = "入库方库房ID", example = "322954885380706304")
    private String purposeLocationId;

    @Schema(description = "库房名称", example = "西药库")
    private String purposeLocationName;

    @Schema(description = "出库方类型", example = "SUPPLIER")
    private String sourceTypeEnum;

    @Schema(description = "入库方类型", example = "WAREHOUSE")
    private String purposeTypeEnum;

    @Schema(description = "供应类别编码", example = "PURCHASE")
    private String categoryEnum;

    @Schema(description = "乐观锁版本号", example = "0")
    private Integer rowVersion;

    @Schema(description = "申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称", example = "库管")
    private String applicantName;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称", example = "admin")
    private String approverName;

    @Schema(description = "制单人ID", example = "322932096108597248")
    private String creatorId;

    @Schema(description = "制单人名称", example = "库管")
    private String creatorName;

    @Schema(description = "经办人ID", example = "322932096108597248")
    private String practitionerId;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "原因/备注", example = "按药库订货单到货入库")
    private String reason;

    @Schema(description = "单据总金额", example = "1090.00")
    private BigDecimal totalAmount;

    @Schema(description = "明细行列表")
    private List<InboundReceiptDetailLineVO> lines;
}
