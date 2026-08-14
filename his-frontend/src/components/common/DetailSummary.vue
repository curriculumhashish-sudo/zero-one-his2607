<template>
  <section class="detail-summary" :aria-busy="loading">
    <header v-if="title || $slots.title || $slots.extra" class="detail-summary__header">
      <div class="detail-summary__title">
        <slot name="title">{{ title }}</slot>
      </div>
      <div v-if="$slots.extra" class="detail-summary__extra">
        <slot name="extra" />
      </div>
    </header>

    <div v-loading="loading" class="detail-summary__body">
      <EmptyState
        v-if="!loading && items.length === 0"
        compact
        :title="emptyDescription"
      />
      <el-descriptions
        v-else-if="!loading"
        :column="column"
        :border="border"
        :direction="direction"
      >
        <el-descriptions-item
          v-for="item in items"
          :key="item.key"
          :label="item.label"
          :span="Math.max(1, item.span ?? 1)"
          :label-width="labelWidth"
        >
          <slot name="value" :item="item">
            <slot :name="`value-${item.key}`" :item="item">
              <span :class="{ 'is-empty': item.value == null || item.value === '' }">
                {{ item.value == null || item.value === '' ? emptyValue : item.value }}
              </span>
            </slot>
          </slot>
        </el-descriptions-item>
      </el-descriptions>
    </div>
  </section>
</template>

<script setup lang="ts">
import EmptyState from './EmptyState.vue'
import type { DetailSummaryItem } from './type'

interface DetailSummaryProps {
  title?: string
  items?: DetailSummaryItem[]
  column?: number
  border?: boolean
  direction?: 'horizontal' | 'vertical'
  labelWidth?: string | number
  loading?: boolean
  emptyDescription?: string
  emptyValue?: string
}

withDefaults(defineProps<DetailSummaryProps>(), {
  title: '',
  items: () => [],
  column: 4,
  border: true,
  direction: 'horizontal',
  labelWidth: '',
  loading: false,
  emptyDescription: '暂无摘要信息',
  emptyValue: '—'
})
</script>

<style scoped>
.detail-summary {
  box-sizing: border-box;
  min-width: 0;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.detail-summary__header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  min-height: 44px;
  padding: 0 16px;
  border-bottom: 1px solid var(--el-border-color-light);
}

.detail-summary__title {
  min-width: 0;
  overflow: hidden;
  color: var(--el-text-color-primary);
  font-size: 15px;
  font-weight: 600;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.detail-summary__extra {
  flex-shrink: 0;
}

.detail-summary__body {
  position: relative;
  min-height: 72px;
  padding: 12px 16px 16px;
}

.detail-summary__body :deep(.el-descriptions__label) {
  color: var(--el-text-color-secondary);
}

.is-empty {
  color: var(--el-text-color-placeholder);
}
</style>
