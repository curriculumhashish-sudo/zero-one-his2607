package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 药房管理-历史入库单-列表响应对象
 * 对应图片：历史入库单列表项
 */
@Data
@Schema(description = "历史入库单列表项")
public class PurchaseInboundListVO {

    @Schema(description = "主键ID", example = "1")
    private String id;

    @Schema(description = "单据编号", example = "PSI2026-06-300001")
    private String busNo;

    @Schema(description = "单据状态 (中文展示: 同意/待审核)", example = "同意")
    private String statusName;

    @Schema(description = "制单人姓名 (对应 wkf_supply_request.created_by)", example = "张三")
    private String createByName;

    @Schema(description = "审批人姓名 (对应 wkf_supply_request.approver_id)", example = "李四")
    private String approverName;

    @Schema(description = "申请/制单时间 (对应 wkf_supply_request.apply_time)", example = "2026-06-30 16:35")
    private LocalDateTime applyTime;

    @Schema(description = "流转路径描述 (如: 西药库 → 门诊西药房)", example = "西药库 → 门诊西药房")
    private String transferPath;

    @Schema(description = "总金额 (对应 wkf_supply_request.total_price)", example = "1500.00")
    private BigDecimal totalPrice;
}