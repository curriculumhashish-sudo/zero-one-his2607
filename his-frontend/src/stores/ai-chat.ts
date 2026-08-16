import { defineStore } from 'pinia'
import type { AgentOption, ChatImage, ChatMessage } from '@/components/ai-chat/type'
import { getAgents, requestAiReply } from '@/apis/ai'
import type { AiChatReplyVO } from '@/apis/ai/type'

/**
 * AI 对话相关状态
 * - 提供智能体列表与当前选中智能体
 * - 承载首页演示历史消息与对话请求方法
 */
export const useAiChatStore = defineStore('ai-chat', {
  state: () => ({
    /** 当前选中的智能体对象 */
    currentAgent: null as AgentOption | null,
    /** 智能体列表（首次打开对话窗口时按需加载） */
    agents: [] as AgentOption[],
    /** 智能体列表加载失败信息 */
    agentsLoadError: '',
    /** 智能体列表是否已完成初始化加载 */
    agentsLoaded: false,
    /** 智能体列表是否正在加载 */
    agentsLoading: false,
    /** 初始历史消息 */
    initialMessages: [] as ChatMessage[],
    /** his-assistant 返回的服务端会话标识，用于连续多轮对话。 */
    conversationId: ''
  }),
  actions: {
    /**
     * 设置当前选中的智能体
     * @param agent 智能体对象
     */
    setCurrentAgent(agent: AgentOption | null) {
      this.currentAgent = agent
    },
    /**
     * 设置智能体列表
     * @param list 智能体列表
     */
    setAgents(list: AgentOption[]) {
      this.agents = Array.isArray(list) ? list : []
      const currentId = this.currentAgent?.id
      this.currentAgent =
        this.agents.find((item) => item.id === currentId) || this.agents[0] || null
    },
    /**
     * 首次打开对话框时加载智能体列表
     * @param force 是否强制重试加载
     */
    async loadAgents(force = false) {
      if ((!force && this.agentsLoaded) || this.agentsLoading) return
      this.agentsLoading = true
      this.agentsLoadError = ''
      try {
        this.setAgents(await getAgents())
      } catch (err) {
        console.error('加载智能体列表失败:', err)
        this.agentsLoadError = '获取助手列表失败，请重试'
        this.setAgents([])
      } finally {
        this.agentsLoaded = this.agents.length > 0
        this.agentsLoading = false
      }
    },
    /**
     * 对话请求（目前实现大部分是示例代码，后续根据需求调整实现）
     * @param text 用户输入文本
     * @param _history 历史消息（预留给多轮对话上下文）
     * @param _images 当前发送的图片列表
     */
    async requestAi(text: string, _history: ChatMessage[], _images?: ChatImage[]) {
      const currentAgent = this.currentAgent
      if (!currentAgent) return '请选择助手'
      if (currentAgent?.url) {
        try {
          const reply = await requestAiReply(text, this.conversationId || undefined)
          if (typeof reply === 'undefined') return '（无内容）'
          this.conversationId = reply.conversationId || this.conversationId
          return this.getReplyText(reply)
        } catch (err: any) {
          const msg = err?.message || err?.data?.message || '未知错误'
          return `请求失败：${msg}`
        }
      }
      // 直接提示无法访问智能体
      return `无法访问智能助手 ${currentAgent?.name}`
    },
    /**
     * 统一提取 AI 回复文本
     * @param reply AI 响应数据
     */
    getReplyText(reply: AiChatReplyVO) {
      if (typeof reply.answer === 'string') {
        return reply.degraded ? `【降级模式】${reply.answer}` : reply.answer
      }
      return JSON.stringify(reply)
    }
  }
})
