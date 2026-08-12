<script setup lang="ts">
import type { EChartsOption } from 'echarts'
import type {
  DiseaseDistributionResponse,
  InventoryAmountComparisonResponse,
  InventoryExpiryWarningDistributionResponse,
  StatisticsQuery,
  StatisticsProvider
} from '@/apis/report/data-statistics'
import type { ActionToolbarItem } from '@/components/common/type'
import type { DataStatisticsQuery } from './type'

import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage } from 'element-plus'
import { Refresh } from '@element-plus/icons-vue'
import dayjs from 'dayjs'

import {
  getInventoryAmountComparison,
  getInventoryExpiryWarningDistribution,
  getPatientDiseaseDistribution,
  getStatisticsProvider,
  setStatisticsProvider
} from '@/apis/report/data-statistics'
import { resolveScheduleOrganizationId } from '@/apis/booking/schedule-template/context'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import StatisticChartCard from './components/StatisticChartCard.vue'
import { allDepartmentOption, granularityOptions } from './constants'

defineOptions({ name: 'ReportDataStatistics' })

type StatisticKey = 'disease' | 'inventoryAmount' | 'inventoryExpiry'

const today = dayjs().format('YYYY-MM-DD')
const currentMonthStart = dayjs().startOf('month').format('YYYY-MM-DD')
const query = reactive<DataStatisticsQuery>({
  dateRange: [currentMonthStart, today],
  granularity: 'DAY',
  organizationId: ''
})
const loading = ref(false)
const errorMessage = ref('')
const requestSequence = ref(0)
const storedProvider =
  typeof localStorage === 'undefined' ? '' : localStorage.getItem('reportStatisticsProvider')
const sourceProvider = ref<StatisticsProvider>(
  storedProvider === 'j6' || storedProvider === 'c1' || storedProvider === 'j8'
    ? storedProvider
    : getStatisticsProvider()
)
setStatisticsProvider(sourceProvider.value)
const sourceOptions = [
  { label: 'J8组', value: 'j8' },
  { label: 'J6组', value: 'j6' },
  { label: 'C1组', value: 'c1' }
]
const configuredC1OrganizationId = String(
  import.meta.env.VITE_C1_DEFAULT_ORGANIZATION_ID ?? ''
).trim()
const c1OrganizationId = ref(configuredC1OrganizationId)
let controller: AbortController | undefined
const retryControllers = new Map<StatisticKey, AbortController>()

const disease = ref<DiseaseDistributionResponse>()
const inventoryAmount = ref<InventoryAmountComparisonResponse>()
const inventoryExpiry = ref<InventoryExpiryWarningDistributionResponse>()
const chartErrors = reactive<Record<string, string>>({})
const chartLoading = reactive<Record<string, boolean>>({})

const departmentOptions = computed(() => {
  const rawContext =
    typeof localStorage === 'undefined' ? null : localStorage.getItem('loginContext')
  const organizationId = resolveScheduleOrganizationId(
    rawContext,
    import.meta.env.VITE_J8_DEFAULT_ORGANIZATION_ID
  )
  if (!organizationId) return [allDepartmentOption]
  let label = '当前科室'
  try {
    const context = JSON.parse(rawContext ?? '{}') as Record<string, unknown>
    label = String(
      context.organizationName ??
        context.departmentName ??
        context.orgName ??
        context.deptName ??
        label
    )
  } catch {
    // 登录上下文损坏时仍使用已配置的真实科室 ID。
  }
  return [allDepartmentOption, { label, value: organizationId }]
})

const actions = computed<ActionToolbarItem[]>(() => [
  { key: 'refresh', label: '刷新', icon: Refresh, loading: loading.value }
])
const c1EmptyText = computed(() => ({
  disease: '后端未返回该日期范围内具有完整诊断关联的数据',
  inventoryExpiry: '后端未返回库存有效期预警数据'
}))
const requestQuery = computed<StatisticsQuery>(() => ({
  dateFrom: query.dateRange[0],
  dateTo: query.dateRange[1],
  granularity: query.granularity,
  organizationId:
    sourceProvider.value === 'c1'
      ? c1OrganizationId.value || undefined
      : query.organizationId || undefined
}))

function diseaseOption(result?: DiseaseDistributionResponse): EChartsOption | undefined {
  if (!result?.items.length) return undefined
  return {
    tooltip: { trigger: 'item' },
    legend: { bottom: 0 },
    series: [
      {
        type: 'pie',
        radius: ['35%', '68%'],
        data: result.items.map((item) => ({ name: item.name, value: item.value }))
      }
    ]
  }
}
function inventoryOption(result?: InventoryAmountComparisonResponse): EChartsOption | undefined {
  if (!result?.categories.length) return undefined
  const series = result.series.map((item) => ({
    ...item,
    displayName: `${item.name}${item.direction === 'INBOUND' ? '入库' : '出库'}`
  }))
  return {
    tooltip: { trigger: 'axis' },
    legend: { data: series.map((item) => item.displayName) },
    xAxis: { type: 'category', data: result.categories },
    yAxis: { type: 'value', name: result.unit },
    series: series.map((item) => ({
      name: item.displayName,
      type: 'bar',
      stack: item.direction === 'INBOUND' ? '入库' : '出库',
      data: item.values
    }))
  }
}
function expiryOption(
  result?: InventoryExpiryWarningDistributionResponse
): EChartsOption | undefined {
  if (!result?.items.length) return undefined
  return {
    tooltip: { trigger: 'axis' },
    xAxis: { type: 'category', data: result.items.map((item) => item.name) },
    yAxis: { type: 'value', name: result.unit },
    series: [{ type: 'bar', data: result.items.map((item) => item.value) }]
  }
}

function setChartError(key: StatisticKey, error: unknown) {
  chartErrors[key] = error instanceof Error ? error.message : '接口加载失败，请重试'
}

async function loadStatistics() {
  controller?.abort()
  retryControllers.forEach((item) => item.abort())
  retryControllers.clear()
  const currentController = new AbortController()
  controller = currentController
  const sequence = ++requestSequence.value
  loading.value = true
  errorMessage.value = ''
  const keys: StatisticKey[] = ['disease', 'inventoryAmount', 'inventoryExpiry']
  keys.forEach((key) => {
    delete chartErrors[key]
    chartLoading[key] = true
  })
  const context = { signal: currentController.signal }
  const q = requestQuery.value
  async function settle<T>(key: StatisticKey, request: Promise<T>, assign: (value: T) => void) {
    try {
      const value = await request
      if (sequence !== requestSequence.value || currentController.signal.aborted) return
      assign(value)
    } catch (error) {
      if (currentController.signal.aborted || sequence !== requestSequence.value) return
      setChartError(key, error)
      errorMessage.value = '部分统计区域加载失败，可单独重试'
    } finally {
      if (sequence === requestSequence.value) chartLoading[key] = false
    }
  }
  const requests: Promise<void>[] = [
    settle(
      'disease',
      getPatientDiseaseDistribution(q, context),
      (value) => (disease.value = value)
    ),
    settle(
      'inventoryAmount',
      getInventoryAmountComparison(q, context),
      (value) => (inventoryAmount.value = value)
    ),
    settle(
      'inventoryExpiry',
      getInventoryExpiryWarningDistribution(q, context),
      (value) => (inventoryExpiry.value = value)
    )
  ]
  await Promise.all(requests)
  if (sequence === requestSequence.value) loading.value = false
}

function handleFilterChange() {
  void loadStatistics()
}

function clearStatistics() {
  disease.value = undefined
  inventoryAmount.value = undefined
  inventoryExpiry.value = undefined
}

function changeSourceProvider(value: string | number | boolean | undefined) {
  if (value !== 'j8' && value !== 'j6' && value !== 'c1') return
  controller?.abort()
  retryControllers.forEach((item) => item.abort())
  retryControllers.clear()
  sourceProvider.value = value
  setStatisticsProvider(value)
  if (typeof localStorage !== 'undefined') localStorage.setItem('reportStatisticsProvider', value)
  clearStatistics()
  void loadStatistics()
}

async function retryChart(key: StatisticKey) {
  retryControllers.get(key)?.abort()
  const retryController = new AbortController()
  retryControllers.set(key, retryController)
  const sequence = requestSequence.value
  const q = { ...requestQuery.value }
  chartLoading[key] = true
  delete chartErrors[key]
  try {
    if (key === 'disease') {
      const value = await getPatientDiseaseDistribution(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      disease.value = value
    } else if (key === 'inventoryAmount') {
      const value = await getInventoryAmountComparison(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      inventoryAmount.value = value
    } else {
      const value = await getInventoryExpiryWarningDistribution(q, {
        signal: retryController.signal
      })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      inventoryExpiry.value = value
    }
    if (!Object.keys(chartErrors).length) errorMessage.value = ''
  } catch (error) {
    if (retryController.signal.aborted || sequence !== requestSequence.value) return
    setChartError(key, error)
    errorMessage.value = '部分统计区域加载失败，可单独重试'
  } finally {
    if (sequence === requestSequence.value && !retryController.signal.aborted)
      chartLoading[key] = false
    if (retryControllers.get(key) === retryController) retryControllers.delete(key)
  }
}

function showFilterSummary(title: string, item?: { name: string; value: unknown }) {
  const department =
    departmentOptions.value.find((option) => option.value === query.organizationId)?.label ?? '全院'
  const granularity =
    granularityOptions.find((option) => option.value === query.granularity)?.label ??
    query.granularity
  const itemText = item?.name ? `；数据项：${item.name}（${String(item.value ?? '—')}）` : ''
  ElMessage.info(
    `${title}；统计日期：${query.dateRange[0]} 至 ${query.dateRange[1]}；统计粒度：${granularity}；科室：${department}${itemText}`
  )
}

function handleToolbar() {
  void loadStatistics()
}
onMounted(() => void loadStatistics())
onBeforeUnmount(() => {
  controller?.abort()
  retryControllers.forEach((item) => item.abort())
})
</script>

<template>
  <main class="statistics-page">
    <SearchPanel
      class="statistics-search"
      keyword-label="关键词"
      :keyword="''"
      dropdown-label="科室"
      dropdown-placeholder="请选择科室"
      :dropdown-options="departmentOptions"
      :dropdown-value="query.organizationId"
      :loading="loading"
      :field-count="2"
      @update:dropdown-value="query.organizationId = String($event ?? '')"
      @search="handleFilterChange"
      @reset="
        () => {
          query.dateRange = [currentMonthStart, today]
          query.granularity = 'DAY'
          query.organizationId = ''
          handleFilterChange()
        }
      "
    >
      <template #header>
        <div class="statistics-filter">
          <span>统计日期</span
          ><DateRangeField
            v-model="query.dateRange"
            :show-options="false"
            :max-range-days="366"
            @change="handleFilterChange"
          />
        </div>
        <div class="statistics-filter">
          <span>统计粒度</span
          ><el-select v-model="query.granularity" @change="handleFilterChange"
            ><el-option v-for="item in granularityOptions" :key="item.value" v-bind="item"
          /></el-select>
        </div>
        <div class="statistics-filter statistics-source-filter">
          <span>接口小组</span>
          <el-segmented
            :model-value="sourceProvider"
            :options="sourceOptions"
            :disabled="loading"
            @change="changeSourceProvider"
          />
        </div>
      </template>
    </SearchPanel>
    <ActionToolbar
      title="数据统计"
      description="统一刷新三个统计区域"
      :actions="actions"
      :disabled="loading"
      @action="handleToolbar"
    />
    <el-alert
      v-if="errorMessage"
      type="warning"
      :title="errorMessage"
      :closable="false"
      show-icon
    />
    <section class="statistics-grid">
      <StatisticChartCard
        title="患者疾病类型分布"
        :option="diseaseOption(disease)"
        :loading="chartLoading.disease"
        :error="chartErrors.disease"
        :empty="disease?.items.length === 0"
        :empty-text="sourceProvider === 'c1' ? c1EmptyText.disease : '暂无数据'"
        @retry="retryChart('disease')"
        @select="showFilterSummary('患者疾病类型分布', $event)"
      />
      <StatisticChartCard
        title="物资入库与出库金额对比"
        :option="inventoryOption(inventoryAmount)"
        :loading="chartLoading.inventoryAmount"
        :error="chartErrors.inventoryAmount"
        :empty="inventoryAmount?.categories.length === 0"
        @retry="retryChart('inventoryAmount')"
        @select="showFilterSummary('物资入库与出库金额对比', $event)"
      />
      <StatisticChartCard
        title="库存物资有效期预警分布"
        :option="expiryOption(inventoryExpiry)"
        :loading="chartLoading.inventoryExpiry"
        :error="chartErrors.inventoryExpiry"
        :empty="inventoryExpiry?.items.length === 0"
        :empty-text="sourceProvider === 'c1' ? c1EmptyText.inventoryExpiry : '暂无数据'"
        @retry="retryChart('inventoryExpiry')"
        @select="showFilterSummary('库存物资有效期预警分布', $event)"
      />
    </section>
  </main>
</template>

<style scoped>
.statistics-page {
  display: flex;
  min-height: calc(100vh - 96px);
  flex-direction: column;
  gap: 12px;
  padding: 16px;
  color: #1f2937;
}
.statistics-filter {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 8px;
}
.statistics-filter > span {
  flex-shrink: 0;
  color: #374151;
  font-size: 13px;
}
.statistics-filter :deep(.el-select) {
  min-width: 180px;
}
.statistics-source-filter :deep(.el-segmented) {
  min-width: 180px;
}
.statistics-search :deep(.query-shell__fields > .query-shell__field:first-child) {
  display: none;
}
.statistics-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 14px;
}
@media (max-width: 960px) {
  .statistics-grid {
    grid-template-columns: 1fr;
  }
}
</style>
