# `AiChatWidget` 智能对话组件 🤖

用于在页面右下角提供“智能对话”入口，点击后以右侧抽屉形式打开对话面板。该组件定位为 AI 接入的 UI 容器，默认提供占位回复，可通过 `request` 注入真实的 AI 请求逻辑。

## ✨ 特性
- **悬浮入口**：固定在页面右下角，不随页面滚动。
- **贴边自动隐藏**：入口按钮可贴右侧自动隐藏，仅露出一小截，减少遮挡页面内容。
- **侧边栏对话框**：使用 `el-drawer` 从右侧拉出，玻璃拟态背景。
- **消息列表**：支持 user / assistant / system 三类消息展示，并自动滚动到底部。
- **快捷发送**：Enter 发送、Shift+Enter 换行。
- **可插拔请求**：通过 `request(text, history, images)` 接入你的 AI 接口。
- **智能体选择**：输入区工具栏以图标入口触发下拉菜单选择智能体（图标来自服务端字符串：`emoji` 或图片 URL）。
- **图片附件**：输入区工具栏提供选图入口，支持预览、移除，并随消息一起发送（只接收图片类型）。
- **懒加载智能体**：支持首次打开对话框时加载智能体列表，并透出加载中/失败/重试状态。

## 🧩 Props
| 参数 | 说明 | 类型 | 默认值 |
| --- | --- | --- | --- |
| title | 标题 | string | 智能对话 |
| subtitle | 副标题 | string | 用于项目接入 AI 的对话面板 |
| fabText | 右下角入口按钮文案 | string | 智能助手 |
| fabIcon | 右下角入口按钮图标 | any | ChatDotRound |
| fabAutoHide | 入口按钮贴右侧自动隐藏 | boolean | false |
| fabPeekWidth | 贴边隐藏时保留可见宽度(px) | number | 24 |
| sendIcon | 发送按钮图标 | any | Promotion |
| closeIcon | 关闭按钮图标 | any | Close |
| resetIcon | 清空按钮图标 | any | Delete |
| agentIcon | 智能体选择入口图标 | any | User |
| imageIcon | 选择图片入口图标 | any | Picture |
| placeholder | 输入框占位提示 | string | 输入内容，Enter 发送，Shift+Enter 换行 |
| maxlength | 输入框最大字数 | number | 2000 |
| initialMessages | 初始消息列表（用于历史回显/演示） | ChatMessage[] | [] |
| timeDividerIntervalMinutes | 时间分隔防重复显示间隔（分钟） | number | 2 |
| emptyTitle | 空状态标题 | string | 开始对话 |
| emptyDesc | 空状态描述 | string | 在这里输入你的问题，后续可接入项目 AI 接口。 |
| welcomeMessage | 初始 AI 欢迎语（设为空字符串可关闭默认欢迎语） | string | 你好，我是你的智能助手。你可以把这里当作项目 AI 接入的对话入口。 |
| loadingText | 执行中提示文案 | string | 执行中 |
| showLoadingBubble | 是否展示执行中气泡 | boolean | true |
| agents | 智能体列表（不传则不显示下拉选择） | AgentOption[] | [] |
| agentsLoading | 智能体列表是否正在加载 | boolean | false |
| agentsError | 智能体列表加载失败提示 | string | '' |
| currentAgent | 当前选中的智能体（受控） | AgentOption \| null | null |
| defaultCurrentAgent | 默认选中的智能体（非受控） | AgentOption \| null | null |
| agentPlaceholder | 智能体选择占位 | string | 选择智能体 |
| drawerWidth | 抽屉宽度 | string | 420px |
| request | 自定义请求实现 | (text: string, history: ChatMessage[], images?: ChatImage[]) => Promise<string> | - |

## 📡 Events
| 事件 | 说明 | 参数 |
| --- | --- | --- |
| send | 发送消息时触发 | `{ text, history, currentAgent, images }` |
| reset | 清空对话时触发 | - |
| open | 打开对话框时触发 | - |
| retry-load-agents | 点击重试加载智能体时触发 | - |
| update:currentAgent | 智能体对象变更 | `AgentOption \| null` |
| agent-change | 智能体切换事件 | `AgentOption \| null` |

## 💬 ChatMessage 类型
定义在 [type.ts](./type.ts) 中，字段如下：

```ts
type Role = 'user' | 'assistant' | 'system'

interface ChatImage {
  id: string
  name: string
  size: number
  url: string
  file?: File
}

interface ChatMessage {
  id: string
  role: Role
  agentId?: string
  content: string
  images?: ChatImage[]
  createdAt: number
}
```

## 🤖 AgentOption 类型
定义在 [type.ts](./type.ts) 中：
```ts
interface AgentOption {
  id: string
  name: string
  url?: string
  icon?: string
  desc?: string
}
```

### 🎨 icon 字段约定
- `icon` 为 **emoji/字符**：直接渲染为文本头像，例如 `🤖`、`🧪`。
- `icon` 为 **图片 URL / data URL / 相对路径**：渲染为 `<img>`，例如 `https://.../robot.png`、`/icons/robot.png`、`data:image/png;base64,...`。
- 未传 `icon`：回退到组件的 `agentIcon`（Vue 组件图标）。

## 🚀 使用示例

### 1）直接使用（占位回复）
```vue
<template>
  <AiChatWidget />
</template>

<script setup lang="ts">
import AiChatWidget from '@/components/ai-chat/AiChatWidget.vue'
</script>
```

### 2）接入项目 AI 接口（示例）
```vue
<template>
  <AiChatWidget
    :request="request"
    :agents="agents"
    :agents-loading="agentsLoading"
    :agents-error="agentsError"
    v-model:current-agent="currentAgent"
    loading-text="思考中"
    @open="loadAgents"
    @retry-load-agents="retryLoadAgents"
  />
</template>

<script setup lang="ts">
import { ref } from 'vue'
import AiChatWidget from '@/components/ai-chat/AiChatWidget.vue'
import type { AgentOption, ChatImage } from '@/components/ai-chat/type'

const currentAgent = ref<AgentOption | null>(null)
const agents = ref<AgentOption[]>([])
const agentsLoading = ref(false)
const agentsError = ref('')

const loadAgents = async () => {
  if (agentsLoading.value) return
  agentsLoading.value = true
  agentsError.value = ''
  try {
    // 这里替换成你的真实接口
    agents.value = [
      { id: 'assistant-1', name: '通用助手', url: '/ai/chat' },
      { id: 'assistant-2', name: '需求分析', url: '/ai/chat' }
    ]
    currentAgent.value = agents.value[0] || null
  } catch (error) {
    agentsError.value = '获取助手列表失败，请重试'
  } finally {
    agentsLoading.value = false
  }
}

const retryLoadAgents = () => loadAgents()

const request = async (text: string, _history: any, images?: ChatImage[]) => {
  return `(${currentAgent.value?.name || 'AI'}) AI 回复占位：${text}${images?.length ? '（含图片）' : ''}`
}
</script>
```

## 📝 交互说明
- Enter 发送消息；Shift+Enter 插入换行。
- 输入框默认最多输入 2000 字，并显示字数统计。
- 消息时间默认按 2 分钟间隔去重；两条消息间隔小于等于 2 分钟时，不重复显示时间。
- 时间显示规则：当天内显示 `HH:mm`，跨天且 7 天内显示 `星期X HH:mm`，更早显示 `YYYY-M-D`。
- 点击工具栏图片按钮可选择图片（仅接收图片类型），发送前可移除。
- 首次打开对话框时，可结合 `open` 事件懒加载智能体列表；加载失败时可结合 `retry-load-agents` 提供重试。
- 点击右上角垃圾桶按钮清空对话。
- 点击右上角关闭按钮关闭抽屉。
