```vue
<template>
  <div
    class="document-workstation"
    :class="{
      'document-workstation--readonly': readonly,
      'document-workstation--queue-hidden': queueCollapsed
    }"
    :style="rootStyle"
  >
    <!-- ==================== Queue 区（左侧单据列表） ==================== -->
    <aside v-show="!queueCollapsed" class="document-workstation__queue" :style="queueStyle">
      <header
        v-if="$slots['queue-header'] || collapsible"
        class="document-workstation__queue-header"
      >
        <div class="document-workstation__queue-header-content">
          <slot name="queue-header" />
        </div>

        <el-button
          v-if="collapsible"
          class="document-workstation__queue-toggle"
          link
          :aria-label="'收起单据列表'"
          @click="toggleQueue"
        >
          <el-icon><DArrowLeft /></el-icon>
        </el-button>
      </header>

      <div class="document-workstation__queue-body">
        <slot name="queue" />
      </div>
    </aside>

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

      <!-- 有选中单据：展示 header + content + footer -->
      <template v-else>
        <header v-if="$slots['main-header']" class="document-workstation__main-header">
          <slot name="main-header" />
        </header>

        <div
          class="document-workstation__main-content"
          :class="{ 'is-readonly': readonly }"
          v-loading="loading"
          element-loading-text="加载中，请稍候..."
        >
          <slot name="main-content" />
        </div>

        <footer v-if="$slots['main-footer']" class="document-workstation__main-footer">
          <slot name="main-footer" />
        </footer>
      </template>
    </main>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { DArrowLeft } from '@element-plus/icons-vue'

// ==================== Props ====================
interface DocumentWorkstationProps {
  /** 左侧 queue 区宽度 */
  queueWidth?: string | number
  /** 左侧 queue 区是否收起（支持 v-model:queueCollapsed） */
  queueCollapsed?: boolean
  /** 是否允许收起/展开左侧 queue 区 */
  collapsible?: boolean
  /** 是否有选中的单据 */
  hasSelection?: boolean
  /** 是否处于加载状态 */
  loading?: boolean
  /** 是否处于只读模式 */
  readonly?: boolean
  /** queue 区与 main 区之间的间距 */
  gap?: string | number
}

const props = withDefaults(defineProps<DocumentWorkstationProps>(), {
  queueWidth: 320,
  queueCollapsed: false,
  collapsible: true,
  hasSelection: false,
  loading: false,
  readonly: false,
  gap: 0
})

// ==================== Emits ====================
const emit = defineEmits<{
  'update:queueCollapsed': [value: boolean]
  collapseChange: [collapsed: boolean]
}>()

// ==================== Computed ====================
/** 容器 CSS 变量（gap） */
const rootStyle = computed(() => ({
  '--dw-gap': typeof props.gap === 'number' ? `${props.gap}px` : props.gap
}))

/** queue 区宽度 */
const queueStyle = computed(() => ({
  width: typeof props.queueWidth === 'number' ? `${props.queueWidth}px` : props.queueWidth
}))

// ==================== Methods ====================
function toggleQueue() {
  const next = !props.queueCollapsed
  emit('update:queueCollapsed', next)
  emit('collapseChange', next)
}
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
  transition: width 0.25s ease;
}

.document-workstation__queue-header {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  min-height: 48px;
  padding: 0 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.document-workstation__queue-header-content {
  display: flex;
  align-items: center;
  min-width: 0;
  flex: 1;
  overflow: hidden;
}

.document-workstation__queue-toggle {
  flex-shrink: 0;
}

.document-workstation__queue-body {
  flex: 1;
  min-height: 0;
  overflow: hidden auto;
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

/* ---------- Main Header ---------- */
.document-workstation__main-header {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  min-height: 48px;
  padding: 0 16px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

/* ---------- Main Content ---------- */
.document-workstation__main-content {
  position: relative;
  flex: 1;
  min-height: 0;
  overflow: hidden auto;
  padding: 16px;
}

.document-workstation__main-content.is-readonly {
  pointer-events: none;
  opacity: 0.7;
}

/* ---------- Main Footer ---------- */
.document-workstation__main-footer {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: flex-end;
  gap: 8px;
  min-height: 56px;
  padding: 0 16px;
  border-top: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

/* ==================== 收起状态：queue 隐藏 ==================== */
.document-workstation--queue-hidden .document-workstation__queue {
  display: none;
}
</style>
```

