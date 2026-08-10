<script setup lang="ts">
import { Search } from '@element-plus/icons-vue'

import type { InpatientDiagnosisUsageHistoryVO } from '@/apis/inpatient/doctor-station'
import SidePanelShell from '@/components/common/SidePanelShell.vue'

defineOptions({
  name: 'InpatientDiagnosisHistoryPanel'
})

const props = withDefaults(
  defineProps<{
    rows?: InpatientDiagnosisUsageHistoryVO[]
    existingCatalogIds?: string[]
    loading?: boolean
    error?: string
    keyword?: string
    readonly?: boolean
  }>(),
  {
    rows: () => [],
    existingCatalogIds: () => [],
    loading: false,
    error: '',
    keyword: '',
    readonly: false
  }
)

const emit = defineEmits<{
  'update:keyword': [value: string]
  search: [keyword: string]
  add: [diagnosis: InpatientDiagnosisUsageHistoryVO]
  retry: []
}>()

function isAdded(diagnosis: InpatientDiagnosisUsageHistoryVO) {
  return props.existingCatalogIds.includes(diagnosis.id)
}
</script>

<template>
  <SidePanelShell
    title="诊断使用历史"
    width="100%"
    :loading="loading"
    :empty="!error && rows.length === 0"
    :empty-description="keyword ? '暂无匹配的常用诊断' : '暂无诊断使用历史'"
  >
    <template #search>
      <el-input
        :model-value="keyword"
        clearable
        placeholder="诊断编码/诊断名称"
        @update:model-value="emit('update:keyword', String($event))"
        @keyup.enter="emit('search', keyword)"
        @clear="emit('search', '')"
      >
        <template #append>
          <el-button
            :icon="Search"
            aria-label="搜索诊断使用历史"
            @click="emit('search', keyword)"
          />
        </template>
      </el-input>
    </template>

    <div v-if="error" class="diagnosis-history__error">
      <span>{{ error }}</span>
      <el-button link type="primary" @click="emit('retry')">重试</el-button>
    </div>

    <ul v-else class="diagnosis-history__list">
      <li v-for="diagnosis in rows" :key="diagnosis.id" class="diagnosis-history__item">
        <div class="diagnosis-history__heading">
          <span class="diagnosis-history__name">{{ diagnosis.name }}</span>
          <el-tag size="small" effect="plain">{{ diagnosis.code }}</el-tag>
        </div>
        <div class="diagnosis-history__meta">
          <span>{{ diagnosis.typeName }}</span>
          <span>使用 {{ diagnosis.useCount }} 次</span>
        </div>
        <div class="diagnosis-history__footer">
          <span>{{ diagnosis.lastUsedAt }}</span>
          <el-button
            link
            type="primary"
            :disabled="readonly || isAdded(diagnosis)"
            @click="emit('add', diagnosis)"
          >
            {{ isAdded(diagnosis) ? '已添加' : '加入诊断' }}
          </el-button>
        </div>
      </li>
    </ul>

    <template #footer>
      <span class="diagnosis-history__hint">
        {{ readonly ? '待出院患者的诊断仅供查看' : '可将历史诊断快速加入本次住院诊断' }}
      </span>
    </template>
  </SidePanelShell>
</template>

<style scoped>
.diagnosis-history__list {
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 0;
  margin: 0;
  list-style: none;
}

.diagnosis-history__item {
  display: flex;
  flex-direction: column;
  gap: 8px;
  padding: 12px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.diagnosis-history__heading,
.diagnosis-history__meta,
.diagnosis-history__footer,
.diagnosis-history__error {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
}

.diagnosis-history__name {
  min-width: 0;
  overflow: hidden;
  color: var(--el-text-color-primary, #303133);
  font-weight: 600;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.diagnosis-history__meta,
.diagnosis-history__footer,
.diagnosis-history__hint {
  color: var(--el-text-color-secondary, #909399);
  font-size: 12px;
}

.diagnosis-history__error {
  color: var(--el-color-danger);
  font-size: 13px;
}
</style>
