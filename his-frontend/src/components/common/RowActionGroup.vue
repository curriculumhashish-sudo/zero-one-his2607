<template>
  <div class="row-action-group">
    <slot>
      <el-button
        v-for="action in visibleActions"
        :key="action.key"
        link
        size="small"
        :type="buttonType(action.type)"
        :disabled="disabled || action.disabled"
        @click="triggerAction(action)"
      >
        {{ action.label }}
      </el-button>

      <el-dropdown
        v-if="overflowActions.length > 0"
        :disabled="disabled"
        trigger="click"
        @command="triggerOverflowAction"
      >
        <el-button link size="small" :disabled="disabled">更多</el-button>
        <template #dropdown>
          <el-dropdown-menu>
            <el-dropdown-item
              v-for="action in overflowActions"
              :key="action.key"
              :command="action.key"
              :disabled="action.disabled"
              :divided="action.divided"
            >
              {{ action.label }}
            </el-dropdown-item>
          </el-dropdown-menu>
        </template>
      </el-dropdown>
    </slot>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { ElMessageBox } from 'element-plus'
import type { CommonComponentKey, RowActionItem } from './type'

interface RowActionGroupProps {
  actions?: RowActionItem[]
  row?: unknown
  maxVisible?: number
  disabled?: boolean
}

const props = withDefaults(defineProps<RowActionGroupProps>(), {
  actions: () => [],
  row: undefined,
  maxVisible: 3,
  disabled: false
})

const emit = defineEmits<{
  action: [action: RowActionItem, row: unknown]
  click: [key: CommonComponentKey, row: unknown, action: RowActionItem]
}>()

const visibleCount = computed(() => Math.max(0, props.maxVisible))
const visibleActions = computed(() => props.actions.slice(0, visibleCount.value))
const overflowActions = computed(() => props.actions.slice(visibleCount.value))

function buttonType(type: RowActionItem['type']) {
  return type === 'default' ? undefined : type
}

async function triggerAction(action: RowActionItem) {
  if (props.disabled || action.disabled) return
  if (action.confirmText) {
    try {
      await ElMessageBox.confirm(action.confirmText, '操作确认', { type: 'warning' })
    } catch {
      return
    }
  }
  emit('action', action, props.row)
  emit('click', action.key, props.row, action)
}

function triggerOverflowAction(key: CommonComponentKey) {
  const action = overflowActions.value.find((item) => item.key === key)
  if (action) void triggerAction(action)
}
</script>

<style scoped>
.row-action-group {
  display: inline-flex;
  align-items: center;
  gap: 8px;
  white-space: nowrap;
}
</style>
