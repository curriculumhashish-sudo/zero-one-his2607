package com.zeroone.star.project.vo.assistant;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;
import java.util.List;

/**
 * 通用助手会话响应。
 */
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "通用助手会话响应")
public class GeneralAssistantChatVO {

    @Schema(description = "会话ID")
    private String conversationId;

    @Schema(description = "识别到的意图；多意图时使用逗号分隔", example = "registration,knowledge")
    private String intent;

    @Schema(description = "是否执行了多个垂直助手")
    private Boolean composite;

    @Schema(description = "是否因模型或工具暂时不可用而降级")
    private Boolean degraded;

    @Schema(description = "助手答复")
    private String answer;

    @Schema(description = "路由执行轨迹")
    private List<AssistantRouteVO> routes;

    @Schema(description = "下一步可选提问")
    private List<String> suggestions;

    @Schema(description = "响应时间")
    private LocalDateTime timestamp;
}
