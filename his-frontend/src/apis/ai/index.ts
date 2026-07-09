import { DataUpType, useHttp } from '@/plugins/http'
import type { AiAgentVO, AiChatReplyVO, AiChatRequestDTO } from './type'
import { useUserStore } from '@/stores/user'

/** AI 模块基础URL */
const baseURL = '/ai'
/** 智能体列表接口地址 */
const agentsListUrl = '/ai/agents'

/**
 * 获取智能体列表
 */
export const getAgents = async () => {
  const http = useHttp()
  const res = await http.get<AiAgentVO[]>(agentsListUrl, undefined, {
    baseURL,
    timeout: 100000
  })
  return Array.isArray(res?.data) ? res.data : []
}

/**
 * 发送 AI 对话请求
 * @param url 智能体对话接口地址
 * @param text 用户输入文本
 */
export const requestAiReply = async (url: string, text: string) => {
  const http = useHttp()
  const userStore = useUserStore()
  const data: AiChatRequestDTO = {
    message: text,
    userId: userStore.getUser?.id || ''
  }
  const res = await http.post<string | AiChatReplyVO>(url, data, {
    upType: DataUpType.form,
    baseURL,
    timeout: 100000
  })
  return res?.data
}
