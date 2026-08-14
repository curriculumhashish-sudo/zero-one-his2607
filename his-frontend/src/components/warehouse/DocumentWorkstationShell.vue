<template>
  <div
    class="document-workstation"
    :class="{ 'document-workstation--readonly': readonly }"
    :style="rootStyle"
  >
    <!-- ==================== Queue 区（左侧单据列表） ==================== -->
    <aside ref="queueRef" class="document-workstation__queue" :style="queueStyle">
      <header v-if="$slots['queue-header']" class="document-workstation__queue-header">
        <div class="document-workstation__queue-header-content">
          <slot name="queue-header" />
        </div>
      </header>

      <div class="document-workstation__queue-body">
        <slot name="queue" />
      </div>
    </aside>

    <!-- ==================== 拖拽分隔条 ==================== -->
    <div class="document-workstation__resizer" @mousedown="onResizeStart" />

    <!-- ==================== Main 区（右侧主区域） ==================== -->
    <main class="document-workstation__main">
      <!-- 无选中单据：展示 empty slot -->
      <template v-if="!hasSelection">
        <div class="document-workstation__empty">
          <slot name="empty">
            <el-empty description="请选择一个单据" />
          </slot>
        </div>
      </template>

      <!-- 有选中单据：展示 main slot -->
      <template v-else>
        <slot name="main" />
      </template>
    </main>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onUnmounted, watch } from 'vue'

// ==================== Props ====================
interface DocumentWorkstationProps {
  /** 左侧 queue 区宽度（支持 v-model:queueWidth） */
  queueWidth?: string | number
  /** 是否有选中的单据 */
  hasSelection?: boolean
  /** 是否处于只读模式 */
  readonly?: boolean
  /** queue 区与 main 区之间的间距 */
  gap?: string | number
  /** 最小拖拽宽度 */
  minQueueWidth?: number
  /** 最大拖拽宽度 */
  maxQueueWidth?: number
}

const props = withDefaults(defineProps<DocumentWorkstationProps>(), {
  queueWidth: 320,
  hasSelection: false,
  readonly: false,
  gap: 0,
  minQueueWidth: 200,
  maxQueueWidth: 600
})

// ==================== Emits ====================
const emit = defineEmits<{
  'update:queueWidth': [value: number]
}>()

// ==================== 内部拖拽状态 ====================
const queueRef = ref<HTMLElement | null>(null)
const queueWidthValue = ref<string | number>(props.queueWidth)
const dragging = ref(false)
let startX = 0
let startWidth = 0

// ==================== Computed ====================
const rootStyle = computed(() => ({
  '--dw-gap': typeof props.gap === 'number' ? `${props.gap}px` : props.gap
}))

const queueStyle = computed(() => ({
  width:
    typeof queueWidthValue.value === 'number' ? `${queueWidthValue.value}px` : queueWidthValue.value
}))

watch(
  () => props.queueWidth,
  (value) => {
    if (!dragging.value) queueWidthValue.value = value
  }
)

// ==================== 拖拽逻辑 ====================
function onResizeStart(e: MouseEvent) {
  e.preventDefault()
  dragging.value = true
  startX = e.clientX
  startWidth = queueRef.value?.getBoundingClientRect().width ?? props.minQueueWidth
  document.addEventListener('mousemove', onResizing)
  document.addEventListener('mouseup', onResizeEnd)
}

function onResizing(e: MouseEvent) {
  if (!dragging.value) return
  const delta = e.clientX - startX
  const next = Math.min(props.maxQueueWidth, Math.max(props.minQueueWidth, startWidth + delta))
  queueWidthValue.value = next
}

function onResizeEnd() {
  dragging.value = false
  emit(
    'update:queueWidth',
    typeof queueWidthValue.value === 'number' ? queueWidthValue.value : startWidth
  )
  document.removeEventListener('mousemove', onResizing)
  document.removeEventListener('mouseup', onResizeEnd)
}

onUnmounted(() => {
  document.removeEventListener('mousemove', onResizing)
  document.removeEventListener('mouseup', onResizeEnd)
})
</script>

<style scoped>
/* ==================== 根容器 ==================== */
.document-workstation {
  display: flex;
  width: 100%;
  height: 100%;
  min-height: 0;
  gap: var(--dw-gap, 0);
  box-sizing: border-box;
}

/* ==================== Queue 区 ==================== */
.document-workstation__queue {
  display: flex;
  flex: 0 0 auto;
  flex-direction: column;
  min-height: 0;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
  overflow: hidden;
}

.document-workstation__queue-header {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  min-height: 48px;
  padding: 0 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.document-workstation__queue-header-content {
  display: flex;
  font-size: 16px;
  font-weight: 600;
  align-items: center;
  color: var(--el-text-color-primary);
  min-width: 0;
  flex: 1;
  overflow: hidden;
}

.document-workstation__queue-body {
  flex: 1;
  min-height: 0;
  overflow: hidden auto;
}

/* ==================== 拖拽分隔条 ==================== */
.document-workstation__resizer {
  flex: 0 0 4px;
  cursor: col-resize;
  background: transparent;
  transition: background 0.15s;
  user-select: none;
}

.document-workstation__resizer:hover {
  background: var(--el-color-primary-light-5);
}

/* ==================== Main 区 ==================== */
.document-workstation__main {
  display: flex;
  flex: 1;
  flex-direction: column;
  min-width: 0;
  min-height: 0;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
  overflow: hidden;
}

/* ---------- 空状态 ---------- */
.document-workstation__empty {
  display: flex;
  flex: 1;
  align-items: center;
  justify-content: center;
  min-height: 0;
}
</style>
