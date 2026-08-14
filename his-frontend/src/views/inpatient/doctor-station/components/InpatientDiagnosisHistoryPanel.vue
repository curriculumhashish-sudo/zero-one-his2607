<script setup lang="ts">
import type { InpatientDiagnosisBriefVO } from '@/apis/inpatient/doctor-station'
import SidePanelShell from '@/components/common/SidePanelShell.vue'

defineOptions({
  name: 'InpatientDiagnosisHistoryPanel'
})

const props = withDefaults(
  defineProps<{
    rows?: InpatientDiagnosisBriefVO[]
    existingDescriptions?: string[]
    loading?: boolean
    error?: string
    readonly?: boolean
  }>(),
  {
    rows: () => [],
    existingDescriptions: () => [],
    loading: false,
    error: '',
    readonly: false
  }
)

const emit = defineEmits<{
  add: [diagnosis: InpatientDiagnosisBriefVO]
  retry: []
}>()

function isAdded(diagnosis: InpatientDiagnosisBriefVO) {
  return props.existingDescriptions.includes(diagnosis.definitionName)
}
</script>

<template>
  <SidePanelShell
    title="历史诊断"
    width="100%"
    :loading="loading"
    :empty="!error && rows.length === 0"
    empty-description="暂无历史诊断"
  >
    <div v-if="error" class="diagnosis-history__error">
      <span>{{ error }}</span>
      <el-button link type="primary" @click="emit('retry')">重试</el-button>
    </div>

    <ul v-else class="diagnosis-history__list">
      <li v-for="diagnosis in rows" :key="diagnosis.definitionName" class="diagnosis-history__item">
        <span class="diagnosis-history__name">{{ diagnosis.definitionName }}</span>
        <el-button
          link
          type="primary"
          :disabled="readonly || isAdded(diagnosis)"
          @click="emit('add', diagnosis)"
        >
          {{ isAdded(diagnosis) ? '已加入' : '加入' }}
        </el-button>
      </li>
    </ul>

    <template #footer>
      <span class="diagnosis-history__hint">
        {{ readonly ? '当前患者的诊断仅供查看' : '可将历史诊断快速加入本次住院诊断' }}
      </span>
    </template>
  </SidePanelShell>
</template>

<style scoped>
.diagnosis-history__list {
  display: flex;
  flex-direction: column;
  gap: 8px;
  padding: 0;
  margin: 0;
  list-style: none;
}

.diagnosis-history__item,
.diagnosis-history__error {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
}

.diagnosis-history__item {
  padding: 10px 12px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.diagnosis-history__name {
  min-width: 0;
  overflow: hidden;
  color: var(--el-text-color-primary);
  font-size: 13px;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.diagnosis-history__error {
  color: var(--el-color-danger);
  font-size: 13px;
}

.diagnosis-history__hint {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}
</style>
