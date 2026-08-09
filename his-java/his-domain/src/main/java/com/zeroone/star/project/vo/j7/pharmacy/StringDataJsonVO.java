package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 字符串 data 响应包装对象。
 *
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "字符串数据响应")
public class StringDataJsonVO {

    @Schema(description = "状态码", example = "10000", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer code;

    @Schema(description = "提示消息", example = "操作成功", requiredMode = Schema.RequiredMode.REQUIRED)
    private String message;

    @Schema(description = "字符串数据", example = "保存入库单据成功")
    private String data;
}
