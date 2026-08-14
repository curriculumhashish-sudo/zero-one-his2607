<template>
  <section class="action-toolbar" :class="{ 'is-compact': compact }">
    <div class="action-toolbar__main">
      <slot name="left">
        <div v-if="title || description" class="action-toolbar__title-wrap">
          <strong v-if="title" class="action-toolbar__title">{{ title }}</strong>
          <span v-if="description" class="action-toolbar__description">{{ description }}</span>
        </div>
      </slot>

      <span v-if="showSelection" class="action-toolbar__selection">
        已选 <strong>{{ selectionCount }}</strong> 项
      </span>
    </div>

    <div class="action-toolbar__actions">
      <slot name="actions">
        <el-button
          v-for="action in visibleActions"
          :key="action.key"
          :type="buttonType(action.type)"
          :plain="action.plain"
          :text="action.text"
          :disabled="disabled || action.disabled"
          :loading="loading || action.loading"
          @click="triggerAction(action)"
        >
          <el-icon v-if="action.icon">
            <component :is="action.icon" />
          </el-icon>
          {{ action.label }}
        </el-button>

        <el-dropdown
          v-if="overflowActions.length > 0"
          :disabled="disabled || loading"
          trigger="click"
          @command="triggerOverflowAction"
        >
          <el-button :disabled="disabled || loading">
            更多
            <el-icon class="el-icon--right"><ArrowDown /></el-icon>
          </el-button>
          <template #dropdown>
            <el-dropdown-menu>
              <el-dropdown-item
                v-for="action in overflowActions"
                :key="action.key"
                :command="action.key"
                :disabled="action.disabled"
                :divided="action.key === overflowActions[0]?.key && visibleActions.length > 0"
              >
                {{ action.label }}
              </el-dropdown-item>
            </el-dropdown-menu>
          </template>
        </el-dropdown>
      </slot>
    </div>
  </section>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { ArrowDown } from '@element-plus/icons-vue'
import { ElMessageBox } from 'element-plus'
import type { ActionToolbarItem, CommonComponentKey } from './type'

interface ActionToolbarProps {
  title?: string
  description?: string
  actions?: ActionToolbarItem[]
  selectionCount?: number
  maxVisible?: number
  loading?: boolean
  disabled?: boolean
  compact?: boolean
}

const props = withDefaults(defineProps<ActionToolbarProps>(), {
  title: '',
  description: '',
  actions: () => [],
  selectionCount: 0,
  maxVisible: 4,
  loading: false,
  disabled: false,
  compact: false
})

const emit = defineEmits<{
  action: [action: ActionToolbarItem]
  click: [key: CommonComponentKey, action: ActionToolbarItem]
}>()

const visibleCount = computed(() => Math.max(0, props.maxVisible))
const visibleActions = computed(() => props.actions.slice(0, visibleCount.value))
const overflowActions = computed(() => props.actions.slice(visibleCount.value))
const showSelection = computed(() => props.selectionCount > 0)

function buttonType(type: ActionToolbarItem['type']) {
  return type === 'default' ? undefined : type
}

async function triggerAction(action: ActionToolbarItem) {
  if (props.disabled || props.loading || action.disabled || action.loading) return
  if (action.confirmText) {
    try {
      await ElMessageBox.confirm(action.confirmText, '操作确认', { type: 'warning' })
    } catch {
      return
    }
  }
  emit('action', action)
  emit('click', action.key, action)
}

function triggerOverflowAction(key: CommonComponentKey) {
  const action = overflowActions.value.find((item) => item.key === key)
  if (action) void triggerAction(action)
}
</script>

<style scoped>
.action-toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  min-width: 0;
  padding: 12px 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.action-toolbar.is-compact {
  padding: 8px 12px;
}

.action-toolbar__main {
  display: flex;
  align-items: center;
  min-width: 0;
  gap: 12px;
}

.action-toolbar__title-wrap {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 4px;
}

.action-toolbar__title {
  overflow: hidden;
  color: var(--el-text-color-primary);
  font-size: 14px;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.action-toolbar__description,
.action-toolbar__selection {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.action-toolbar__selection strong {
  color: var(--el-color-primary);
}

.action-toolbar__actions {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  gap: 8px;
}

@media (max-width: 768px) {
  .action-toolbar {
    align-items: stretch;
    flex-direction: column;
  }

  .action-toolbar__actions {
    flex-wrap: wrap;
  }
}
</style>
