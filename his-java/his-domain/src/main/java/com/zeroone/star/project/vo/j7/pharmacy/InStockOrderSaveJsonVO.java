package com.zeroone.star.project.vo.j7.pharmacy;

import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderSaveDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 入库单保存响应包装对象。
 *
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "入库单保存响应")
public class InStockOrderSaveJsonVO {

    @Schema(description = "状态码", example = "10000", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer code;

    @Schema(description = "提示消息", example = "操作成功", requiredMode = Schema.RequiredMode.REQUIRED)
    private String message;

    @Schema(description = "入库单全量信息")
    private InStockOrderSaveDTO data;
}
