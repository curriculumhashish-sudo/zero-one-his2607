package com.zeroone.star.assistant.service;

import com.zeroone.star.assistant.model.IntentDecision;

/**
 * 通用助手意图识别器。
 */
public interface IntentRecognizer {

    IntentDecision recognize(String message);
}
