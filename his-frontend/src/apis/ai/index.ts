import { useHttp } from '@/plugins/http'
import type {
  AiAgentVO,
  AiChatReplyVO,
  AiChatRequestDTO,
  AssistantCapabilityVO
} from './type'
import { useUserStore } from '@/stores/user'

/** 本地开发时由 Vite 代理到 his-assistant。 */
const baseURL = '/assistant'
const capabilitiesUrl = '/general-assistant/list-capabilities'
const chatUrl = '/general-assistant/chat'

/**
 * 获取智能体列表
 */
export const getAgents = async () => {
  const http = useHttp()
  const res = await http.get<AssistantCapabilityVO[]>(capabilitiesUrl, undefined, {
    baseURL,
    timeout: 10000
  })
  const capabilities = Array.isArray(res?.data) ? res.data : []
  const capabilityNames = capabilities.map((item) => item.name).filter(Boolean)
  return [
    {
      id: 'general-assistant',
      name: '通用助手',
      url: chatUrl,
      icon: 'AI',
      desc: capabilityNames.length
        ? `可协调：${capabilityNames.join('、')}`
        : '智能识别任务并协调垂直助手'
    }
  ] satisfies AiAgentVO[]
}

/**
 * 发送 AI 对话请求
 * @param url 智能体对话接口地址
 * @param text 用户输入文本
 */
export const requestAiReply = async (text: string, conversationId?: string) => {
  const http = useHttp()
  const userStore = useUserStore()
  const data: AiChatRequestDTO = {
    conversationId,
    message: text,
    context: {
      userId: userStore.getUser?.id || 'dev-user'
    }
  }
  const res = await http.post<AiChatReplyVO>(chatUrl, data, {
    baseURL,
    timeout: 100000
  })
  return res?.data
}
