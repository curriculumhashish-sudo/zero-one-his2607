package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：入库单全量数据传输对象
 * </p>
 * <p>
 * 功能说明：用于保存入库单、保存并提交审批接口的请求参数，同时也是新增入库单的返回响应。
 * 包含完整的单据头信息（状态、制单人、审批信息、总金额等）及明细列表。
 * 前端需要回传服务端之前返回的所有字段以保证数据一致性。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@Schema(description = "入库单全量信息DTO（保存/提交审批用）")
public class InStockOrderSaveDTO extends InStockOrderCreateDTO {

    @Schema(description = "单据ID（保存时必填，新增时不传）", example = "330265145864359936")
    private String id;

    @Schema(description = "单据状态（PENDING_APPROVAL-待审批 APPROVAL-审批中 AGREE-已通过 WITHDRAW-已撤回）", example = "AGREE")
    @Size(max = 32, message = "单据状态长度不能超过32个字符")
    private String statusEnum;

    @Schema(description = "制单人ID", example = "308791351298756608")
    private String createdBy;

    @Schema(description = "制单时间", example = "2026-06-30 16:35:46")
    private LocalDateTime createdAt;

    @Schema(description = "审批人ID", example = "308791351298756608")
    private String approverId;

    @Schema(description = "审批时间", example = "2026-06-30 16:35:46")
    private LocalDateTime approvalTime;

    @Schema(description = "整单总金额（由后端汇总）", example = "55.92")
    private BigDecimal totalPrice;
}