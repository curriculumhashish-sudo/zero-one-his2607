package com.zeroone.star.project.vo.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 盘点单分页响应包装对象。
 *
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单分页响应")
public class StocktakePageJsonVO {

    @Schema(description = "状态码", example = "10000", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer code;

    @Schema(description = "提示消息", example = "操作成功", requiredMode = Schema.RequiredMode.REQUIRED)
    private String message;

    @Schema(description = "盘点单分页数据")
    private StocktakePageVO data;
}
