package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;


/**
 * 药房管理-药房入库单-提交审批请求对象
 */
@Data
@Schema(description = "提交审批请求对象")
public class PurchaseInboundSubmitDTO {

    @Schema(description = "入库单ID", example = "1", required = true)
    private String id;

}