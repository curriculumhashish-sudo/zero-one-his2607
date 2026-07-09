<template>
  <div
    class="ai-chat-widget"
    :class="{ 'is-auto-hide': fabAutoHide, 'is-drawer-open': visible }"
    :style="fabVars"
  >
    <button class="ai-chat-fab" type="button" @click="openDrawer">
      <el-icon :size="22"><component :is="fabIcon" /></el-icon>
      <span class="ai-chat-fab-text">{{ fabText }}</span>
    </button>

    <el-drawer
      v-model="visible"
      direction="rtl"
      :with-header="false"
      :size="drawerWidth"
      :modal="true"
      :lock-scroll="true"
      class="ai-chat-drawer"
    >
      <div class="ai-chat-panel">
        <div class="ai-chat-header">
          <div class="ai-chat-title">
            <div class="ai-chat-badge">AI</div>
            <div class="ai-chat-title-text">
              <div class="ai-chat-title-main">{{ title }}</div>
              <div class="ai-chat-title-sub">{{ subtitle }}</div>
            </div>
          </div>

          <div class="ai-chat-actions">
            <el-button circle text :disabled="sending" @click="reset">
              <el-icon><component :is="resetIcon" /></el-icon>
            </el-button>
            <el-button circle text @click="visible = false">
              <el-icon><component :is="closeIcon" /></el-icon>
            </el-button>
          </div>
        </div>

        <div ref="listRef" class="ai-chat-list">
          <div v-if="agentsLoading" class="ai-chat-loading-banner">
            <span class="ai-chat-loading-banner-text">正在加载助手列表...</span>
            <span class="ai-chat-loading-dots">
              <i class="dot" />
              <i class="dot" />
              <i class="dot" />
            </span>
          </div>
          <template v-for="(m, index) in messages" :key="m.id">
            <div v-if="shouldShowMessageTime(index)" class="ai-chat-time-divider">
              {{ formatTime(m.createdAt) }}
            </div>
            <div v-if="m.role === 'system'" class="ai-chat-system">
              <span class="ai-chat-system-text" v-text="m.content"></span>
            </div>
            <div v-else class="ai-chat-item" :class="`role-${m.role}`">
              <div class="ai-chat-avatar">
                <span v-if="m.role === 'user'">我</span>
                <template v-else>
                  <img
                    v-if="resolveAgentIcon(m.agentId).kind === 'img'"
                    class="ai-chat-avatar-img"
                    :src="resolveAgentIcon(m.agentId).src"
                    :alt="resolveAgentIcon(m.agentId).alt"
                  />
                  <span
                    v-else-if="resolveAgentIcon(m.agentId).kind === 'text'"
                    class="ai-chat-avatar-text"
                  >
                    {{ resolveAgentIcon(m.agentId).text }}
                  </span>
                  <el-icon v-else :size="22">
                    <component :is="resolveAgentIcon(m.agentId).component" />
                  </el-icon>
                </template>
              </div>
              <div class="ai-chat-bubble">
                <div v-if="m.images?.length" class="ai-chat-images">
                  <img
                    v-for="img in m.images"
                    :key="img.id"
                    class="ai-chat-image"
                    :src="img.url"
                    :alt="img.name"
                  />
                </div>
                <div class="ai-chat-content" v-text="m.content"></div>
              </div>
            </div>
          </template>

          <div v-if="sending && showLoadingBubble" class="ai-chat-item role-assistant">
            <div class="ai-chat-avatar">
              <span>AI</span>
            </div>
            <div class="ai-chat-bubble ai-chat-bubble-loading">
              <div class="ai-chat-loading-row">
                <span class="ai-chat-loading-text">{{ loadingText }}</span>
                <span class="ai-chat-loading-dots">
                  <i class="dot" />
                  <i class="dot" />
                  <i class="dot" />
                </span>
              </div>
            </div>
          </div>

          <div v-if="messages.length === 0" class="ai-chat-empty">
            <div class="ai-chat-empty-title">{{ agentsLoading ? '助手加载中' : emptyTitle }}</div>
            <div class="ai-chat-empty-desc">
              {{ agentsLoading ? '正在获取助手列表，请稍候...' : emptyDesc }}
            </div>
          </div>
        </div>

        <div class="ai-chat-input-area">
          <div class="ai-chat-toolbar">
            <el-dropdown
              v-if="agents.length > 0 || agentsLoading"
              trigger="click"
              :teleported="true"
              :hide-on-click="true"
              :disabled="agentsLoading || agents.length === 0"
              popper-class="ai-chat-agent-popper"
              @command="handleAgentChange"
            >
              <el-button text class="ai-chat-agent-trigger" :disabled="sending || agentsLoading">
                <span class="ai-chat-agent-trigger-icon">
                  <img
                    v-if="
                      resolveIcon(
                        currentAgentInner?.icon,
                        currentAgentInner?.name || 'AI',
                        agentIcon
                      ).kind === 'img'
                    "
                    class="ai-chat-agent-icon-img"
                    :src="
                      resolveIcon(
                        currentAgentInner?.icon,
                        currentAgentInner?.name || 'AI',
                        agentIcon
                      ).src
                    "
                    :alt="
                      resolveIcon(
                        currentAgentInner?.icon,
                        currentAgentInner?.name || 'AI',
                        agentIcon
                      ).alt
                    "
                  />
                  <span
                    v-else-if="
                      resolveIcon(
                        currentAgentInner?.icon,
                        currentAgentInner?.name || 'AI',
                        agentIcon
                      ).kind === 'text'
                    "
                    class="ai-chat-agent-icon-text"
                  >
                    {{
                      resolveIcon(
                        currentAgentInner?.icon,
                        currentAgentInner?.name || 'AI',
                        agentIcon
                      ).text
                    }}
                  </span>
                  <el-icon v-else>
                    <component
                      :is="
                        resolveIcon(
                          currentAgentInner?.icon,
                          currentAgentInner?.name || 'AI',
                          agentIcon
                        ).component
                      "
                    />
                  </el-icon>
                </span>
                <span class="ai-chat-agent-name">{{
                  currentAgentInner?.name || (agentsLoading ? '加载中...' : agentPlaceholder)
                }}</span>
                <el-icon class="ai-chat-agent-caret"><CaretBottom /></el-icon>
              </el-button>
              <template #dropdown>
                <el-dropdown-menu class="ai-chat-agent-menu">
                  <el-dropdown-item
                    v-for="a in agents"
                    :key="a.id"
                    :command="a"
                    :class="{ 'is-active': a.id === currentAgentInner?.id }"
                  >
                    <div class="ai-chat-agent-item">
                      <div class="ai-chat-agent-item-name">
                        <span class="ai-chat-agent-item-icon">
                          <img
                            v-if="resolveIcon(a.icon, a.name, agentIcon).kind === 'img'"
                            class="ai-chat-agent-icon-img"
                            :src="resolveIcon(a.icon, a.name, agentIcon).src"
                            :alt="resolveIcon(a.icon, a.name, agentIcon).alt"
                          />
                          <span
                            v-else-if="resolveIcon(a.icon, a.name, agentIcon).kind === 'text'"
                            class="ai-chat-agent-icon-text"
                          >
                            {{ resolveIcon(a.icon, a.name, agentIcon).text }}
                          </span>
                          <el-icon v-else>
                            <component :is="resolveIcon(a.icon, a.name, agentIcon).component" />
                          </el-icon>
                        </span>
                        <span>{{ a.name }}</span>
                      </div>
                      <div v-if="a.desc" class="ai-chat-agent-item-desc">{{ a.desc }}</div>
                    </div>
                  </el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
            <span v-else-if="agentsError" class="ai-chat-agent-error-wrap">
              <span class="ai-chat-agent-error" :title="agentsError" v-text="agentsError" />
              <button
                type="button"
                class="ai-chat-agent-retry"
                :disabled="agentsLoading"
                @click="emit('retry-load-agents')"
              >
                重试
              </button>
            </span>
            <el-button text class="ai-chat-tool-btn" :disabled="sending" @click="openFilePicker">
              <el-icon><component :is="imageIcon" /></el-icon>
            </el-button>
          </div>
          <input
            ref="fileInputRef"
            class="ai-chat-file-input"
            type="file"
            accept="image/*"
            multiple
            @change="onPickImages"
          />
          <div v-if="pendingImages.length > 0" class="ai-chat-pending">
            <div class="ai-chat-pending-list">
              <div v-for="img in pendingImages" :key="img.id" class="ai-chat-pending-item">
                <img class="ai-chat-pending-thumb" :src="img.url" :alt="img.name" />
                <button
                  type="button"
                  class="ai-chat-pending-remove"
                  @click="removePendingImage(img.id)"
                >
                  <el-icon :size="14"><Close /></el-icon>
                </button>
              </div>
            </div>
          </div>
          <el-input
            v-model="draft"
            type="textarea"
            :autosize="{ minRows: 6, maxRows: 12 }"
            :placeholder="placeholder"
            :maxlength="maxlength"
            show-word-limit
            :disabled="sending"
            @keydown="onKeydown"
          />
          <div class="ai-chat-input-actions">
            <el-button type="primary" :loading="sending" :disabled="!canSend" @click="send">
              发送
              <el-icon class="ai-chat-send-icon"><component :is="sendIcon" /></el-icon>
            </el-button>
          </div>
        </div>
      </div>
    </el-drawer>
  </div>
</template>

<script setup lang="ts">
import { computed, nextTick, onMounted, onUnmounted, ref, watch } from 'vue'
import {
  ChatDotRound,
  Close,
  Delete,
  Promotion,
  User,
  CaretBottom,
  Picture
} from '@element-plus/icons-vue'
import type { AgentOption, ChatImage, ChatMessage } from './type'

interface Props {
  /** 标题 */
  title?: string
  /** 副标题 */
  subtitle?: string
  /** 右下角入口按钮文案 */
  fabText?: string
  /** 右下角入口按钮图标 */
  fabIcon?: any
  /** 右下角入口按钮是否贴右侧并自动隐藏（减少遮挡页面内容） */
  fabAutoHide?: boolean
  /** 入口按钮贴边隐藏时保留可见宽度(px) */
  fabPeekWidth?: number
  /** 发送按钮图标 */
  sendIcon?: any
  /** 关闭按钮图标 */
  closeIcon?: any
  /** 清空按钮图标 */
  resetIcon?: any
  /** 智能体选择入口图标 */
  agentIcon?: any
  /** 选择图片入口图标 */
  imageIcon?: any
  /** 输入框占位提示 */
  placeholder?: string
  /** 输入框最大字数 */
  maxlength?: number
  /** 初始消息列表（用于历史回显/演示效果） */
  initialMessages?: ChatMessage[]
  /** 时间分隔防重复显示间隔（分钟） */
  timeDividerIntervalMinutes?: number
  /** 空状态标题 */
  emptyTitle?: string
  /** 空状态描述 */
  emptyDesc?: string
  /** 初始 AI 欢迎语（设为空字符串可关闭默认欢迎语） */
  welcomeMessage?: string
  /** 执行中提示文案 */
  loadingText?: string
  /** 是否展示执行中气泡 */
  showLoadingBubble?: boolean
  /** 智能体列表（不传则不显示下拉选择） */
  agents?: AgentOption[]
  /** 智能体列表是否正在加载 */
  agentsLoading?: boolean
  /** 智能体列表加载失败提示 */
  agentsError?: string
  /** 当前选中的智能体（受控） */
  currentAgent?: AgentOption | null
  /** 默认选中的智能体（非受控） */
  defaultCurrentAgent?: AgentOption | null
  /** 智能体选择占位 */
  agentPlaceholder?: string
  /** 抽屉宽度 */
  drawerWidth?: string
  /** 发送请求实现：输入文本 + 历史消息 + 当前发送图片 => 返回 AI 回复文本 */
  request?: (text: string, history: ChatMessage[], images?: ChatImage[]) => Promise<string>
}

const props = withDefaults(defineProps<Props>(), {
  title: '智能对话',
  subtitle: '用于项目接入 AI 的对话面板',
  fabText: '智能助手',
  fabIcon: ChatDotRound,
  fabAutoHide: false,
  fabPeekWidth: 24,
  sendIcon: Promotion,
  closeIcon: Close,
  resetIcon: Delete,
  agentIcon: User,
  imageIcon: Picture,
  placeholder: '输入内容，Enter 发送，Shift+Enter 换行',
  maxlength: 2000,
  initialMessages: () => [],
  timeDividerIntervalMinutes: 2,
  emptyTitle: '开始对话',
  emptyDesc: '在这里输入你的问题，后续可接入项目 AI 接口。',
  welcomeMessage: '你好，我是你的智能助手。你可以把这里当作项目 AI 接入的对话入口。',
  loadingText: '执行中',
  showLoadingBubble: true,
  agents: () => [],
  agentsLoading: false,
  agentsError: '',
  currentAgent: null,
  defaultCurrentAgent: null,
  agentPlaceholder: '选择智能体',
  drawerWidth: '420px'
})

const emit = defineEmits<{
  (
    e: 'send',
    payload: {
      text: string
      history: ChatMessage[]
      currentAgent: AgentOption | null
      images?: ChatImage[]
    }
  ): void
  (e: 'reset'): void
  (e: 'open'): void
  (e: 'retry-load-agents'): void
  (e: 'update:currentAgent', value: AgentOption | null): void
  (e: 'agent-change', value: AgentOption | null): void
}>()

/**
 * 对话框是否可见（抽屉开关）
 */
const visible = ref(false)
/**
 * 输入框草稿内容
 */
const draft = ref('')
/**
 * 是否正在请求（用于禁用交互与展示 loading 气泡）
 */
const sending = ref(false)
/**
 * 消息列表容器，用于滚动控制
 */
const listRef = ref<HTMLElement | null>(null)
/**
 * 当前会话消息列表（包含 system / user / assistant）
 */
const messages = ref<ChatMessage[]>([])
/**
 * 待发送的本地图片附件（选图后先预览，发送时并入 user 消息）
 */
const pendingImages = ref<ChatImage[]>([])
/**
 * 隐藏的文件选择器 input
 */
const fileInputRef = ref<HTMLInputElement | null>(null)
/**
 * 当前选中的智能体（内部状态）
 * - 优先：defaultCurrentAgent（非受控默认值）
 * - 其次：currentAgent（受控值）
 * - 再次：agents[0]
 */
const currentAgentInner = ref<AgentOption | null>(
  props.defaultCurrentAgent || props.currentAgent || props.agents[0] || null
)

/**
 * 是否允许发送：
 * - 有文本 或 有图片
 * - 且不在发送中
 */
const canSend = computed(
  () => (draft.value.trim().length > 0 || pendingImages.value.length > 0) && !sending.value
)
/**
 * 入口按钮贴边隐藏时，用 CSS 变量控制露出宽度
 */
const fabVars = computed(() => ({
  '--ai-chat-fab-peek': `${props.fabPeekWidth}px`
}))

/**
 * 格式化消息时间
 * - 当天内：HH:mm
 * - 跨天但 7 天内：星期X HH:mm
 * - 更早：YYYY-M-D
 */
const formatTime = (ts: number) => {
  const d = new Date(ts)
  const now = new Date()
  const hh = String(d.getHours()).padStart(2, '0')
  const mm = String(d.getMinutes()).padStart(2, '0')
  const time = `${hh}:${mm}`

  const todayStart = new Date(now.getFullYear(), now.getMonth(), now.getDate()).getTime()
  const targetStart = new Date(d.getFullYear(), d.getMonth(), d.getDate()).getTime()
  const dayDiff = Math.floor((todayStart - targetStart) / (24 * 60 * 60 * 1000))

  if (dayDiff === 0) return time
  if (dayDiff > 0 && dayDiff < 7) {
    const weekMap = ['星期日', '星期一', '星期二', '星期三', '星期四', '星期五', '星期六']
    return `${weekMap[d.getDay()]} ${time}`
  }

  return `${d.getFullYear()}-${d.getMonth() + 1}-${d.getDate()}`
}

/**
 * 判断是否为初始化欢迎语消息
 */
const isWelcomeMessage = (msg?: ChatMessage) => {
  return (
    !!msg &&
    msg.role === 'assistant' &&
    !!props.welcomeMessage &&
    msg.content === props.welcomeMessage
  )
}

/**
 * 查找当前消息之前、用于时间去重比较的上一条消息
 * - 跳过 system 消息
 * - 跳过初始化欢迎语，避免它把后续真实对话的时间“吃掉”
 */
const findPreviousComparableMessage = (index: number) => {
  for (let i = index - 1; i >= 0; i -= 1) {
    const msg = messages.value[i]
    if (!msg) continue
    if (msg.role === 'system') continue
    if (isWelcomeMessage(msg)) continue
    return msg
  }
  return undefined
}

/**
 * 判断当前消息是否需要显示独立时间行
 * - 第一条消息始终显示
 * - 与上一条可比较消息间隔小于等于指定分钟数时不显示时间
 */
const shouldShowMessageTime = (index: number) => {
  const current = messages.value[index]
  if (!current) return false
  if (index === 0) return true
  if (isWelcomeMessage(current)) return true
  const prev = findPreviousComparableMessage(index)
  if (!prev) return true
  const intervalMs = Math.max(props.timeDividerIntervalMinutes, 0) * 60 * 1000
  return current.createdAt - prev.createdAt > intervalMs
}

/**
 * 将消息列表滚动到最底部
 */
const scrollToBottom = async () => {
  await nextTick()
  const el = listRef.value
  if (!el) return
  el.scrollTop = el.scrollHeight
}

/**
 * 抽屉打开时，滚动到底部
 */
watch(
  () => visible.value,
  (v) => {
    if (!v) return
    emit('open')
    scrollToBottom()
  }
)

/**
 * 新增消息时，滚动到底部
 */
watch(
  () => messages.value.length,
  () => scrollToBottom()
)

/**
 * 生成消息 id
 */
const createId = () => `${Date.now()}_${Math.random().toString(16).slice(2)}`

/**
 * 复制并按时间升序排序初始消息
 * 避免外部传入的历史消息顺序混乱，导致时间分隔显示异常
 */
const normalizeInitialMessages = (list: ChatMessage[]) => {
  return list.map((msg) => ({ ...msg })).sort((a, b) => a.createdAt - b.createdAt)
}

/**
 * 统一的图标解析结果
 * - img: 图片（URL / data URL）
 * - text: 文本（emoji/字符）
 * - component: Vue 组件（用于回退到默认图标）
 */
type ResolvedIconKind = 'img' | 'text' | 'component'

/**
 * 图标解析结果的统一结构，将各种形式的图标输入标准化为固定类型
 */
interface ResolvedIcon {
  /** 图标类型：图片/文本/组件 */
  kind: ResolvedIconKind
  /** 图片类型的资源地址，非图片类型为空字符串 */
  src: string
  /** 图片的 alt 描述文本 */
  alt: string
  /** 文本类型的显示文本（支持 emoji/字符），非文本类型为空字符串 */
  text: string
  /** 组件类型的 Vue 组件实例，非组件类型为默认回退图标 */
  component: any
}

/**
 * 判断 icon 字符串是否为图片地址
 */
const isImageIconString = (v: string) => {
  const s = v.trim()
  return (
    s.startsWith('data:image/') ||
    s.startsWith('http://') ||
    s.startsWith('https://') ||
    s.startsWith('/') ||
    s.startsWith('./') ||
    s.startsWith('../')
  )
}

/**
 * 统一解析图标配置：
 * - 空值时回退到默认组件图标
 * - 图片地址返回 img 类型
 * - 其余字符串按文本图标处理
 * @param icon 图标配置
 * @param name 图标名称，用于生成图片 alt
 * @param fallback 默认回退图标组件
 * @returns 标准化后的图标描述对象
 */
const resolveIcon = (icon: string | undefined, name: string, fallback: any): ResolvedIcon => {
  const alt = name || 'AI'
  if (typeof icon !== 'string' || icon.trim().length === 0) {
    return { kind: 'component', src: '', alt, text: '', component: fallback }
  }
  const s = icon.trim()
  if (isImageIconString(s)) return { kind: 'img', src: s, alt, text: '', component: fallback }
  return { kind: 'text', src: '', alt, text: s, component: fallback }
}

/**
 * 根据智能体 id 解析头像/图标
 */
const resolveAgentIcon = (agentId?: string): ResolvedIcon => {
  const agent = props.agents.find((a) => a.id === agentId) || currentAgentInner.value
  return resolveIcon(agent?.icon, agent?.name || 'AI', props.agentIcon)
}

/**
 * 追加 system 消息（例如切换智能体、忽略非图片文件等）
 */
const appendSystemMessage = (content: string, agentId?: string) => {
  const tip: ChatMessage = {
    id: createId(),
    role: 'system',
    agentId,
    content,
    createdAt: Date.now()
  }
  messages.value = [...messages.value, tip]
}

/**
 * 回收 blob URL，避免频繁选图/清空导致内存泄漏
 */
const revokeImages = (imgs?: ChatImage[]) => {
  if (!imgs || imgs.length === 0) return
  imgs.forEach((img) => {
    if (img.url && img.url.startsWith('blob:')) URL.revokeObjectURL(img.url)
  })
}

/**
 * 图片扩展名白名单
 */
const imageExtAllowList = ['.png', '.jpg', '.jpeg', '.gif', '.webp', '.bmp', '.svg']

/**
 * 判断文件是否为图片
 * - 优先使用 MIME（file.type）
 * - 兼容部分浏览器/场景未提供 MIME 的情况（回退到扩展名）
 */
const isImageFile = (f: File) => {
  const mime = (f.type || '').toLowerCase()
  if (mime.startsWith('image/')) return true
  const dot = f.name.lastIndexOf('.')
  const ext = dot >= 0 ? f.name.slice(dot).toLowerCase() : ''
  return imageExtAllowList.includes(ext)
}

/**
 * 打开对话抽屉，并通知外部执行首次加载
 */
const openDrawer = () => {
  visible.value = true
}

/**
 * 打开图片选择器
 */
const openFilePicker = () => {
  const el = fileInputRef.value
  if (!el) return
  el.click()
}

/**
 * 选择图片后的处理：
 * - 过滤非图片文件（accept 只是提示，不可靠）
 * - 生成 blob URL 用于预览，并在移除/卸载时回收
 */
const onPickImages = (evt: Event) => {
  const target = evt.target as HTMLInputElement | null
  const files = target?.files
  if (!files || files.length === 0) return

  const next: ChatImage[] = []
  const ignored: string[] = []
  Array.from(files).forEach((f) => {
    if (!isImageFile(f)) {
      ignored.push(f.name)
      return
    }
    const url = URL.createObjectURL(f)
    next.push({
      id: createId(),
      name: f.name,
      size: f.size,
      url,
      file: f
    })
  })

  if (ignored.length > 0) {
    appendSystemMessage(`已忽略非图片文件：${ignored.join('、')}`)
  }

  if (next.length > 0) {
    pendingImages.value = [...pendingImages.value, ...next]
  }
  if (target) target.value = ''
}

/**
 * 移除待发送图片，并回收对应 blob URL
 */
const removePendingImage = (id: string) => {
  const hit = pendingImages.value.find((x) => x.id === id)
  if (hit) revokeImages([hit])
  pendingImages.value = pendingImages.value.filter((x) => x.id !== id)
}

/**
 * 统一的请求入口：如果未注入 request，则走默认占位实现
 */
const requestImpl = async (text: string, history: ChatMessage[], images?: ChatImage[]) => {
  if (props.request) return props.request(text, history, images)
  return Promise.resolve(`已收到：${text}`)
}

/**
 * 发送消息
 * - 将文本与待发送图片合并为一条 user 消息
 * - 触发 send 事件（对外通知）
 * - 调用 request（如果注入）获取 AI 回复，并追加 assistant 消息
 */
const send = async () => {
  const text = draft.value.trim()
  if ((!text && pendingImages.value.length === 0) || sending.value) return

  const images = pendingImages.value.length > 0 ? [...pendingImages.value] : undefined

  const userMsg: ChatMessage = {
    id: createId(),
    role: 'user',
    content: text,
    images,
    createdAt: Date.now()
  }
  messages.value = [...messages.value, userMsg]
  draft.value = ''
  pendingImages.value = []

  sending.value = true
  try {
    emit('send', { text, history: messages.value, currentAgent: currentAgentInner.value, images })
    const reply = await requestImpl(text, messages.value, images)
    const aiMsg: ChatMessage = {
      id: createId(),
      role: 'assistant',
      agentId: currentAgentInner.value?.id,
      content: reply || '（无内容）',
      createdAt: Date.now()
    }
    messages.value = [...messages.value, aiMsg]
  } finally {
    sending.value = false
  }
}

/**
 * 重置对话
 */
const reset = () => {
  messages.value.forEach((m) => revokeImages(m.images))
  messages.value = []
  draft.value = ''
  revokeImages(pendingImages.value)
  pendingImages.value = []
  emit('reset')
}

/**
 * 智能体切换
 */
const handleAgentChange = (agent: AgentOption) => {
  currentAgentInner.value = agent
  emit('update:currentAgent', agent)
  emit('agent-change', agent)
  appendSystemMessage(`已切换智能体：${agent.name}`, agent.id)
}

/**
 * 输入框键盘事件：Enter 发送、Shift+Enter 换行
 */
const onKeydown = (evt: Event | KeyboardEvent) => {
  if (!(evt instanceof KeyboardEvent)) return
  const e = evt
  if (e.key !== 'Enter') return
  if (e.shiftKey) return
  e.preventDefault()
  send()
}

onMounted(() => {
  /**
   * 优先使用外部传入的初始消息；未传时再注入欢迎语
   */
  if (messages.value.length === 0 && props.initialMessages.length > 0) {
    messages.value = normalizeInitialMessages(props.initialMessages)
    return
  }
  if (messages.value.length === 0 && props.welcomeMessage) {
    messages.value = [
      {
        id: createId(),
        role: 'assistant',
        agentId: currentAgentInner.value?.id,
        content: props.welcomeMessage,
        createdAt: Date.now()
      }
    ]
  }
})

/**
 * 受控模式：外部 currentAgent 变化时同步到内部
 */
watch(
  () => props.currentAgent,
  (v) => {
    if (v !== currentAgentInner.value) currentAgentInner.value = v || null
  }
)

/**
 * 智能体列表更新时，如果当前未选择，则默认取第一个
 */
watch(
  () => props.agents,
  (list) => {
    if (!list || list.length === 0) return
    const first = list[0]
    const currentId = currentAgentInner.value?.id
    currentAgentInner.value = list.find((item) => item.id === currentId) || first || null
  }
)

onUnmounted(() => {
  /**
   * 组件卸载时统一回收所有 blob URL
   */
  messages.value.forEach((m) => revokeImages(m.images))
  revokeImages(pendingImages.value)
})
</script>

<style scoped>
.ai-chat-widget {
  position: fixed;
  right: 18px;
  bottom: 68px;
  z-index: 3000;
}

.ai-chat-widget.is-auto-hide {
  right: 0;
}

.ai-chat-fab {
  display: inline-flex;
  align-items: center;
  gap: 10px;
  height: 44px;
  padding: 0 14px;
  border-radius: 999px;
  border: 1px solid rgba(79, 172, 254, 0.35);
  background: radial-gradient(
    circle at 30% 30%,
    rgba(79, 172, 254, 0.9) 0%,
    rgba(0, 114, 255, 0.9) 55%,
    rgba(0, 242, 254, 0.85) 100%
  );
  color: #fff;
  cursor: pointer;
  box-shadow: 0 14px 34px rgba(0, 114, 255, 0.25);
  transition:
    transform 0.18s ease,
    box-shadow 0.18s ease;
  transform: translate(var(--ai-chat-fab-x, 0px), 0px);
}

.ai-chat-fab:hover {
  transform: translate(var(--ai-chat-fab-x, 0px), -2px);
  box-shadow: 0 18px 40px rgba(0, 114, 255, 0.32);
}

.ai-chat-widget.is-auto-hide .ai-chat-fab {
  --ai-chat-fab-x: calc(100% - var(--ai-chat-fab-peek, 24px));
}

.ai-chat-widget.is-auto-hide .ai-chat-fab:hover,
.ai-chat-widget.is-auto-hide .ai-chat-fab:focus-visible {
  --ai-chat-fab-x: 0px;
}

.ai-chat-widget.is-drawer-open .ai-chat-fab {
  opacity: 0;
  pointer-events: none;
  transform: translate(120%, 0);
}

.ai-chat-fab-text {
  font-size: 14px;
  font-weight: 700;
  letter-spacing: 0.4px;
}

.ai-chat-panel {
  height: 100%;
  display: flex;
  flex-direction: column;
  background:
    radial-gradient(900px 600px at 0% 0%, rgba(79, 172, 254, 0.18), transparent 60%),
    radial-gradient(700px 500px at 80% 10%, rgba(0, 242, 254, 0.14), transparent 55%),
    rgba(255, 255, 255, 0.85);
  backdrop-filter: blur(18px);
}

.ai-chat-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 14px 14px 12px;
  border-bottom: 1px solid rgba(64, 158, 255, 0.16);
}

.ai-chat-title {
  display: flex;
  align-items: center;
  gap: 12px;
}

.ai-chat-badge {
  width: 34px;
  height: 34px;
  border-radius: 10px;
  display: flex;
  align-items: center;
  justify-content: center;
  color: #fff;
  font-weight: 900;
  background: linear-gradient(135deg, rgba(79, 172, 254, 1), rgba(0, 114, 255, 1));
  box-shadow: 0 10px 18px rgba(0, 114, 255, 0.22);
}

.ai-chat-title-main {
  font-size: 15px;
  font-weight: 800;
  color: var(--title-color, #1f2d3d);
  line-height: 1.1;
}

.ai-chat-title-sub {
  font-size: 12px;
  color: rgba(31, 45, 61, 0.6);
  margin-top: 2px;
  max-width: 240px;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.ai-chat-actions {
  display: inline-flex;
  align-items: center;
  gap: 6px;
}

.ai-chat-list {
  flex: 1;
  overflow: auto;
  padding: 14px 14px 10px;
}

.ai-chat-time-divider {
  display: flex;
  justify-content: center;
  margin: 6px 0 12px;
  font-size: 12px;
  color: rgba(31, 45, 61, 0.46);
  line-height: 1;
}

.ai-chat-item {
  display: flex;
  gap: 10px;
  margin-bottom: 12px;
}

.ai-chat-system {
  display: flex;
  justify-content: center;
  margin: 10px 0 12px;
}

.ai-chat-system-text {
  font-size: 12px;
  color: rgba(31, 45, 61, 0.62);
  background: rgba(31, 45, 61, 0.06);
  border: 1px solid rgba(31, 45, 61, 0.08);
  padding: 6px 10px;
  border-radius: 999px;
  max-width: 92%;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.ai-chat-item.role-user {
  flex-direction: row-reverse;
}

.ai-chat-avatar {
  width: 40px;
  height: 40px;
  border-radius: 14px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 12px;
  font-weight: 800;
  color: #fff;
  background: rgba(64, 158, 255, 0.9);
}

.ai-chat-avatar-img {
  width: 28px;
  height: 28px;
  border-radius: 12px;
  object-fit: cover;
  display: block;
}

.ai-chat-avatar-text {
  font-size: 22px;
  line-height: 1;
}

.ai-chat-item.role-assistant .ai-chat-avatar {
  background: rgba(0, 114, 255, 0.9);
}

.ai-chat-bubble {
  max-width: 78%;
  padding: 10px 12px;
  border-radius: 14px;
  background: rgba(255, 255, 255, 0.75);
  border: 1px solid rgba(64, 158, 255, 0.16);
  box-shadow: 0 10px 22px rgba(31, 45, 61, 0.08);
}

.ai-chat-bubble-loading {
  padding: 12px 12px;
}

.ai-chat-loading-row {
  display: inline-flex;
  align-items: center;
  gap: 10px;
}

.ai-chat-loading-text {
  font-size: 13px;
  color: rgba(31, 45, 61, 0.75);
  font-weight: 700;
}

.ai-chat-loading-dots {
  display: inline-flex;
  align-items: center;
  gap: 6px;
}

.ai-chat-loading-dots .dot {
  width: 6px;
  height: 6px;
  border-radius: 999px;
  background: rgba(64, 158, 255, 0.85);
  animation: aiDotPulse 0.7s infinite ease-in-out;
}

.ai-chat-loading-dots .dot:nth-child(2) {
  animation-delay: 0.12s;
}

.ai-chat-loading-dots .dot:nth-child(3) {
  animation-delay: 0.24s;
}

.ai-chat-loading-banner {
  display: inline-flex;
  align-items: center;
  gap: 10px;
  margin: 0 0 12px;
  padding: 10px 12px;
  border-radius: 12px;
  background: rgba(64, 158, 255, 0.08);
  border: 1px solid rgba(64, 158, 255, 0.18);
}

.ai-chat-loading-banner-text {
  font-size: 12px;
  font-weight: 700;
  color: rgba(31, 45, 61, 0.72);
}

@keyframes aiDotPulse {
  0%,
  100% {
    transform: translateY(0);
    opacity: 0.55;
  }
  50% {
    transform: translateY(-4px);
    opacity: 1;
  }
}

.ai-chat-item.role-user .ai-chat-bubble {
  background: rgba(64, 158, 255, 0.1);
  border-color: rgba(64, 158, 255, 0.22);
}

.ai-chat-content {
  font-size: 13px;
  color: rgba(31, 45, 61, 0.88);
  white-space: pre-wrap;
  word-break: break-word;
  line-height: 1.55;
}

.ai-chat-empty {
  margin-top: 18px;
  padding: 14px 12px;
  border-radius: 14px;
  background: rgba(64, 158, 255, 0.08);
  border: 1px dashed rgba(64, 158, 255, 0.28);
}

.ai-chat-empty-title {
  font-size: 14px;
  font-weight: 800;
  color: rgba(31, 45, 61, 0.85);
  margin-bottom: 6px;
}

.ai-chat-empty-desc {
  font-size: 12px;
  color: rgba(31, 45, 61, 0.6);
  line-height: 1.5;
}

.ai-chat-toolbar {
  display: flex;
  align-items: center;
  justify-content: flex-start;
  gap: 8px;
  padding: 0 0 10px;
}

.ai-chat-tool-btn {
  height: 28px;
  border-radius: 10px;
  border: 1px solid rgba(64, 158, 255, 0.18);
  background: rgba(255, 255, 255, 0.7);
  box-shadow: 0 6px 16px rgba(31, 45, 61, 0.08);
  padding: 0 10px;
}

.ai-chat-file-input {
  display: none;
}

.ai-chat-pending {
  margin-top: 8px;
  padding: 10px 10px 0;
  border-radius: 12px;
  background: rgba(64, 158, 255, 0.06);
  border: 1px solid rgba(64, 158, 255, 0.14);
}

.ai-chat-pending-list {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  padding-bottom: 10px;
}

.ai-chat-pending-item {
  position: relative;
  width: 56px;
  height: 56px;
  border-radius: 12px;
  overflow: hidden;
  border: 1px solid rgba(64, 158, 255, 0.16);
  background: rgba(255, 255, 255, 0.7);
}

.ai-chat-pending-thumb {
  width: 100%;
  height: 100%;
  object-fit: cover;
  display: block;
}

.ai-chat-pending-remove {
  position: absolute;
  right: 4px;
  top: 4px;
  width: 20px;
  height: 20px;
  border: none;
  border-radius: 999px;
  background: rgba(31, 45, 61, 0.55);
  color: #fff;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
}

.ai-chat-images {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  margin-bottom: 8px;
}

.ai-chat-image {
  width: 140px;
  max-width: 100%;
  height: 92px;
  object-fit: cover;
  border-radius: 12px;
  border: 1px solid rgba(64, 158, 255, 0.18);
  background: rgba(255, 255, 255, 0.6);
}

.ai-chat-agent-trigger {
  padding: 0;
  height: 28px;
  display: inline-flex;
  align-items: center;
  gap: 8px;
  border-radius: 10px;
  border: 1px solid rgba(64, 158, 255, 0.18);
  background: rgba(255, 255, 255, 0.7);
  box-shadow: 0 6px 16px rgba(31, 45, 61, 0.08);
  padding-left: 10px;
  padding-right: 8px;
}

.ai-chat-agent-name {
  font-size: 12px;
  font-weight: 700;
  color: rgba(31, 45, 61, 0.78);
  max-width: 220px;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.ai-chat-agent-error-wrap {
  flex: 1;
  min-width: 0;
  display: inline-flex;
  align-items: center;
  gap: 8px;
}

.ai-chat-agent-error {
  flex: 1;
  min-width: 0;
  font-size: 12px;
  color: #e67e22;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.ai-chat-agent-retry {
  flex: none;
  padding: 0;
  border: 0;
  background: transparent;
  color: #409eff;
  font-size: 12px;
  cursor: pointer;
}

.ai-chat-agent-retry:disabled {
  color: rgba(64, 158, 255, 0.45);
  cursor: not-allowed;
}

.ai-chat-agent-caret {
  opacity: 0.7;
}

.ai-chat-agent-menu {
  min-width: 220px;
  max-width: 340px;
}

.ai-chat-agent-menu :deep(.el-dropdown-menu__item) {
  white-space: normal;
  align-items: flex-start;
}

.ai-chat-agent-item {
  display: flex;
  flex-direction: column;
  gap: 2px;
  width: 100%;
  min-width: 0;
}

.ai-chat-agent-item-name {
  font-size: 13px;
  font-weight: 700;
  color: rgba(31, 45, 61, 0.86);
  display: inline-flex;
  align-items: center;
  gap: 8px;
  min-width: 0;
}

.ai-chat-agent-trigger-icon,
.ai-chat-agent-item-icon {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  width: 18px;
  height: 18px;
}

.ai-chat-agent-icon-img {
  width: 18px;
  height: 18px;
  border-radius: 6px;
  object-fit: cover;
  display: block;
}

.ai-chat-agent-icon-text {
  font-size: 15px;
  line-height: 1;
}

.ai-chat-agent-item-icon {
  opacity: 0.9;
}

.ai-chat-agent-item-desc {
  font-size: 12px;
  color: rgba(31, 45, 61, 0.55);
  width: 100%;
  min-width: 0;
  white-space: normal;
  word-break: break-word;
  overflow: hidden;
  text-overflow: ellipsis;
  display: -webkit-box;
  line-clamp: 2;
  -webkit-line-clamp: 2;
  -webkit-box-orient: vertical;
}

.ai-chat-agent-menu :deep(.el-dropdown-menu__item.is-active) {
  font-weight: 800;
  color: var(--el-color-primary, #409eff);
  background: rgba(64, 158, 255, 0.1);
}

:global(.ai-chat-agent-popper) {
  z-index: 5000 !important;
}

.ai-chat-input-area {
  padding: 12px 14px 14px;
  border-top: 1px solid rgba(64, 158, 255, 0.16);
  background: rgba(255, 255, 255, 0.65);
  backdrop-filter: blur(16px);
}

.ai-chat-input-actions {
  display: flex;
  justify-content: flex-end;
  margin-top: 10px;
}

.ai-chat-send-icon {
  margin-left: 6px;
}

:deep(.ai-chat-drawer .el-drawer__body) {
  padding: 0;
}

:deep(.ai-chat-drawer .el-overlay) {
  backdrop-filter: blur(4px);
}
</style>
