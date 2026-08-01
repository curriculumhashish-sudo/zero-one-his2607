<script setup lang="ts">
import type { ActionToolbarItem } from '@/components/common/type'
import type { FeeDetailQuery, FeeDetailTableRow } from './type'

import { computed, nextTick, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage } from 'element-plus'
import { Download, Printer, Refresh } from '@element-plus/icons-vue'
import dayjs from 'dayjs'

import {
  exportOutpatientFeeDetails,
  pageOutpatientFeeDetails
} from '@/apis/report/outpatient-fee-detail'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import { displayAmount, flattenEncounterBlocks, isSubtotal } from './adapter'
import { feeDetailOptions, summaryFields, tradeDirectionOptions } from './constants'

defineOptions({ name: 'ReportOutpatientFeeDetail' })

const today = dayjs().format('YYYY-MM-DD')
const query = reactive<FeeDetailQuery>({
  pageNo: 1,
  pageSize: 10,
  dateRange: [today, today],
  tradeDirection: 'ALL'
})
const rows = ref<FeeDetailTableRow[]>([])
const total = ref(0)
const summary = ref<Record<string, number | string> | undefined>()
const loading = ref(false)
const exporting = ref(false)
const printing = ref(false)
const printRows = ref<FeeDetailTableRow[]>([])
const printSummary = ref<Record<string, number | string> | undefined>()
const errorMessage = ref('')
const initialized = ref(false)
let controller: AbortController | undefined
let printController: AbortController | undefined
let requestSequence = 0

const actions = computed<ActionToolbarItem[]>(() => [
  { key: 'export', label: '导出', icon: Download, disabled: total.value === 0, loading: exporting.value },
  { key: 'print', label: '打印 / PDF', icon: Printer, disabled: total.value === 0, loading: printing.value },
  { key: 'refresh', label: '刷新', icon: Refresh, loading: loading.value }
])
const summaryText = computed(() =>
  summaryFields
    .map(([label, key]) => `${label} ${displayAmount(summary.value?.[key])}`)
    .join('　')
)

function buildPayload() {
  const [dateFrom, dateTo] = query.dateRange
  if (!dateFrom || !dateTo) {
    ElMessage.warning('请选择收费日期范围')
    return null
  }
  if (dateFrom > dateTo) {
    ElMessage.warning('开始日期不能晚于结束日期')
    return null
  }
  return {
    PageIndex: query.pageNo,
    PageSize: query.pageSize,
    dateFrom,
    dateTo,
    registrationDepartmentId: query.registrationDepartmentId,
    feeCategoryCodes: query.feeCategoryCodes?.length ? query.feeCategoryCodes : undefined,
    subCategoryCodes: query.subCategoryCodes?.length ? query.subCategoryCodes : undefined,
    patientKeyword: query.patientKeyword?.trim() || undefined,
    productNameKeyword: query.productNameKeyword?.trim() || undefined,
    productCodeKeyword: query.productCodeKeyword?.trim() || undefined,
    cashierId: query.cashierId,
    tradeDirection: query.tradeDirection
  }
}

async function loadRows() {
  const payload = buildPayload()
  if (!payload) return
  controller?.abort()
  const currentController = new AbortController()
  controller = currentController
  const sequence = ++requestSequence
  loading.value = true
  errorMessage.value = ''
  try {
    const result = await pageOutpatientFeeDetails(payload, { signal: currentController.signal })
    if (sequence !== requestSequence || currentController.signal.aborted) return
    rows.value = flattenEncounterBlocks(result.content ?? [])
    total.value = result.totalCount ?? 0
    summary.value = result.summary as Record<string, number | string> | undefined
    initialized.value = true
  } catch (error) {
    if (currentController.signal.aborted) return
    rows.value = []
    total.value = 0
    summary.value = undefined
    errorMessage.value = error instanceof Error ? error.message : '门诊费用明细加载失败，请重试'
  } finally {
    if (sequence === requestSequence) loading.value = false
  }
}

function resetQuery() {
  Object.assign(query, {
    pageNo: 1,
    pageSize: 10,
    dateRange: [today, today],
    tradeDirection: 'ALL',
    registrationDepartmentId: undefined,
    feeCategoryCodes: undefined,
    subCategoryCodes: undefined,
    patientKeyword: undefined,
    productNameKeyword: undefined,
    productCodeKeyword: undefined,
    cashierId: undefined
  })
  void loadRows()
}

function handleSearch() {
  query.pageNo = 1
  void loadRows()
}

function handlePaginationChange(payload: { page: number; pageSize: number }) {
  query.pageNo = payload.pageSize !== query.pageSize ? 1 : payload.page
  query.pageSize = payload.pageSize
  void loadRows()
}

async function handleExport() {
  const payload = buildPayload()
  if (!payload || !total.value || exporting.value) return
  exporting.value = true
  try {
    const filters = {
      dateFrom: payload.dateFrom,
      dateTo: payload.dateTo,
      registrationDepartmentId: payload.registrationDepartmentId,
      feeCategoryCodes: payload.feeCategoryCodes,
      subCategoryCodes: payload.subCategoryCodes,
      patientKeyword: payload.patientKeyword,
      productNameKeyword: payload.productNameKeyword,
      productCodeKeyword: payload.productCodeKeyword,
      cashierId: payload.cashierId,
      tradeDirection: payload.tradeDirection
    }
    const result = await exportOutpatientFeeDetails(filters)
    if (!result.body) throw new Error('导出文件为空')
    const binary = atob(result.body)
    const bytes = Uint8Array.from(binary, (char) => char.charCodeAt(0))
    const url = URL.createObjectURL(new Blob([bytes], { type: result.contentType }))
    const link = document.createElement('a')
    link.href = url
    link.download = result.fileName ?? '门诊费用明细.xlsx'
    link.click()
    URL.revokeObjectURL(url)
    ElMessage.success(`导出成功，共 ${result.rowCount ?? result.exportedRowCount ?? 0} 行`)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '导出失败，请重试')
  } finally {
    exporting.value = false
  }
}

async function handlePrint() {
  const payload = buildPayload()
  if (!payload || !total.value || printing.value) return
  printController?.abort()
  const currentController = new AbortController()
  printController = currentController
  printing.value = true
  try {
    const result = await pageOutpatientFeeDetails(
      { ...payload, PageIndex: 1, PageSize: total.value },
      { signal: currentController.signal }
    )
    if (currentController.signal.aborted) return
    printRows.value = flattenEncounterBlocks(result.content ?? [])
    printSummary.value = result.summary as Record<string, number | string> | undefined
    if (!printRows.value.length) throw new Error('当前条件下没有可打印的数据')
    await nextTick()
    window.print()
  } catch (error) {
    if (currentController.signal.aborted) return
    ElMessage.error(error instanceof Error ? error.message : '打印数据加载失败，请重试')
  } finally {
    if (printController === currentController) {
      printing.value = false
      printController = undefined
    }
  }
}

function handleToolbar(action: ActionToolbarItem) {
  if (action.key === 'export') void handleExport()
  else if (action.key === 'print') void handlePrint()
  else void loadRows()
}

onMounted(() => void loadRows())
onBeforeUnmount(() => {
  controller?.abort()
  printController?.abort()
})
</script>

<template>
  <main class="fee-detail-page">
    <SearchPanel
      keyword-label="患者 / 门诊号"
      keyword-placeholder="请输入患者姓名或门诊号"
      :keyword="query.patientKeyword ?? ''"
      :loading="loading"
      :field-count="6"
      @update:keyword="query.patientKeyword = $event"
      @search="handleSearch"
      @reset="resetQuery"
    >
      <template #header>
        <div class="query-field"><span>收费日期</span><DateRangeField v-model="query.dateRange" :show-options="false" :max-range-days="31" @change="handleSearch" /></div>
        <div class="query-field"><span>收退方向</span><el-select v-model="query.tradeDirection" @change="handleSearch"><el-option v-for="item in tradeDirectionOptions" :key="item.value" v-bind="item" /></el-select></div>
        <div class="query-field"><span>挂号科室</span><el-select v-model="query.registrationDepartmentId" clearable @change="handleSearch"><el-option v-for="item in feeDetailOptions.departments" :key="item.value" v-bind="item" /></el-select></div>
        <div class="query-field"><span>费用大类</span><el-select v-model="query.feeCategoryCodes" multiple collapse-tags clearable @change="handleSearch"><el-option v-for="item in feeDetailOptions.feeCategories" :key="item.value" v-bind="item" /></el-select></div>
        <div class="query-field"><span>细分类</span><el-select v-model="query.subCategoryCodes" multiple collapse-tags clearable @change="handleSearch"><el-option v-for="item in feeDetailOptions.subCategories" :key="item.value" v-bind="item" /></el-select></div>
        <div class="query-field"><span>项目名称</span><el-input v-model="query.productNameKeyword" clearable @keyup.enter="handleSearch" /></div>
        <div class="query-field"><span>院内编码</span><el-input v-model="query.productCodeKeyword" clearable @keyup.enter="handleSearch" /></div>
        <div class="query-field"><span>收费员</span><el-select v-model="query.cashierId" clearable @change="handleSearch"><el-option v-for="item in feeDetailOptions.cashiers" :key="item.value" v-bind="item" /></el-select></div>
      </template>
    </SearchPanel>

    <ActionToolbar title="门诊费用明细" :description="initialized ? `全量合计：${summaryText}` : ''" :actions="actions" :disabled="loading || exporting || printing" @action="handleToolbar" />
    <el-alert v-if="errorMessage" type="error" :title="errorMessage" :closable="false" show-icon><el-button type="primary" link @click="loadRows">重试</el-button></el-alert>
    <EmptyState v-if="initialized && !loading && !rows.length" title="暂无门诊费用明细" description="当前条件下没有可展示的收费记录" />
    <DataTablePanel v-else :data="rows" :loading="loading" :total="0" min-height="420px" border stripe>
      <el-table-column prop="busNo" label="门诊号" width="130" />
      <el-table-column prop="patientName" label="患者" width="100" />
      <el-table-column prop="registrationDepartmentName" label="挂号科室" width="120" />
      <el-table-column prop="encounterDate" label="就诊日期" width="110" />
      <el-table-column prop="feeCategoryDisplay" label="大类" width="80" />
      <el-table-column prop="subCategoryDisplay" label="细分类" width="100" />
      <el-table-column prop="productName" label="项目名称" min-width="150" />
      <el-table-column prop="productCode" label="院内编码" width="110" />
      <el-table-column prop="specification" label="规格" width="100" />
      <el-table-column prop="quantityValue" label="数量" width="80" />
      <el-table-column prop="quantityUnitDisplay" label="单位" width="70" />
      <el-table-column prop="orderingDoctorName" label="开单医生" width="100" />
      <el-table-column prop="cashierName" label="收费人" width="90" />
      <el-table-column prop="billDate" label="结算时间" width="170" />
      <el-table-column prop="tradeDirection" label="来源" width="80" />
      <el-table-column prop="insuranceLevelDisplay" label="医保等级" width="90" />
      <el-table-column prop="insuranceTypeDisplay" label="医保类型" width="90" />
      <el-table-column prop="baseAmount" label="原价" width="90" />
      <el-table-column prop="discountAmount" label="优惠金额" width="100" />
      <el-table-column prop="receivableAmount" label="应收金额" width="100" />
      <el-table-column prop="discountedAmount" label="折后金额" width="100" />
      <el-table-column prop="unitPrice" label="单价" width="90" />
      <el-table-column prop="lineAmount" label="行金额" width="100" />
      <el-table-column prop="actualReceivedAmount" label="实收金额" width="100" />
      <el-table-column prop="insuranceAmount" label="医保金额" width="100" />
      <el-table-column prop="selfPayAmount" label="自费金额" width="100" />
      <el-table-column label="行类型" width="90"><template #default="{ row }">{{ isSubtotal(row) ? '就诊小计' : '明细' }}</template></el-table-column>
    </DataTablePanel>
    <PaginationBar v-if="total > 0" :page="query.pageNo" :page-size="query.pageSize" :total="total" :disabled="loading" @change="handlePaginationChange" />

    <section class="print-sheet" aria-hidden="true">
      <h1>门诊费用明细</h1>
      <p>收费日期：{{ query.dateRange[0] }} 至 {{ query.dateRange[1] }}</p>
      <p>{{ summaryFields.map(([label, key]) => `${label} ${displayAmount(printSummary?.[key])}`).join('　') }}</p>
      <table>
        <thead><tr><th>门诊号</th><th>患者</th><th>挂号科室</th><th>就诊日期</th><th>费用分类</th><th>项目名称</th><th>院内编码</th><th>数量</th><th>单价</th><th>行金额</th><th>实收</th><th>医保</th><th>自费</th><th>结算时间</th></tr></thead>
        <tbody>
          <tr v-for="row in printRows" :key="row.rowKey" :class="{ subtotal: isSubtotal(row) }">
            <td>{{ row.busNo }}</td><td>{{ row.patientName }}</td><td>{{ row.registrationDepartmentName }}</td><td>{{ row.encounterDate }}</td><td>{{ row.feeCategoryDisplay }}{{ row.subCategoryDisplay ? ` / ${row.subCategoryDisplay}` : '' }}</td><td>{{ isSubtotal(row) ? '就诊小计' : row.productName }}</td><td>{{ row.productCode }}</td><td>{{ row.quantityValue }} {{ row.quantityUnitDisplay }}</td><td>{{ displayAmount(row.unitPrice) }}</td><td>{{ displayAmount(row.lineAmount) }}</td><td>{{ displayAmount(row.actualReceivedAmount) }}</td><td>{{ displayAmount(row.insuranceAmount) }}</td><td>{{ displayAmount(row.selfPayAmount) }}</td><td>{{ row.billDate }}</td>
          </tr>
        </tbody>
      </table>
    </section>
  </main>
</template>

<style scoped>
.fee-detail-page { display: flex; min-height: calc(100vh - 96px); flex-direction: column; gap: 12px; padding: 16px; color: #1f2937; }
.query-field { display: flex; min-width: 0; align-items: center; gap: 8px; }
.query-field > span { flex-shrink: 0; color: #374151; font-size: 13px; }
.query-field :deep(.el-select), .query-field :deep(.el-input) { min-width: 0; flex: 1; }
.fee-detail-page > :deep(.pagination-bar) { justify-content: flex-end; }
.print-sheet { display: none; }
@media print {
  @page { size: landscape; margin: 8mm; }
  .fee-detail-page { display: block; min-height: 0; padding: 0; color: #000; }
  .fee-detail-page > :not(.print-sheet) { display: none !important; }
  .print-sheet { display: block; }
  .print-sheet h1 { margin: 0 0 8px; text-align: center; font-size: 18px; }
  .print-sheet p { margin: 4px 0; font-size: 10px; }
  .print-sheet table { width: 100%; border-collapse: collapse; table-layout: fixed; font-size: 8px; }
  .print-sheet th, .print-sheet td { overflow-wrap: anywhere; padding: 3px; border: 1px solid #666; }
  .print-sheet th { background: #eee; }
  .print-sheet tr.subtotal { background: #f2f2f2; font-weight: 700; }
}
</style>
