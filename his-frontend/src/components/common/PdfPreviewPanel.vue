<template>
  <div class="pdf-preview-panel" :style="containerStyle">
    <!-- 加载状态 -->
    <div v-if="loading" class="pdf-preview-panel__overlay pdf-preview-panel__overlay--loading">
      <slot name="loading">
        <div class="pdf-preview-panel__loading-content">
          <span class="pdf-preview-panel__spinner"></span>
          <span>加载中...</span>
        </div>
      </slot>
    </div>

    <!-- 错误状态 -->
    <div v-else-if="error" class="pdf-preview-panel__overlay pdf-preview-panel__overlay--error">
      <slot name="error">
        <div class="pdf-preview-panel__error-content">
          <span class="pdf-preview-panel__error-icon">⚠️</span>
          <span>{{ error.message || '加载失败，请重试' }}</span>
          <button class="pdf-preview-panel__retry-btn" @click="handleRetry">重试</button>
        </div>
      </slot>
    </div>

    <!-- 空状态 -->
    <div v-else-if="!pdfSrc" class="pdf-preview-panel__overlay pdf-preview-panel__overlay--empty">
      <slot name="empty">
        <div class="pdf-preview-panel__empty-content">
          <span class="pdf-preview-panel__empty-icon">📄</span>
          <span>暂无 PDF 文件</span>
        </div>
      </slot>
    </div>

    <!-- PDF 预览 - 使用 iframe 直接加载 -->
    <iframe
      v-else
      ref="iframeRef"
      class="pdf-preview-panel__iframe"
      :src="pdfSrc"
      frameborder="0"
      allowfullscreen
      @load="handleIframeLoad"
      @error="handleIframeError"
    />

    <!-- 工具栏 -->
    <div
      v-if="showToolbar && pdfSrc && !error"
      class="pdf-preview-panel__toolbar"
    >
      <slot name="toolbar">
        <span class="pdf-preview-panel__filename">
          {{ displayFileName }}
        </span>
        <div class="pdf-preview-panel__actions">
          <button
            v-if="downloadable"
            class="pdf-preview-panel__action-btn"
            @click="handleDownload"
            title="下载 PDF"
          >
            ⬇️ 下载
          </button>
          <button
            v-if="openable"
            class="pdf-preview-panel__action-btn"
            @click="handleOpen"
            title="在新窗口打开"
          >
            🔗 新窗口
          </button>
        </div>
      </slot>
    </div>
  </div>
</template>

<script setup lang="ts">
import {
  ref,
  computed,
  watch,
  onBeforeUnmount,
  shallowRef,
  nextTick,
} from 'vue'

// ---------- Props ----------
const props = withDefaults(
  defineProps<{
    /** PDF 源：URL 字符串 或 Blob 对象 */
    source?: string | Blob | null
    /** 文件名（用于下载和显示） */
    fileName?: string
    /** 预览高度（CSS 长度） */
    height?: string | number
    /** 是否显示工具栏 */
    showToolbar?: boolean
    /** 是否允许下载 */
    downloadable?: boolean
    /** 是否允许在新窗口打开 */
    openable?: boolean
    /** 加载超时时间（毫秒） */
    timeout?: number
  }>(),
  {
    source: null,
    fileName: 'document.pdf',
    height: '600px',
    showToolbar: true,
    downloadable: true,
    openable: true,
    timeout: 30000,
  }
)

// ---------- Emits ----------
const emit = defineEmits<{
  (e: 'load', payload: { url: string; fileName: string }): void
  (e: 'error', error: Error): void
  (e: 'retry'): void
  (e: 'download', payload: { url: string; fileName: string }): void
  (e: 'open', payload: { url: string; fileName: string }): void
}>()

// ---------- 内部状态 ----------
const iframeRef = shallowRef<HTMLIFrameElement | null>(null)
const loading = ref(false)
const error = ref<Error | null>(null)
const pdfSrc = ref<string | null>(null)
const isLoaded = ref(false)

/** 组件自己创建的 Blob URL（需要主动释放） */
const selfCreatedBlobUrl = ref<string | null>(null)

/** 加载超时定时器 */
let loadTimeout: ReturnType<typeof setTimeout> | null = null

// ---------- 计算属性 ----------
const containerStyle = computed(() => ({
  height: typeof props.height === 'number' ? `${props.height}px` : props.height,
}))

const displayFileName = computed(() => {
  if (props.fileName) return props.fileName
  if (typeof props.source === 'string') {
    const parts = props.source.split('/')
    const last = parts[parts.length - 1] || 'document.pdf'
    return last.includes('.') ? last : 'document.pdf'
  }
  return 'document.pdf'
})

/** 判断 source 是否为 Blob */
const isBlobSource = computed(() => {
  return props.source instanceof Blob
})

// ---------- 核心方法 ----------
/** 创建 PDF 源 URL */
function createPdfSource(): string | null {
  if (!props.source) return null

  // 如果是 Blob，创建 Object URL
  if (props.source instanceof Blob) {
    // 如果已有 selfCreatedBlobUrl，先释放
    if (selfCreatedBlobUrl.value) {
      URL.revokeObjectURL(selfCreatedBlobUrl.value)
      selfCreatedBlobUrl.value = null
    }
    const url = URL.createObjectURL(props.source)
    selfCreatedBlobUrl.value = url
    return url
  }

  // 如果是字符串，直接使用（外部 URL）
  return props.source
}

/** 加载 PDF */
function loadPdf() {
  // 清除之前的错误和加载状态
  error.value = null
  loading.value = false
  isLoaded.value = false

  // 清除超时定时器
  if (loadTimeout) {
    clearTimeout(loadTimeout)
    loadTimeout = null
  }

  // 创建 PDF 源 URL
  const src = createPdfSource()
  pdfSrc.value = src

  if (!src) {
    // 无源，空状态
    return
  }

  // 设置加载状态
  loading.value = true

  // 设置超时检测
  loadTimeout = setTimeout(() => {
    if (!isLoaded.value && loading.value) {
      const err = new Error('PDF 加载超时，请检查网络或文件是否可访问')
      error.value = err
      loading.value = false
      emit('error', err)
    }
  }, props.timeout)
}

/** 重置状态（用于 source 变化或重试） */
function resetState() {
  // 清除超时
  if (loadTimeout) {
    clearTimeout(loadTimeout)
    loadTimeout = null
  }

  // 重置加载状态
  loading.value = false
  error.value = null
  isLoaded.value = false
}

/** 清理 Blob URL */
function cleanupBlobUrl() {
  if (selfCreatedBlobUrl.value) {
    URL.revokeObjectURL(selfCreatedBlobUrl.value)
    selfCreatedBlobUrl.value = null
  }
}

// ---------- iframe 事件处理 ----------
function handleIframeLoad() {
  // iframe load 事件触发，表示 PDF 已加载完成
  if (!loading.value) return

  loading.value = false
  isLoaded.value = true

  if (loadTimeout) {
    clearTimeout(loadTimeout)
    loadTimeout = null
  }

  const src = pdfSrc.value || ''
  emit('load', { url: src, fileName: displayFileName.value })
}

function handleIframeError(event: Event) {
  // iframe 加载失败
  loading.value = false
  const err = new Error('PDF 加载失败，请检查文件是否有效')
  error.value = err
  emit('error', err)

  if (loadTimeout) {
    clearTimeout(loadTimeout)
    loadTimeout = null
  }
}

// ---------- 用户操作 ----------
function handleRetry() {
  // 清除错误状态
  error.value = null
  loading.value = false
  isLoaded.value = false
  emit('retry')

  // 清除旧的 iframe 状态
  if (iframeRef.value) {
    // 强制刷新 iframe
    const currentSrc = pdfSrc.value
    if (currentSrc) {
      // 先清空再重新设置，强制 reload
      iframeRef.value.src = 'about:blank'
      nextTick(() => {
        if (iframeRef.value) {
          iframeRef.value.src = currentSrc
        }
      })
    } else {
      // 重新加载
      loadPdf()
    }
  } else {
    loadPdf()
  }
}

function handleDownload() {
  if (!pdfSrc.value) return

  const url = pdfSrc.value
  // 如果是 blob URL，可以直接使用；如果是外部 URL，需要 fetch 下来再下载
  // 简单起见，直接使用 a 标签的 download 属性
  const link = document.createElement('a')
  link.href = url
  link.download = displayFileName.value
  link.target = '_blank'
  document.body.appendChild(link)
  link.click()
  document.body.removeChild(link)

  emit('download', { url, fileName: displayFileName.value })
}

function handleOpen() {
  if (!pdfSrc.value) return
  const url = pdfSrc.value
  window.open(url, '_blank')
  emit('open', { url, fileName: displayFileName.value })
}

// ---------- 监听 source 变化 ----------
watch(
  () => props.source,
  (newVal, oldVal) => {
    // 如果新旧都是 Blob，但引用相同，不触发重新加载
    if (newVal instanceof Blob && oldVal instanceof Blob && newVal === oldVal) {
      return
    }

    // 重置状态
    resetState()

    // 如果 source 变化，需要清理旧的 Blob URL（组件自己创建的）
    cleanupBlobUrl()

    // 如果新 source 为空，清空 pdfSrc
    if (!newVal) {
      pdfSrc.value = null
      return
    }

    // 加载新 PDF
    loadPdf()
  },
  { immediate: true }
)

// ---------- 组件卸载时清理 ----------
onBeforeUnmount(() => {
  // 清除超时定时器
  if (loadTimeout) {
    clearTimeout(loadTimeout)
    loadTimeout = null
  }

  // 释放组件自己创建的 Blob URL
  cleanupBlobUrl()

  // 清空 iframe 的 src，释放资源
  if (iframeRef.value) {
    iframeRef.value.src = 'about:blank'
  }
})

// ---------- 暴露方法 ----------
defineExpose({
  /** 重新加载当前 PDF */
  reload: loadPdf,
  /** 获取当前 PDF 源 URL */
  getPdfSrc: () => pdfSrc.value,
  /** 获取加载状态 */
  getLoading: () => loading.value,
  /** 清除错误并重试 */
  retry: handleRetry,
})
</script>

<style scoped>
.pdf-preview-panel {
  position: relative;
  border: 1px solid #e5e7eb;
  border-radius: 8px;
  overflow: hidden;
  background: #f9fafb;
  display: flex;
  flex-direction: column;
}

/* iframe 占据剩余空间 */
.pdf-preview-panel__iframe {
  flex: 1;
  width: 100%;
  min-height: 0;
  background: #f9fafb;
}

/* 覆盖层（loading / error / empty） */
.pdf-preview-panel__overlay {
  position: absolute;
  inset: 0;
  display: flex;
  align-items: center;
  justify-content: center;
  background: rgba(249, 250, 251, 0.85);
  z-index: 2;
  border-radius: 8px;
}

/* 加载状态 */
.pdf-preview-panel__loading-content,
.pdf-preview-panel__error-content,
.pdf-preview-panel__empty-content {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
  padding: 24px;
  font-size: 14px;
  color: #6b7280;
}

/* 加载 Spinner */
.pdf-preview-panel__spinner {
  width: 36px;
  height: 36px;
  border: 3px solid #e5e7eb;
  border-top-color: #409eff;
  border-radius: 50%;
  animation: pdf-preview-spin 0.8s linear infinite;
}

@keyframes pdf-preview-spin {
  to {
    transform: rotate(360deg);
  }
}

.pdf-preview-panel__error-icon,
.pdf-preview-panel__empty-icon {
  font-size: 36px;
}

.pdf-preview-panel__retry-btn {
  padding: 6px 20px;
  background: #409eff;
  color: #fff;
  border: none;
  border-radius: 6px;
  cursor: pointer;
  font-size: 13px;
  transition: background 0.2s;
}

.pdf-preview-panel__retry-btn:hover {
  background: #3a8ee6;
}

/* 工具栏 */
.pdf-preview-panel__toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 8px 16px;
  border-top: 1px solid #e5e7eb;
  background: #ffffff;
  flex-shrink: 0;
  gap: 12px;
  min-height: 44px;
}

.pdf-preview-panel__filename {
  font-size: 13px;
  color: #374151;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
  flex: 1;
}

.pdf-preview-panel__actions {
  display: flex;
  gap: 8px;
  flex-shrink: 0;
}

.pdf-preview-panel__action-btn {
  padding: 4px 12px;
  font-size: 12px;
  background: transparent;
  border: 1px solid #e5e7eb;
  border-radius: 4px;
  cursor: pointer;
  color: #4b5563;
  transition: all 0.2s;
}

.pdf-preview-panel__action-btn:hover {
  background: #f3f4f6;
  border-color: #d1d5db;
}

.pdf-preview-panel__action-btn:active {
  background: #e5e7eb;
}
</style>