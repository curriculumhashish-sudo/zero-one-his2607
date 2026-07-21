<script setup lang="ts">
import { reactive, ref } from 'vue'
import { Refresh } from '@element-plus/icons-vue'

import DateRangeField from '@/components/common/DateRangeField.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import type { DateRangeValue } from '@/components/common/type.ts'

import PatientCard from './PatientCard.vue'
import type { PatientSummary } from './type.ts'

defineOptions({
  name: 'PatientQueuePanel'
})

interface PatientQueuePanelProps {
  patients?: PatientSummary[]
  loading?: boolean
  selectedId?: string | number | null
}

withDefaults(defineProps<PatientQueuePanelProps>(), {
  patients: () => [],
  loading: false,
  selectedId: null
})

const emit = defineEmits<{
  search: [keyword: string]
  dateChange: [value: DateRangeValue]
  refresh: []
  select: [patient: PatientSummary]
}>()

const searchModel = reactive({
  keyword: ''
})
const dateRange = ref<DateRangeValue>(null)

function handleSearch() {
  emit('search', searchModel.keyword.trim())
}

function handleReset() {
  searchModel.keyword = ''
  dateRange.value = null
  emit('search', '')
  emit('dateChange', null)
}

function handleDateChange(value: DateRangeValue) {
  emit('dateChange', value)
}

function handleRefresh() {
  emit('refresh')
}

function handleSelect(patient: PatientSummary) {
  emit('select', patient)
}
</script>

<template>
  <section class="patient-queue-panel" aria-label="患者队列">
    <div class="patient-queue-panel__search">
      <SearchPanel
        :model="searchModel"
        :loading="loading"
        label-width="72px"
        :submit-on-enter="true"
        @search="handleSearch"
        @reset="handleReset"
      >
        <el-form-item label="关键词">
          <el-input v-model="searchModel.keyword" clearable placeholder="姓名/患者号/就诊号" />
        </el-form-item>

        <el-form-item label="日期">
          <DateRangeField v-model="dateRange" :show-options="false" @change="handleDateChange" />
        </el-form-item>
      </SearchPanel>
    </div>

    <header class="patient-queue-panel__toolbar">
      <div class="patient-queue-panel__title">
        <span>患者队列</span>
        <el-tag size="small" effect="plain">{{ patients.length }}</el-tag>
      </div>

      <el-button
        :icon="Refresh"
        circle
        size="small"
        :loading="loading"
        aria-label="刷新患者队列"
        title="刷新患者队列"
        @click="handleRefresh"
      />
    </header>

    <div v-loading="loading" class="patient-queue-panel__list" :aria-busy="loading">
      <EmptyState
        v-if="!loading && patients.length === 0"
        compact
        title="暂无患者"
        description="当前查询条件下没有可展示的患者"
        show-refresh
        @refresh="handleRefresh"
      />

      <div v-else-if="patients.length" class="patient-queue-panel__cards">
        <PatientCard
          v-for="patient in patients"
          :key="patient.id"
          :patient="patient"
          :selected="patient.id === selectedId"
          @select="handleSelect"
          @view="handleSelect"
        />
      </div>
    </div>
  </section>
</template>

<style scoped>
.patient-queue-panel {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  height: 100%;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  background: var(--el-bg-color);
}

.patient-queue-panel__search {
  flex-shrink: 0;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.patient-queue-panel__search :deep(.query-shell) {
  padding: 12px;
  border-radius: 0;
  box-shadow: none;
}

.patient-queue-panel__search :deep(.query-shell:hover) {
  box-shadow: none;
}

.patient-queue-panel__search :deep(.query-shell__actions) {
  gap: 8px;
  margin-top: 8px;
  padding-top: 8px;
}

.patient-queue-panel__search :deep(.query-shell__btn) {
  min-width: 0;
}

.patient-queue-panel__search :deep(.el-form-item) {
  margin-bottom: 8px;
}

.patient-queue-panel__search :deep(.el-form-item__label) {
  white-space: nowrap;
}

.patient-queue-panel__search :deep(.el-form-item:last-child) {
  margin-bottom: 0;
}

.patient-queue-panel__search :deep(.date-range-field__picker) {
  width: 100%;
}

.patient-queue-panel__toolbar {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: space-between;
  min-height: 44px;
  padding: 6px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.patient-queue-panel__title {
  display: flex;
  align-items: center;
  gap: 8px;
  color: var(--el-text-color-primary);
  font-size: 14px;
  font-weight: 600;
}

.patient-queue-panel__list {
  position: relative;
  flex: 1;
  min-height: 120px;
  overflow-y: auto;
  overscroll-behavior: contain;
}

.patient-queue-panel__cards {
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 10px;
}
</style>
