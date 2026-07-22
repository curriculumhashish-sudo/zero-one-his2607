package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 药房管理-药房入库单-获取单据列表响应对象
 */
@Data
@Schema(description = "入库单列表项")
public class PurchaseInboundListVO {

    @Schema(description = "主键ID", example = "1")
    private String id;

    @Schema(description = "单据编号", example = "RK20260721001")
    private String busNo;

    @Schema(description = "供应商名称 (需关联 cus_supplier 表，此处仅做展示字段)", example = "某某药业有限公司")
    private String supplierName;

    @Schema(description = "总金额 (对应 wkf_supply_request.total_price)", example = "1500.00")
    private BigDecimal totalPrice;

    @Schema(description = "单据状态 (对应 wkf_supply_request.status_enum)", example = "PENDING_APPROVAL")
    private String status;

    @Schema(description = "申请人 (对应 wkf_supply_request.applicant_id)", example = "张三")
    private String applicantName;

    @Schema(description = "申请时间 (对应 wkf_supply_request.apply_time)")
    private LocalDateTime applyTime;
}