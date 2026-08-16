package com.zeroone.star.assistant.service.impl;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.assistant.model.AssistantDomain;
import com.zeroone.star.assistant.model.IntentDecision;
import com.zeroone.star.assistant.service.AssistantModelGateway;
import com.zeroone.star.assistant.service.IntentRecognizer;
import com.zeroone.star.project.dto.assistant.GeneralAssistantChatDTO;
import com.zeroone.star.project.vo.assistant.GeneralAssistantChatVO;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class GeneralAssistantServiceImplTest {

    @Test
    void shouldReturnDeterministicGuidanceWithoutCallingModelForUnknownIntent() {
        RecordingModelGateway gateway = new RecordingModelGateway();
        GeneralAssistantServiceImpl service = service(message -> IntentDecision.unknown(), gateway);

        GeneralAssistantChatVO result = service.chat(request("conv-1", "帮我处理一下"));

        assertEquals("unknown", result.getIntent());
        assertFalse(result.getDegraded());
        assertTrue(result.getAnswer().contains("请补充"));
        assertTrue(gateway.prompts.isEmpty());
    }

    @Test
    void shouldExecuteCompositeAssistantsInOrderAndPassPreviousResultForward() {
        RecordingModelGateway gateway = new RecordingModelGateway();
        IntentRecognizer recognizer = message -> new IntentDecision(
                List.of(AssistantDomain.KNOWLEDGE, AssistantDomain.SCHEDULING), 0.95D, false);
        GeneralAssistantServiceImpl service = service(recognizer, gateway);

        GeneralAssistantChatVO result = service.chat(request(
                "conv-2", "查询制度后据此生成排班初稿"));

        assertTrue(result.getComposite());
        assertFalse(result.getDegraded());
        assertEquals("knowledge,scheduling", result.getIntent());
        assertEquals(List.of("knowledge", "scheduling"),
                result.getRoutes().stream().map(route -> route.getCode()).toList());
        assertEquals(3, gateway.prompts.size());
        assertTrue(gateway.prompts.get(1).contains("result-1"));
        assertEquals("result-3", result.getAnswer());
    }

    @Test
    void shouldKeepVerifiedPartialResultsWhenOneCompositeStepFails() {
        RecordingModelGateway gateway = new RecordingModelGateway();
        gateway.failOnCall = 2;
        IntentRecognizer recognizer = message -> new IntentDecision(
                List.of(AssistantDomain.KNOWLEDGE, AssistantDomain.SCHEDULING), 0.95D, false);
        GeneralAssistantServiceImpl service = service(recognizer, gateway);

        GeneralAssistantChatVO result = service.chat(request("conv-3", "组合任务"));

        assertTrue(result.getDegraded());
        assertEquals("FAILED", result.getRoutes().get(1).getStatus());
        assertEquals("result-3", result.getAnswer());
    }

    @Test
    void shouldClearMainAndAllRoutedConversations() {
        RecordingModelGateway gateway = new RecordingModelGateway();
        GeneralAssistantServiceImpl service = service(message -> IntentDecision.unknown(), gateway);

        service.clearConversation("conv-4");

        assertEquals(5, gateway.clearedConversationIds.size());
        assertTrue(gateway.clearedConversationIds.contains("conv-4"));
        assertTrue(gateway.clearedConversationIds.contains("conv-4:route:registration"));
    }

    private GeneralAssistantServiceImpl service(IntentRecognizer recognizer, AssistantModelGateway gateway) {
        return new GeneralAssistantServiceImpl(recognizer, gateway, new ObjectMapper(), 20, 500);
    }

    private GeneralAssistantChatDTO request(String conversationId, String message) {
        GeneralAssistantChatDTO dto = new GeneralAssistantChatDTO();
        dto.setConversationId(conversationId);
        dto.setMessage(message);
        dto.setContext(Map.of("operatorId", "1001"));
        return dto;
    }

    private static final class RecordingModelGateway implements AssistantModelGateway {
        private final List<String> prompts = new ArrayList<>();
        private final List<String> clearedConversationIds = new ArrayList<>();
        private int calls;
        private int failOnCall = -1;

        @Override
        public String classify(String systemPrompt, String userPrompt) {
            throw new UnsupportedOperationException();
        }

        @Override
        public String complete(String conversationId, String systemPrompt, String userPrompt,
                               Map<String, Object> toolContext) {
            calls++;
            if (calls == failOnCall) {
                throw new IllegalStateException("simulated downstream failure");
            }
            prompts.add(userPrompt);
            return "result-" + calls;
        }

        @Override
        public void clearConversation(String conversationId) {
            clearedConversationIds.add(conversationId);
        }
    }
}
