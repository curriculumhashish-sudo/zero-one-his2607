package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 入库单审批流转响应包装对象。
 *
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "入库单审批流转响应")
public class InboundApprovalJsonVO {

    @Schema(description = "状态码", example = "10000", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer code;

    @Schema(description = "提示消息", example = "操作成功", requiredMode = Schema.RequiredMode.REQUIRED)
    private String message;

    @Schema(description = "入库单审批流转结果")
    private InboundApprovalVO data;
}
