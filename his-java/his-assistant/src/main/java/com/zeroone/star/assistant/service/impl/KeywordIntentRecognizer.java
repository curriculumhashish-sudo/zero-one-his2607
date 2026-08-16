package com.zeroone.star.assistant.service.impl;

import com.zeroone.star.assistant.model.AssistantDomain;
import com.zeroone.star.assistant.model.IntentDecision;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;

/**
 * 可预测、无外部依赖的快速意图识别器，也是模型路由失败时的兜底。
 */
@Component
public class KeywordIntentRecognizer {

    public IntentDecision recognize(String message) {
        if (message == null || message.isBlank()) {
            return IntentDecision.unknown();
        }

        String normalized = message.toLowerCase(Locale.ROOT);
        List<DomainMatch> matches = new ArrayList<>();
        for (AssistantDomain domain : AssistantDomain.values()) {
            int firstIndex = Integer.MAX_VALUE;
            int hitCount = 0;
            for (String keyword : domain.getKeywords()) {
                int index = normalized.indexOf(keyword.toLowerCase(Locale.ROOT));
                if (index >= 0) {
                    firstIndex = Math.min(firstIndex, index);
                    hitCount++;
                }
            }
            if (hitCount > 0) {
                matches.add(new DomainMatch(domain, firstIndex, hitCount));
            }
        }

        if (matches.isEmpty()) {
            return IntentDecision.unknown();
        }

        matches.sort(Comparator.comparingInt(DomainMatch::firstIndex)
                .thenComparing(Comparator.comparingInt(DomainMatch::hitCount).reversed()));
        List<AssistantDomain> domains = matches.stream().map(DomainMatch::domain).toList();
        double confidence = domains.size() > 1 ? 0.95D : Math.min(0.97D, 0.82D + matches.get(0).hitCount() * 0.05D);
        return new IntentDecision(domains, confidence, false);
    }

    private record DomainMatch(AssistantDomain domain, int firstIndex, int hitCount) {
    }
}
