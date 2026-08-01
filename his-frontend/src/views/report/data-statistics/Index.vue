<script setup lang="ts">
import type { EChartsOption } from 'echarts'
import type {
  DiseaseDistributionResponse,
  InventoryAmountComparisonResponse,
  InventoryExpiryWarningDistributionResponse,
  MonthlyVisitCountResponse,
  StatisticsQuery,
  TrendResponse
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
  getMonthlyOutpatientTrend,
  getMonthlyVisitCount,
  getPatientDiseaseDistribution,
  getTrafficTrend,
  getWeeklyOutpatientTrend
} from '@/apis/report/data-statistics'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import StatisticChartCard from './components/StatisticChartCard.vue'
import { departmentOptions, granularityOptions } from './constants'

defineOptions({ name: 'ReportDataStatistics' })

type StatisticKey = 'traffic' | 'monthlyVisit' | 'weeklyOutpatient' | 'monthlyOutpatient' | 'disease' | 'inventoryAmount' | 'inventoryExpiry'

const today = dayjs().format('YYYY-MM-DD')
const query = reactive<DataStatisticsQuery>({ dateRange: [today, today], granularity: 'DAY', organizationId: '' })
const loading = ref(false)
const errorMessage = ref('')
const requestSequence = ref(0)
let controller: AbortController | undefined
const retryControllers = new Map<StatisticKey, AbortController>()

const traffic = ref<TrendResponse>()
const monthlyVisit = ref<MonthlyVisitCountResponse>()
const weeklyOutpatient = ref<TrendResponse>()
const monthlyOutpatient = ref<TrendResponse>()
const disease = ref<DiseaseDistributionResponse>()
const inventoryAmount = ref<InventoryAmountComparisonResponse>()
const inventoryExpiry = ref<InventoryExpiryWarningDistributionResponse>()
const chartErrors = reactive<Record<string, string>>({})
const chartLoading = reactive<Record<string, boolean>>({})

const actions = computed<ActionToolbarItem[]>(() => [{ key: 'refresh', label: '刷新', icon: Refresh, loading: loading.value }])
const requestQuery = computed<StatisticsQuery>(() => ({ dateFrom: query.dateRange[0], dateTo: query.dateRange[1], granularity: query.granularity, organizationId: query.organizationId || undefined }))

function lineOption(result?: TrendResponse): EChartsOption | undefined {
  if (!result?.points.length) return undefined
  return { tooltip: { trigger: 'axis' }, xAxis: { type: 'category', data: result.points.map((item) => item.label) }, yAxis: { type: 'value', name: result.unit }, series: [{ type: 'line', smooth: true, data: result.points.map((item) => item.value), areaStyle: {} }] }
}
function barOption(result?: TrendResponse): EChartsOption | undefined {
  if (!result?.points.length) return undefined
  return { tooltip: { trigger: 'axis' }, xAxis: { type: 'category', data: result.points.map((item) => item.label) }, yAxis: { type: 'value', name: result.unit }, series: [{ type: 'bar', data: result.points.map((item) => item.value) }] }
}
function diseaseOption(result?: DiseaseDistributionResponse): EChartsOption | undefined {
  if (!result?.items.length) return undefined
  return { tooltip: { trigger: 'item' }, legend: { bottom: 0 }, series: [{ type: 'pie', radius: ['35%', '68%'], data: result.items.map((item) => ({ name: item.name, value: item.value })) }] }
}
function inventoryOption(result?: InventoryAmountComparisonResponse): EChartsOption | undefined {
  if (!result?.categories.length) return undefined
  return { tooltip: { trigger: 'axis' }, legend: { data: result.series.map((item) => item.name) }, xAxis: { type: 'category', data: result.categories }, yAxis: { type: 'value', name: result.unit }, series: result.series.map((item) => ({ name: item.name, type: 'bar', stack: item.direction === 'INBOUND' ? '入库' : '出库', data: item.values })) }
}
function expiryOption(result?: InventoryExpiryWarningDistributionResponse): EChartsOption | undefined {
  if (!result?.items.length) return undefined
  return { tooltip: { trigger: 'axis' }, xAxis: { type: 'category', data: result.items.map((item) => item.name) }, yAxis: { type: 'value', name: result.unit }, series: [{ type: 'bar', data: result.items.map((item) => item.value) }] }
}

function setChartError(key: string, error: unknown) {
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
  const keys: StatisticKey[] = ['traffic', 'monthlyVisit', 'weeklyOutpatient', 'monthlyOutpatient', 'disease', 'inventoryAmount', 'inventoryExpiry']
  keys.forEach((key) => {
    delete chartErrors[key]
    chartLoading[key] = true
  })
  const context = { signal: currentController.signal }
  const q = requestQuery.value
  async function settle<T>(key: string, request: Promise<T>, assign: (value: T) => void) {
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
  await Promise.all([
    settle('traffic', getTrafficTrend(q, context), (value) => (traffic.value = value)),
    settle('monthlyVisit', getMonthlyVisitCount(q, context), (value) => (monthlyVisit.value = value)),
    settle('weeklyOutpatient', getWeeklyOutpatientTrend(q, context), (value) => (weeklyOutpatient.value = value)),
    settle('monthlyOutpatient', getMonthlyOutpatientTrend(q, context), (value) => (monthlyOutpatient.value = value)),
    settle('disease', getPatientDiseaseDistribution(q, context), (value) => (disease.value = value)),
    settle('inventoryAmount', getInventoryAmountComparison(q, context), (value) => (inventoryAmount.value = value)),
    settle('inventoryExpiry', getInventoryExpiryWarningDistribution(q, context), (value) => (inventoryExpiry.value = value))
  ])
  if (sequence === requestSequence.value) loading.value = false
}

function handleFilterChange() {
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
    if (key === 'traffic') {
      const value = await getTrafficTrend(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      traffic.value = value
    } else if (key === 'monthlyVisit') {
      const value = await getMonthlyVisitCount(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      monthlyVisit.value = value
    } else if (key === 'weeklyOutpatient') {
      const value = await getWeeklyOutpatientTrend(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      weeklyOutpatient.value = value
    } else if (key === 'monthlyOutpatient') {
      const value = await getMonthlyOutpatientTrend(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      monthlyOutpatient.value = value
    } else if (key === 'disease') {
      const value = await getPatientDiseaseDistribution(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      disease.value = value
    } else if (key === 'inventoryAmount') {
      const value = await getInventoryAmountComparison(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      inventoryAmount.value = value
    } else {
      const value = await getInventoryExpiryWarningDistribution(q, { signal: retryController.signal })
      if (sequence !== requestSequence.value || retryController.signal.aborted) return
      inventoryExpiry.value = value
    }
    if (!Object.keys(chartErrors).length) errorMessage.value = ''
  } catch (error) {
    if (retryController.signal.aborted || sequence !== requestSequence.value) return
    setChartError(key, error)
    errorMessage.value = '部分统计区域加载失败，可单独重试'
  } finally {
    if (sequence === requestSequence.value && !retryController.signal.aborted) chartLoading[key] = false
    if (retryControllers.get(key) === retryController) retryControllers.delete(key)
  }
}

function showFilterSummary(title: string, item?: { name: string; value: unknown }) {
  const department = departmentOptions.find((option) => option.value === query.organizationId)?.label ?? '全院'
  const granularity = granularityOptions.find((option) => option.value === query.granularity)?.label ?? query.granularity
  const itemText = item?.name ? `；数据项：${item.name}（${String(item.value ?? '—')}）` : ''
  ElMessage.info(`${title}；统计日期：${query.dateRange[0]} 至 ${query.dateRange[1]}；统计粒度：${granularity}；科室：${department}${itemText}`)
}

function handleToolbar() { void loadStatistics() }
onMounted(() => void loadStatistics())
onBeforeUnmount(() => {
  controller?.abort()
  retryControllers.forEach((item) => item.abort())
})
</script>

<template>
  <main class="statistics-page">
    <SearchPanel class="statistics-search" keyword-label="关键词" :keyword="''" dropdown-label="科室" dropdown-placeholder="请选择科室" :dropdown-options="departmentOptions" :dropdown-value="query.organizationId" :loading="loading" :field-count="2" @update:dropdown-value="query.organizationId = String($event ?? '')" @search="handleFilterChange" @reset="() => { query.dateRange = [today, today]; query.granularity = 'DAY'; query.organizationId = ''; handleFilterChange() }">
      <template #header>
        <div class="statistics-filter"><span>统计日期</span><DateRangeField v-model="query.dateRange" :show-options="false" :max-range-days="366" @change="handleFilterChange" /></div>
        <div class="statistics-filter"><span>统计粒度</span><el-select v-model="query.granularity" @change="handleFilterChange"><el-option v-for="item in granularityOptions" :key="item.value" v-bind="item" /></el-select></div>
      </template>
    </SearchPanel>
    <ActionToolbar title="数据统计" description="统一刷新七个统计区域" :actions="actions" :disabled="loading" @action="handleToolbar" />
    <el-alert v-if="errorMessage" type="warning" :title="errorMessage" :closable="false" show-icon />
    <section class="statistics-grid">
      <StatisticChartCard title="访问流量趋势" :option="lineOption(traffic)" :loading="chartLoading.traffic" :error="chartErrors.traffic" :empty="traffic?.points.length === 0" @retry="retryChart('traffic')" @select="showFilterSummary('访问流量趋势', $event)" />
      <StatisticChartCard title="月访问量" :option="barOption(monthlyVisit)" :loading="chartLoading.monthlyVisit" :error="chartErrors.monthlyVisit" :empty="monthlyVisit?.points.length === 0" @retry="retryChart('monthlyVisit')" @select="showFilterSummary('月访问量', $event)" />
      <StatisticChartCard title="周接诊量门诊趋势" :option="lineOption(weeklyOutpatient)" :loading="chartLoading.weeklyOutpatient" :error="chartErrors.weeklyOutpatient" :empty="weeklyOutpatient?.points.length === 0" @retry="retryChart('weeklyOutpatient')" @select="showFilterSummary('周接诊量门诊趋势', $event)" />
      <StatisticChartCard title="月接诊量门诊趋势" :option="barOption(monthlyOutpatient)" :loading="chartLoading.monthlyOutpatient" :error="chartErrors.monthlyOutpatient" :empty="monthlyOutpatient?.points.length === 0" @retry="retryChart('monthlyOutpatient')" @select="showFilterSummary('月接诊量门诊趋势', $event)" />
      <StatisticChartCard title="患者疾病类型分布" :option="diseaseOption(disease)" :loading="chartLoading.disease" :error="chartErrors.disease" :empty="disease?.items.length === 0" @retry="retryChart('disease')" @select="showFilterSummary('患者疾病类型分布', $event)" />
      <StatisticChartCard title="物资入库与出库金额对比" :option="inventoryOption(inventoryAmount)" :loading="chartLoading.inventoryAmount" :error="chartErrors.inventoryAmount" :empty="inventoryAmount?.categories.length === 0" @retry="retryChart('inventoryAmount')" @select="showFilterSummary('物资入库与出库金额对比', $event)" />
      <StatisticChartCard title="库存物资有效期预警分布" :option="expiryOption(inventoryExpiry)" :loading="chartLoading.inventoryExpiry" :error="chartErrors.inventoryExpiry" :empty="inventoryExpiry?.items.length === 0" @retry="retryChart('inventoryExpiry')" @select="showFilterSummary('库存物资有效期预警分布', $event)" />
    </section>
  </main>
</template>

<style scoped>
.statistics-page { display: flex; min-height: calc(100vh - 96px); flex-direction: column; gap: 12px; padding: 16px; color: #1f2937; }
.statistics-filter { display: flex; min-width: 0; align-items: center; gap: 8px; }
.statistics-filter > span { flex-shrink: 0; color: #374151; font-size: 13px; }
.statistics-filter :deep(.el-select) { min-width: 180px; }
.statistics-search :deep(.query-shell__fields > .query-shell__field:first-child) { display: none; }
.statistics-grid { display: grid; grid-template-columns: repeat(6, minmax(0, 1fr)); gap: 14px; }
.statistics-grid > :nth-child(-n + 4) { grid-column: span 3; }
.statistics-grid > :nth-child(n + 5) { grid-column: span 2; }
@media (max-width: 960px) { .statistics-grid { grid-template-columns: 1fr; } .statistics-grid > :nth-child(n) { grid-column: auto; } }
</style>
