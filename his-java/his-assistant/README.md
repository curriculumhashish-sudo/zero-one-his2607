# HIS 通用助手

`his-assistant` 是智能助手总控服务，负责识别挂号、运营数据、排班调度和知识库意图，路由到对应的垂直助手，并在复杂任务中组合多个助手的结果。

## 启动配置

在 Nacos 的 `system.yaml` 中添加：

```yaml
sn:
  assistant: HIS_ASSISTANT
sp:
  assistant: 10003
```

运行环境还需要提供 `AI_DASHSCOPE_API_KEY`，并为 Spring AI Alibaba 分布式 MCP 客户端配置与 `his-mcp-gateway` 一致的 Nacos 命名空间、地址和凭证。

## 接口

- `POST /assistant/general-assistant/chat`：发送任务并获得路由后的回答。
- `GET /assistant/general-assistant/list-capabilities`：查询可用的垂直助手能力。
- `DELETE /assistant/general-assistant/clear-conversation/{conversationId}`：清除会话上下文。

实时号源、运营指标、排班和院内知识等事实必须通过 MCP 工具读取；模型不可用或工具调用失败时，响应中的 `degraded` 会标记为 `true`。
