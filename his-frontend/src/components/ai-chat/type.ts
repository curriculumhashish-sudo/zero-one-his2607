/**
 * 对话消息的角色类型
 * - user: 用户输入
 * - assistant: AI/智能体输出
 * - system: 系统提示（例如切换智能体、清空对话等）
 */
export type Role = 'user' | 'assistant' | 'system'

/**
 * 对话消息数据模型
 */
export interface ChatMessage {
  /** 消息唯一标识 */
  id: string
  /** 消息角色 */
  role: Role
  /** 关联智能体 id（用于展示对应头像/图标） */
  agentId?: string
  /** 文本内容 */
  content: string
  /** 图片列表（用于多模态/附件能力） */
  images?: ChatImage[]
  /** 创建时间戳(ms) */
  createdAt: number
}

/**
 * 图片附件
 */
export interface ChatImage {
  /** 图片唯一标识 */
  id: string
  /** 图片名称 */
  name: string
  /** 图片大小（字节） */
  size: number
  /** 预览地址（通常为 blob: URL） */
  url: string
  /** 原始文件（仅本地选图时存在） */
  file?: File
}

/**
 * 智能体选项
 */
export interface AgentOption {
  /** 智能体唯一标识 */
  id: string
  /** 智能体显示名称 */
  name: string
  /** 智能体接口地址 */
  url?: string
  /** 智能体图标（服务端字符串，例如 emoji / 图片 URL） */
  icon?: string
  /** 可选描述 */
  desc?: string
}
