package com.zeroone.star.project.dto.assistant;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.util.Map;

/**
 * <p>
 * 描述：通用助手会话请求
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Data
@Schema(description = "通用助手会话请求")
public class GeneralAssistantChatDTO {

    @Size(max = 64, message = "会话ID长度不能超过64个字符")
    @Pattern(regexp = "^[A-Za-z0-9_-]+$", message = "会话ID格式不正确")
    @Schema(description = "会话ID；首次请求可不传，由服务端生成", example = "a51e2f7c-13ce-4d45-a571-7e00ed9edb55")
    private String conversationId;

    @NotBlank(message = "消息内容不能为空")
    @Size(max = 2000, message = "消息内容不能超过2000个字符")
    @Schema(description = "用户问题或任务", requiredMode = Schema.RequiredMode.REQUIRED,
            example = "先查呼吸科今天有余号的医生，再推荐最合适的号源")
    private String message;

    @Size(max = 20, message = "业务上下文不能超过20项")
    @Schema(description = "可选业务上下文，例如患者、科室、时间范围等；不要传入密码或密钥")
    private Map<String, Object> context;
}
