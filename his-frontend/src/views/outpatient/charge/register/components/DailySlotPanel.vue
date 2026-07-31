<script setup lang="ts">
import type { BookableSlotHint } from '@/apis/outpatient/charge/register'

import EmptyState from '@/components/common/EmptyState.vue'
import { slotTimeLabel } from '../adapter'

defineProps<{ loading: boolean; selectedId: string; slots: BookableSlotHint[] }>()
const emit = defineEmits<{ refresh: []; select: [slot: BookableSlotHint] }>()
</script>

<template>
  <section v-loading="loading" class="slot-panel">
    <header>
      <strong>当日号源</strong
      ><el-button type="primary" link @click="emit('refresh')">刷新</el-button>
    </header>
    <EmptyState
      v-if="!loading && slots.length === 0"
      compact
      title="暂无可用号源"
      description="请调整科室、医生或服务类型"
    />
    <div v-else class="slot-list">
      <button
        v-for="slot in slots"
        :key="String(slot.slotId ?? slot.slotStart ?? '')"
        type="button"
        class="slot-item"
        :class="{ 'is-selected': selectedId === String(slot.slotId ?? '') }"
        :disabled="(slot.remainingCount ?? 0) <= 0"
        @click="emit('select', slot)"
      >
        <span
          ><strong>{{ slotTimeLabel(slot) }}</strong></span
        >
        <el-tag :type="(slot.remainingCount ?? 0) > 0 ? 'success' : 'danger'"
          >余 {{ slot.remainingCount ?? 0 }}</el-tag
        >
      </button>
    </div>
  </section>
</template>

<style scoped>
.slot-panel {
  min-height: 180px;
}
.slot-panel header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 10px;
}
.slot-list {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(190px, 1fr));
  gap: 8px;
}
.slot-item {
  display: flex;
  min-height: 62px;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  padding: 10px;
  border: 1px solid var(--el-border-color);
  border-radius: 6px;
  color: var(--el-text-color-primary);
  background: var(--el-bg-color);
  cursor: pointer;
  text-align: left;
}
.slot-item:hover,
.slot-item.is-selected {
  border-color: var(--el-color-primary);
  background: var(--el-color-primary-light-9);
}
.slot-item span {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 4px;
}
.slot-item small {
  overflow: hidden;
  color: var(--el-text-color-secondary);
  text-overflow: ellipsis;
  white-space: nowrap;
}
</style>
