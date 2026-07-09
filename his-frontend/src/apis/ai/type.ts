import type { AgentOption } from '@/components/ai-chat/type'

/**
 * 智能体数据模型
 */
export interface AiAgentVO extends AgentOption {}

/**
 * AI 对话请求参数
 */
export interface AiChatRequestDTO {
  /** 用户输入内容 */
  message: string
  /** 当前用户标识 */
  userId: string
}

/**
 * AI 对话响应对象
 */
export interface AiChatReplyVO extends Record<string, any> {
  /** 回复内容 */
  reply?: string
  /** 兼容部分接口返回 content 字段 */
  content?: string
  /** 兼容部分接口返回 message 字段 */
  message?: string
}
