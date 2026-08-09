package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 药房管理-历史入库单-分页响应包装对象。
 * <p>
 * 仅用于 OpenAPI/Knife4j 展示具体 data 结构，实际接口 JSON 结构仍为 JsonVO。
 * </p>
 *
 * @author 北海
 * @version 1.0.0
 */
@Schema(description = "历史入库单分页响应")
@Data
public class PurchaseInboundPageJsonVO {

    @Schema(description = "状态码", example = "10000", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer code;

    @Schema(description = "提示消息", example = "操作成功", requiredMode = Schema.RequiredMode.REQUIRED)
    private String message;

    @Schema(description = "历史入库单分页数据")
    private PurchaseInboundPageVO data;
}
