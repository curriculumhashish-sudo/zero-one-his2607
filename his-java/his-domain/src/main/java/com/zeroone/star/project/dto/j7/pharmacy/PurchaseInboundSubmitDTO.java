package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.util.List;

/**
 * 药房管理-药房入库单-提交审批请求对象
 */
@Data
@Schema(description = "提交审批请求对象")
public class PurchaseInboundSubmitDTO {

    @Schema(description = "入库单ID (若为纯状态流转接口则必填)", example = "1")
    private String id;

    @Schema(description = "入库单明细列表 (若为保存并提交接口则必填)")
    private List<PurchaseInboundItemDTO> items;

    @Schema(description = "备注信息", example = "紧急补货")
    private String remark;
}