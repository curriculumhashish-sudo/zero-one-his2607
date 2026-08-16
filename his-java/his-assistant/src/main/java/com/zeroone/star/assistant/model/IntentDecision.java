package com.zeroone.star.assistant.model;

import java.util.List;

/**
 * 意图识别结果。
 *
 * @param domains 需要执行的业务域，顺序即调度顺序
 * @param confidence 置信度
 * @param needsClarification 是否需要引导用户澄清
 */
public record IntentDecision(List<AssistantDomain> domains, double confidence, boolean needsClarification) {

    public IntentDecision {
        domains = domains == null ? List.of() : List.copyOf(domains);
    }

    public static IntentDecision unknown() {
        return new IntentDecision(List.of(), 0D, true);
    }

    public boolean isUnknown() {
        return domains.isEmpty() || needsClarification;
    }

    public boolean isComposite() {
        return domains.size() > 1;
    }
}
