<template>
  <span class="status-display">
    <slot :option="resolvedOption">
      <el-tag
        :type="resolvedOption.type"
        :effect="resolvedOption.effect"
        :size="size"
        :round="round"
      >
        {{ resolvedOption.label }}
      </el-tag>
      <span v-if="showDescription && resolvedOption.description" class="status-display__desc">
        {{ resolvedOption.description }}
      </span>
    </slot>
  </span>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import type { CommonComponentKey, StatusDisplayOption } from './type'

interface StatusDisplayProps {
  value?: CommonComponentKey | boolean | null
  options?: StatusDisplayOption[]
  fallbackLabel?: string
  size?: 'large' | 'default' | 'small'
  effect?: 'dark' | 'light' | 'plain'
  type?: 'primary' | 'success' | 'warning' | 'danger' | 'info'
  round?: boolean
  showDescription?: boolean
}

const props = withDefaults(defineProps<StatusDisplayProps>(), {
  value: null,
  options: () => [],
  fallbackLabel: '未知状态',
  size: 'small',
  effect: 'plain',
  type: 'info',
  round: false,
  showDescription: false
})

const resolvedOption = computed<Required<Pick<StatusDisplayOption, 'label' | 'effect' | 'type'>> & StatusDisplayOption>(() => {
  const matched = props.options.find((item) => item.value === props.value)
  return {
    value: props.value ?? '',
    label: matched?.label ?? props.fallbackLabel,
    type: matched?.type ?? props.type,
    effect: matched?.effect ?? props.effect,
    description: matched?.description ?? ''
  }
})
</script>

<style scoped>
.status-display {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  min-width: 0;
  vertical-align: middle;
}

.status-display__desc {
  overflow: hidden;
  color: var(--el-text-color-secondary);
  font-size: 12px;
  text-overflow: ellipsis;
  white-space: nowrap;
}
</style>
