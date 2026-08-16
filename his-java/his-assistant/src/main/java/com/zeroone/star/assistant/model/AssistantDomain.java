package com.zeroone.star.assistant.model;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

/**
 * 通用助手支持的业务域及其垂直助手定义。
 */
public enum AssistantDomain {
    REGISTRATION(
            "registration",
            "挂号助手",
            "智能分诊、实时号源与医生推荐、专家专病匹配",
            List.of("我头痛发热应该挂什么科？", "查询呼吸科今天上午还有余号的医生"),
            List.of("挂号", "号源", "余号", "分诊", "预约", "换号", "退号", "专家号", "挂什么科", "看什么科", "推荐医生", "症状"),
            """
                    你是零壹 HIS 的挂号助手，服务收费工作站、患者移动端和门诊分诊台。
                    你的职责是：根据症状给出非诊断性的分诊方向；通过工具查询实时号源并推荐匹配医生；识别专病门诊和特色专家。
                    涉及科室、医生、号源、价格或时间的事实必须先调用 MCP 工具核验，不得编造。信息不足时先询问年龄、主要症状、持续时间等必要信息。
                    你不能替代医生诊断；发现胸痛、呼吸困难、意识障碍、大出血等急症线索时，优先建议立即急诊或呼叫急救。
                    """),
    OPERATIONS(
            "operations",
            "运营数据助手",
            "自然语言取数、DRG/DIP 模拟、异常指标归因、医保风险自查",
            List.of("统计上月各科室门诊量及同比", "分析本月耗材占比异常的原因"),
            List.of("运营", "报表", "统计", "同比", "环比", "门诊量", "收入", "成本", "耗材占比", "指标", "趋势", "drg", "dip", "cmi", "医保飞检", "医保风险"),
            """
                    你是零壹 HIS 的运营数据助手，面向院领导、医保办、财务科和报表中心。
                    你的职责是：自然语言取数与可视化建议；DRG/DIP 运营模拟；异常指标下钻归因；医保飞检风险自查。
                    任何指标、金额、比例、排名、预测基线都必须通过 MCP 工具读取真实数据，并明确统计口径和时间范围。不得生成或猜测 SQL，不得虚构数值。
                    用户没有给出机构、科室、时间或口径时，先提出最少量的澄清问题。预测和归因必须标注假设，不能表述为已证实事实。
                    """),
    SCHEDULING(
            "scheduling",
            "排班调度助手",
            "排班初稿、负荷均衡、冲突合规检测、突发替班推荐",
            List.of("生成下月呼吸科排班初稿", "李医生请假后推荐三名替班人选"),
            List.of("排班", "调班", "替班", "值班", "班次", "出诊安排", "轮班", "请假后", "负荷均衡", "连续工作"),
            """
                    你是零壹 HIS 的排班调度助手，服务门诊部、护理部和排班预约管理端。
                    你的职责是：生成排班初稿；结合历史负荷给出人员增减建议；检测劳动、职称、休假等冲突；推荐突发替班候选人。
                    医生资质、可用时间、历史工作量和现有排班必须通过 MCP 工具核验。不得绕过劳动规则、职称要求或审批流程，也不得直接承诺调班已生效。
                    在给出方案前确认排班周期、科室、岗位、硬性约束和偏好；最终方案应列出冲突、假设和仍需人工确认的事项。
                    """),
    KNOWLEDGE(
            "knowledge",
            "知识库助手",
            "院内 RAG 问答、药品说明书、指南共识、术语与编码查询",
            List.of("院内抗菌药物分级管理规定是什么？", "ICD-10 R11.2 代表什么？"),
            List.of("知识库", "规章", "制度", "流程", "sop", "说明书", "用药", "适应症", "不良反应", "禁忌", "指南", "共识", "icd", "编码", "术语", "定义", "临床路径"),
            """
                    你是零壹 HIS 的知识库助手，为基础数据、配置管理和临床工作站提供院内知识检索。
                    你的职责是：基于医院私有资料进行带出处的 RAG 问答；查询药品说明书及院内库存；检索指南共识；解释医学术语和标准编码。
                    必须优先调用 MCP 知识检索工具并给出资料名称、版本或年份和出处。找不到院内依据时要明确说明，不得杜撰制度条款、药品剂量、证据等级或库存状态。
                    医疗内容仅用于辅助，涉及个体化诊疗或用药调整时应建议由有资质的医务人员确认。
                    """);

    private final String code;
    private final String displayName;
    private final String description;
    private final List<String> examples;
    private final List<String> keywords;
    private final String systemPrompt;

    AssistantDomain(String code, String displayName, String description, List<String> examples,
                    List<String> keywords, String systemPrompt) {
        this.code = code;
        this.displayName = displayName;
        this.description = description;
        this.examples = examples;
        this.keywords = keywords;
        this.systemPrompt = systemPrompt;
    }

    public String getCode() {
        return code;
    }

    public String getDisplayName() {
        return displayName;
    }

    public String getDescription() {
        return description;
    }

    public List<String> getExamples() {
        return examples;
    }

    public List<String> getKeywords() {
        return keywords;
    }

    public String getSystemPrompt() {
        return systemPrompt;
    }

    public static Optional<AssistantDomain> fromCode(String code) {
        if (code == null) {
            return Optional.empty();
        }
        return Arrays.stream(values())
                .filter(domain -> domain.code.equalsIgnoreCase(code.trim()))
                .findFirst();
    }
}
