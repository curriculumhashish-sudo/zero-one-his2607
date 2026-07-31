package com.zeroone.star.project.dto.notify;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：示例通知数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "示例通知数据对象")
public class SampleNotifyDTO {
    @Schema(description = "客户端编号", example = "1")
    private String clientId;
    @Schema(description = "通知消息内容", example = "这是通知消息")
    private String message;
}
