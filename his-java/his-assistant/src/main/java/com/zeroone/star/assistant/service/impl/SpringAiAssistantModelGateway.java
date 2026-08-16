package com.zeroone.star.assistant.service.impl;

import com.zeroone.star.assistant.service.AssistantModelGateway;
import org.springframework.ai.chat.client.ChatClient;
import org.springframework.ai.chat.client.advisor.MessageChatMemoryAdvisor;
import org.springframework.ai.chat.memory.ChatMemory;
import org.springframework.ai.chat.model.ChatModel;
import org.springframework.ai.tool.ToolCallbackProvider;
import org.springframework.beans.factory.ObjectProvider;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import java.util.Map;

/**
 * 基于 Spring AI ChatClient 和 MCP 工具回调的模型网关。
 */
@Component
public class SpringAiAssistantModelGateway implements AssistantModelGateway {

    private final ChatClient routingClient;
    private final ChatClient assistantClient;
    private final ChatMemory chatMemory;

    public SpringAiAssistantModelGateway(ChatModel chatModel,
                                         ChatMemory chatMemory,
                                         ObjectProvider<ToolCallbackProvider> toolCallbackProviders) {
        this.chatMemory = chatMemory;
        this.routingClient = ChatClient.builder(chatModel).build();

        ChatClient.Builder assistantBuilder = ChatClient.builder(chatModel)
                .defaultAdvisors(MessageChatMemoryAdvisor.builder(chatMemory).build());
        ToolCallbackProvider[] providers = toolCallbackProviders.orderedStream()
                .toArray(ToolCallbackProvider[]::new);
        if (providers.length > 0) {
            assistantBuilder.defaultToolCallbacks(providers);
        }
        this.assistantClient = assistantBuilder.build();
    }

    @Override
    public String classify(String systemPrompt, String userPrompt) {
        String content = routingClient.prompt()
                .system(systemPrompt)
                .user(userPrompt)
                .call()
                .content();
        return requireContent(content);
    }

    @Override
    public String complete(String conversationId, String systemPrompt, String userPrompt,
                           Map<String, Object> toolContext) {
        ChatClient.ChatClientRequestSpec request = assistantClient.prompt()
                .system(systemPrompt)
                .user(userPrompt)
                .advisors(advisor -> advisor.param(ChatMemory.CONVERSATION_ID, conversationId));
        if (toolContext != null && !toolContext.isEmpty()) {
            request.toolContext(toolContext);
        }
        return requireContent(request.call().content());
    }

    @Override
    public void clearConversation(String conversationId) {
        chatMemory.clear(conversationId);
    }

    private String requireContent(String content) {
        if (!StringUtils.hasText(content)) {
            throw new IllegalStateException("模型未返回有效内容");
        }
        return content.trim();
    }
}
