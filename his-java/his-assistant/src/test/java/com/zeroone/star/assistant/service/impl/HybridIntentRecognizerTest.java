package com.zeroone.star.assistant.service.impl;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.assistant.model.AssistantDomain;
import com.zeroone.star.assistant.model.IntentDecision;
import com.zeroone.star.assistant.service.AssistantModelGateway;
import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class HybridIntentRecognizerTest {

    @Test
    void shouldUseModelWhenKeywordRouterCannotDecide() {
        StubModelGateway gateway = new StubModelGateway(
                "```json\n{\"domains\":[\"operations\"],\"confidence\":0.91,\"needsClarification\":false}\n```");
        HybridIntentRecognizer recognizer = new HybridIntentRecognizer(
                new KeywordIntentRecognizer(), gateway, new ObjectMapper(), 0.55D);

        IntentDecision decision = recognizer.recognize("帮我看看医院最近经营得怎么样");

        assertEquals(List.of(AssistantDomain.OPERATIONS), decision.domains());
        assertEquals(1, gateway.classifyCalls);
    }

    @Test
    void shouldGuideUserWhenModelResultIsBelowThreshold() {
        StubModelGateway gateway = new StubModelGateway(
                "{\"domains\":[\"registration\"],\"confidence\":0.2,\"needsClarification\":false}");
        HybridIntentRecognizer recognizer = new HybridIntentRecognizer(
                new KeywordIntentRecognizer(), gateway, new ObjectMapper(), 0.55D);

        assertTrue(recognizer.recognize("帮我处理一下").isUnknown());
    }

    private static final class StubModelGateway implements AssistantModelGateway {
        private final String classification;
        private int classifyCalls;

        private StubModelGateway(String classification) {
            this.classification = classification;
        }

        @Override
        public String classify(String systemPrompt, String userPrompt) {
            classifyCalls++;
            return classification;
        }

        @Override
        public String complete(String conversationId, String systemPrompt, String userPrompt,
                               Map<String, Object> toolContext) {
            throw new UnsupportedOperationException();
        }

        @Override
        public void clearConversation(String conversationId) {
        }
    }
}
