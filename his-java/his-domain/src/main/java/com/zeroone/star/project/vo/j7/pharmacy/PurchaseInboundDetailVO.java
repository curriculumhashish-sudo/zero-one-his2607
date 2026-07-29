package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 描述：药房入库单-获取单据详情响应对象
 * </p>
 * <p>
 * 功能说明：用于展示入库单的完整信息，包含单据头信息及多条药品/物资明细。
 * </p>
 */
@Getter
@Setter
@Schema(description = "入库单详情响应对象")
public class PurchaseInboundDetailVO {

    // --- 单据头信息 ---
    @Schema(description = "主键ID", example = "1")
    private String id;

    @Schema(description = "单据编号", example = "PSI2026-06-300001")
    private String busNo;

    @Schema(description = "供应商ID", example = "SUP_1001")
    private String supplierId;

    @Schema(description = "供应商名称 (需关联查询)", example = "某某药业有限公司")
    private String supplierName;

    @Schema(description = "目的库房ID (对应 purpose_location_id)", example = "WH_OUTPATIENT")
    private String purposeLocationId;

    @Schema(description = "目的库房名称", example = "门诊西药房")
    private String purposeLocationName;

    @Schema(description = "总金额", example = "1500.00")
    private BigDecimal totalPrice;

    @Schema(description = "单据状态", example = "PENDING_APPROVAL")
    private String statusEnum;

    @Schema(description = "申请时间", example = "2026-06-30T16:35:00")
    private LocalDateTime applyTime;

    @Schema(description = "备注", example = "紧急补货")
    private String remake;

    // --- 单据明细列表 ---
    @Schema(description = "入库明细列表")
    private List<PurchaseInboundItemVO> items;
}