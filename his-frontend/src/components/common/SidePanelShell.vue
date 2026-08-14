<template>
  <aside
    class="side-panel-shell"
    :class="{ 'is-collapsed': collapsed }"
    :style="{ width: panelWidth }"
  >
    <header class="side-panel-shell__header">
      <div v-show="!collapsed" class="side-panel-shell__title">
        <slot name="title">
          {{ title }}
        </slot>
      </div>

      <el-button
        v-if="collapsible"
        class="side-panel-shell__collapse-button"
        link
        :aria-label="collapsed ? '展开侧栏' : '收起侧栏'"
        @click="toggleCollapsed"
      >
        {{ collapsed ? '展开' : '收起' }}
      </el-button>
    </header>

    <template v-if="!collapsed">
      <div v-if="$slots.search" class="side-panel-shell__search">
        <slot name="search" />
      </div>

      <div
        v-loading="loading"
        class="side-panel-shell__body"
        :class="{ 'is-loading': loading }"
      >
        <el-empty v-if="empty && !loading" :description="emptyDescription" />
        <slot v-else-if="!loading" />
      </div>

      <footer v-if="$slots.footer" class="side-panel-shell__footer">
        <slot name="footer" />
      </footer>
    </template>
  </aside>
</template>

<script setup lang="ts">
import { computed } from 'vue'

interface SidePanelShellProps {
  title?: string
  width?: string | number
  loading?: boolean
  empty?: boolean
  emptyDescription?: string
  collapsible?: boolean
  collapsed?: boolean
}

const props = withDefaults(defineProps<SidePanelShellProps>(), {
  title: '',
  width: 320,
  loading: false,
  empty: false,
  emptyDescription: '暂无数据',
  collapsible: false,
  collapsed: false
})

const emit = defineEmits<{
  'update:collapsed': [value: boolean]
}>()

const panelWidth = computed(() => {
  if (props.collapsed) return '48px'
  return typeof props.width === 'number' ? `${props.width}px` : props.width
})

function toggleCollapsed() {
  emit('update:collapsed', !props.collapsed)
}
</script>

<style scoped>
.side-panel-shell {
  display: flex;
  flex: 0 0 auto;
  flex-direction: column;
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  background: var(--el-bg-color);
  transition: width 0.2s ease;
}

.side-panel-shell__header {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: space-between;
  min-height: 48px;
  padding: 0 12px;
  border-bottom: 1px solid var(--el-border-color-light);
}

.side-panel-shell__title {
  min-width: 0;
  overflow: hidden;
  font-weight: 600;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.side-panel-shell__collapse-button {
  flex-shrink: 0;
}

.side-panel-shell__search {
  flex-shrink: 0;
  padding: 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.side-panel-shell__body {
  position: relative;
  flex: 1;
  min-height: 0;
  overflow: auto;
  padding: 12px;
}

.side-panel-shell__body.is-loading {
  min-height: 120px;
}

.side-panel-shell__footer {
  flex-shrink: 0;
  padding: 12px;
  border-top: 1px solid var(--el-border-color-light);
  background: var(--el-bg-color);
}

.side-panel-shell.is-collapsed .side-panel-shell__header {
  justify-content: center;
  padding: 0;
}
</style>
