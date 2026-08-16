import type { AgentOption } from '@/components/ai-chat/type'

/**
 * 智能体数据模型
 */
export interface AiAgentVO extends AgentOption {}

/**
 * 通用助手能力
 */
export interface AssistantCapabilityVO {
  code: string
  name: string
  description: string
  examples: string[]
}

/** 通用助手对话请求参数 */
export interface AiChatRequestDTO {
  conversationId?: string
  message: string
  context?: Record<string, unknown>
}

/**
 * 通用助手对话响应对象
 */
export interface AiChatReplyVO extends Record<string, any> {
  conversationId?: string
  intent?: string
  composite?: boolean
  degraded?: boolean
  answer?: string
  suggestions?: string[]
  timestamp?: string
}
