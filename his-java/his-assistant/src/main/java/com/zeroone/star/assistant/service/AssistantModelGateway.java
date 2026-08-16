package com.zeroone.star.assistant.service;

import java.util.Map;

/**
 * 隔离 Spring AI 调用细节，便于编排逻辑测试和模型替换。
 */
public interface AssistantModelGateway {

    String classify(String systemPrompt, String userPrompt);

    String complete(String conversationId, String systemPrompt, String userPrompt, Map<String, Object> toolContext);

    void clearConversation(String conversationId);
}
