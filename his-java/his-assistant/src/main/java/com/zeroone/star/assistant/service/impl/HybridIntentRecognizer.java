package com.zeroone.star.assistant.service.impl;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.assistant.model.AssistantDomain;
import com.zeroone.star.assistant.model.IntentDecision;
import com.zeroone.star.assistant.service.AssistantModelGateway;
import com.zeroone.star.assistant.service.IntentRecognizer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

/**
 * 规则优先、模型兜底的混合意图识别器。
 */
@Service
public class HybridIntentRecognizer implements IntentRecognizer {

    private static final Logger LOGGER = LoggerFactory.getLogger(HybridIntentRecognizer.class);

    private static final String ROUTING_SYSTEM_PROMPT = """
            你是零壹 HIS 通用助手的意图路由器。判断用户任务应交给哪些垂直助手，并按实际执行顺序排列。
            仅允许以下编码：
            - registration：挂号、分诊、科室医生与号源
            - operations：运营报表、指标、DRG/DIP、医保风险
            - scheduling：排班、调班、冲突检测、替班
            - knowledge：院内制度、药品、指南、术语和编码
            若任务跨域，返回多个编码；若只是寒暄、信息不足或不属于上述能力，needsClarification=true 且 domains=[]。
            只输出 JSON，不要输出 Markdown 或解释。格式：
            {"domains":["registration"],"confidence":0.92,"needsClarification":false}
            """;

    private final KeywordIntentRecognizer keywordRecognizer;
    private final AssistantModelGateway modelGateway;
    private final ObjectMapper objectMapper;
    private final double routingConfidence;

    public HybridIntentRecognizer(KeywordIntentRecognizer keywordRecognizer,
                                  AssistantModelGateway modelGateway,
                                  ObjectMapper objectMapper,
                                  @Value("${his.assistant.routing-confidence:0.55}") double routingConfidence) {
        this.keywordRecognizer = keywordRecognizer;
        this.modelGateway = modelGateway;
        this.objectMapper = objectMapper;
        this.routingConfidence = routingConfidence;
    }

    @Override
    public IntentDecision recognize(String message) {
        IntentDecision keywordDecision = keywordRecognizer.recognize(message);
        if (!keywordDecision.isUnknown()) {
            return keywordDecision;
        }

        try {
            String content = modelGateway.classify(ROUTING_SYSTEM_PROMPT, message);
            JsonNode root = objectMapper.readTree(extractJson(content));
            double confidence = root.path("confidence").asDouble(0D);
            boolean needsClarification = root.path("needsClarification").asBoolean(false);
            Set<AssistantDomain> domains = new LinkedHashSet<>();
            JsonNode domainNodes = root.path("domains");
            if (domainNodes.isArray()) {
                for (JsonNode node : domainNodes) {
                    AssistantDomain.fromCode(node.asText()).ifPresent(domains::add);
                }
            }
            if (confidence < routingConfidence || needsClarification || domains.isEmpty()) {
                return IntentDecision.unknown();
            }
            return new IntentDecision(new ArrayList<>(domains), confidence, false);
        } catch (RuntimeException | java.io.IOException ex) {
            LOGGER.warn("大模型意图路由失败，使用引导兜底：{}", ex.getMessage());
            return IntentDecision.unknown();
        }
    }

    private String extractJson(String content) {
        if (content == null) {
            throw new IllegalArgumentException("意图路由响应为空");
        }
        int start = content.indexOf('{');
        int end = content.lastIndexOf('}');
        if (start < 0 || end < start) {
            throw new IllegalArgumentException("意图路由响应不是有效 JSON");
        }
        return content.substring(start, end + 1);
    }
}
