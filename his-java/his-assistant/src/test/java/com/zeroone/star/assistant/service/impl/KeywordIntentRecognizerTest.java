package com.zeroone.star.assistant.service.impl;

import com.zeroone.star.assistant.model.AssistantDomain;
import com.zeroone.star.assistant.model.IntentDecision;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class KeywordIntentRecognizerTest {

    private final KeywordIntentRecognizer recognizer = new KeywordIntentRecognizer();

    @Test
    void shouldRecognizeSingleRegistrationIntent() {
        IntentDecision decision = recognizer.recognize("帮我查询今天呼吸科还有余号的医生");

        assertEquals(List.of(AssistantDomain.REGISTRATION), decision.domains());
        assertFalse(decision.isComposite());
        assertFalse(decision.isUnknown());
    }

    @Test
    void shouldRecognizeOperationsIntentWithoutBeingConfusedByOutpatientWording() {
        IntentDecision decision = recognizer.recognize("统计上月各科室门诊量及同比");

        assertEquals(List.of(AssistantDomain.OPERATIONS), decision.domains());
    }

    @Test
    void shouldPreserveBusinessOrderForCompositeIntent() {
        IntentDecision decision = recognizer.recognize("先查询院内抗菌药物制度，再生成下月呼吸科排班初稿");

        assertEquals(List.of(AssistantDomain.KNOWLEDGE, AssistantDomain.SCHEDULING), decision.domains());
        assertTrue(decision.isComposite());
    }

    @Test
    void shouldReturnUnknownForUnsupportedConversation() {
        assertTrue(recognizer.recognize("你好，今天心情不错").isUnknown());
    }
}
