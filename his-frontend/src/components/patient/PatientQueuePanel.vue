<script setup lang="ts">
import { computed, reactive, ref, watch } from 'vue'
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
  date?: string
  showDateFilter?: boolean
  /** 是否显示关键词输入框。 */
  showKeyword?: boolean
  /** 是否显示手动刷新入口。 */
  showRefresh?: boolean
  /** 是否在患者卡片中显示科室。 */
  showDepartment?: boolean
  /** 是否在患者卡片底部显示排队号或就诊号。 */
  showPrimaryNumber?: boolean
  /** 患者卡片左下角主编号的取值规则。 */
  primaryNumberMode?: 'auto' | 'patient-no'
  keywordPlaceholder?: string
  title?: string
  pagination?: {
    pageIndex: number
    pageSize: number
    total: number
  }
}

const props = withDefaults(defineProps<PatientQueuePanelProps>(), {
  patients: () => [],
  loading: false,
  selectedId: null,
  showDateFilter: true,
  showKeyword: true,
  showRefresh: true,
  showDepartment: true,
  showPrimaryNumber: true,
  primaryNumberMode: 'auto',
  keywordPlaceholder: '姓名/患者号/就诊号',
  title: '患者队列'
})

const emit = defineEmits<{
  search: [keyword: string]
  /** 日期范围模式事件，兼容收费、护士站等既有页面。 */
  dateChange: [value: DateRangeValue]
  /** 单日模式事件，供门诊医生站使用。 */
  singleDateChange: [value: string]
  pageChange: [pageIndex: number]
  refresh: []
  select: [patient: PatientSummary]
  view: [patient: PatientSummary]
}>()

const searchModel = reactive({
  keyword: ''
})
const dateRange = defineModel<DateRangeValue>('dateRange', { default: null })
const queryDate = ref(props.date ?? '')
const isSingleDateMode = computed(() => props.date !== undefined)

// 页面重置日期时同步更新输入框，保证界面条件与实际请求参数一致。
watch(
  () => props.date,
  (value) => {
    queryDate.value = value ?? ''
  }
)

function handleSearch() {
  emit('search', searchModel.keyword.trim())
}

function handleReset() {
  searchModel.keyword = ''
  emit('search', '')
  if (isSingleDateMode.value) {
    queryDate.value = ''
    emit('singleDateChange', '')
  } else {
    dateRange.value = null
    emit('dateChange', null)
  }
}

function handleSingleDateChange(value: unknown) {
  emit('singleDateChange', typeof value === 'string' ? value : '')
}

function handleDateRangeChange(value: DateRangeValue) {
  emit('dateChange', value)
}

function handleRefresh() {
  emit('refresh')
}

function handlePageChange(pageIndex: number) {
  emit('pageChange', pageIndex)
}

function handleSelect(patient: PatientSummary) {
  emit('select', patient)
}

function handleView(patient: PatientSummary) {
  emit('view', patient)
}
</script>

<template>
  <section class="patient-queue-panel" aria-label="患者队列">
    <div class="patient-queue-panel__search">
      <SearchPanel
        v-model:keyword="searchModel.keyword"
        :show-keyword="showKeyword"
        :keyword-placeholder="keywordPlaceholder"
        :loading="loading"
        :field-count="showDateFilter ? 1 : 0"
        :submit-on-enter="true"
        @search="handleSearch"
        @reset="handleReset"
      >
        <template #header>
          <div v-if="showDateFilter" class="query-shell__field">
            <label class="query-shell__label">日期</label>
            <el-date-picker
              v-if="isSingleDateMode"
              v-model="queryDate"
              type="date"
              value-format="YYYY-MM-DD"
              format="YYYY-MM-DD"
              :clearable="false"
              placeholder="选择就诊日期"
              @change="handleSingleDateChange"
            />
            <DateRangeField
              v-else
              v-model="dateRange"
              :show-options="false"
              @change="handleDateRangeChange"
            />
          </div>
        </template>
      </SearchPanel>
    </div>

    <header class="patient-queue-panel__toolbar">
      <div class="patient-queue-panel__title">
        <span>{{ title }}</span>
        <el-tag size="small" effect="plain">{{ pagination?.total ?? patients.length }}</el-tag>
      </div>

      <el-button
        v-if="showRefresh"
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
        :show-refresh="showRefresh"
        @refresh="handleRefresh"
      />

      <div v-else-if="patients.length" class="patient-queue-panel__cards">
        <PatientCard
          v-for="patient in patients"
          :key="patient.id"
          :patient="patient"
          :selected="patient.id === selectedId"
          :show-department="showDepartment"
          :show-primary-number="showPrimaryNumber"
          :primary-number-mode="primaryNumberMode"
          @select="handleSelect"
          @view="handleView"
        />
      </div>
    </div>

    <footer v-if="pagination" class="patient-queue-panel__pagination">
      <el-pagination
        small
        background
        layout="prev, pager, next"
        :current-page="pagination.pageIndex"
        :page-size="pagination.pageSize"
        :total="pagination.total"
        :pager-count="5"
        :disabled="loading"
        @current-change="handlePageChange"
      />
    </footer>
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

/*
 * 患者队列位于窄侧栏中，查询条件改成单列排列。
 * max-height: none 用于避免 SearchPanel 的折叠样式裁掉第二行日期字段。
 */
.patient-queue-panel__search :deep(.query-shell__fields) {
  grid-template-columns: minmax(0, 1fr);
}

.patient-queue-panel__search :deep(.query-shell__fields.is-collapsed) {
  max-height: none;
}

/*
 * “关键词”有三个汉字，“日期”只有两个汉字；统一标签占位宽度后，
 * 两行输入控件的左、右边框会落在同一条竖线上。
 */
.patient-queue-panel__search :deep(.query-shell__label) {
  width: 3em;
}

.patient-queue-panel__search :deep(.query-shell__field > .el-input),
.patient-queue-panel__search :deep(.query-shell__field > .el-date-editor),
.patient-queue-panel__search :deep(.query-shell__field > .date-range-field) {
  min-width: 0;
  flex: 1;
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

.patient-queue-panel__search :deep(.el-date-editor),
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

.patient-queue-panel__pagination {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: center;
  min-height: 44px;
  padding: 6px 8px;
  overflow: hidden;
  border-top: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.patient-queue-panel__pagination :deep(.el-pagination) {
  flex-wrap: nowrap;
  justify-content: center;
}
</style>
