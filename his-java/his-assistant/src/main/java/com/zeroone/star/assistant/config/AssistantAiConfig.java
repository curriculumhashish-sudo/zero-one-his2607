package com.zeroone.star.assistant.config;

import org.springframework.ai.chat.memory.ChatMemory;
import org.springframework.ai.chat.memory.MessageWindowChatMemory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * 通用助手会话记忆配置。
 */
@Configuration
public class AssistantAiConfig {

    @Bean
    public ChatMemory assistantChatMemory() {
        return MessageWindowChatMemory.builder()
                .maxMessages(20)
                .build();
    }
}
