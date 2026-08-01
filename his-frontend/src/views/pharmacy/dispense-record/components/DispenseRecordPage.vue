<template>
  <div class="dispense-record-page">
    <SearchPanel
      v-model:keyword="searchForm.patientKeyword"
      keyword-label="患者"
      keyword-placeholder="姓名/门诊号/住院号"
      v-model:dropdown-value="searchForm.recordType"
      dropdown-label="记录类型"
      :dropdown-options="initOptions?.recordTypeOptions || []"
      dropdown-placeholder="全部"
      :loading="tableLoading"
      :field-count="5"
      @search="handleSearch"
      @reset="handleReset"
    >
      <template #header>
        <div class="query-shell__field">
          <label class="query-shell__label">发药日期</label>
          <DateRangeField
            v-model="dateRange"
            :show-options="false"
            :max-range-days="31"
            @change="handleDateChange"
          />
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">药房</label>
          <el-select
            v-model="searchForm.pharmacyId"
            placeholder="全部"
            clearable
            size="default"
            style="width: 100%"
          >
            <el-option
              v-for="opt in initOptions?.pharmacyOptions || []"
              :key="opt.value"
              :label="opt.label"
              :value="opt.value"
            />
          </el-select>
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">药品类别</label>
          <el-select
            v-model="searchForm.category"
            placeholder="全部"
            clearable
            size="default"
            style="width: 100%"
          >
            <el-option
              v-for="opt in initOptions?.categoryOptions || []"
              :key="opt.value"
              :label="opt.label"
              :value="opt.value"
            />
          </el-select>
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">发药人</label>
          <el-select
            v-model="searchForm.dispenserId"
            placeholder="全部"
            clearable
            size="default"
            style="width: 100%"
          >
            <el-option
              v-for="opt in initOptions?.dispenserOptions || []"
              :key="opt.value"
              :label="opt.label"
              :value="opt.value"
            />
          </el-select>
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">药品</label>
          <el-input
            v-model="searchForm.drugKeyword"
            placeholder="药品名称"
            clearable
            size="default"
            @keyup.enter="handleSearch"
          />
        </div>
      </template>
    </SearchPanel>

    <div class="display-mode-bar">
      <span class="display-mode-bar__label">展示方式</span>
      <el-radio-group v-model="displayMode" size="default">
        <el-radio-button
          v-for="mode in DISPLAY_MODES"
          :key="mode.value"
          :value="mode.value"
        >
          {{ mode.label }}
        </el-radio-button>
      </el-radio-group>
      <div class="display-mode-bar__spacer" />
      <el-tooltip content="导出当前筛选结果">
        <el-button
          type="primary"
          :loading="exporting"
          :disabled="!tableData.length"
          @click="handleExport"
        >
          <el-icon><Download /></el-icon>
          导出
        </el-button>
      </el-tooltip>
    </div>

    <DataTablePanel
      v-model:page="pageIndex"
      v-model:page-size="pageSize"
      :data="tableData"
      :total="total"
      :loading="tableLoading"
      show-index
      :key="displayMode"
      :span-method="spanMethod"
      @update:page="handlePageChange"
      @update:pageSize="handlePageSizeChange"
    >
      <el-table-column
        label="处方号"
        width="180"
      />

      <el-table-column
        label="类型"
        width="80"
      >
        <template #default="{ row }">
          <StatusDisplay
            :value="row.recordType"
            :options="RECORD_TYPE_OPTIONS"
            size="small"
          />
        </template>
      </el-table-column>

      <el-table-column
        v-if="isInpatient"
        label="住院号"
        prop="visitNo"
        width="160"
      />
      <el-table-column
        v-else
        label="门诊号"
        prop="visitNo"
        width="160"
      />

      <el-table-column
        label="患者"
        prop="patientName"
        width="100"
      />

      <el-table-column
        v-if="isInpatient"
        label="病区"
        prop="wardName"
        width="100"
      />

      <el-table-column
        v-if="isInpatient"
        label="床号"
        prop="bedNo"
        width="70"
      />

      <el-table-column
        label="开单科室"
        prop="departmentName"
        width="130"
      />

      <el-table-column
        label="发药时间"
        prop="dispenseTime"
        width="160"
      />

      <el-table-column
        label="药房"
        prop="pharmacyName"
        width="120"
      />

      <el-table-column
        label="发药人"
        prop="dispenserName"
        width="100"
      />

      <el-table-column
        label="药品名称"
        prop="drugName"
        min-width="160"
      />

      <el-table-column
        label="规格"
        prop="spec"
        width="120"
      />

      <el-table-column
        label="数量"
        prop="quantity"
        width="70"
        align="right"
      />

      <el-table-column
        label="单位"
        prop="unit"
        width="60"
      />

      <el-table-column
        label="单价"
        prop="price"
        width="80"
        align="right"
      >
        <template #default="{ row }">
          {{ formatMoney(row.price) }}
        </template>
      </el-table-column>

      <el-table-column
        label="金额"
        prop="amount"
        width="90"
        align="right"
      >
        <template #default="{ row }">
          <span class="amount-cell">{{ formatMoney(row.amount) }}</span>
        </template>
      </el-table-column>

      <el-table-column
        label="批号"
        prop="batchNo"
        width="130"
      />

      <el-table-column
        label="追溯码"
        prop="traceCode"
        width="160"
      />

      <el-table-column
        label="用法用量"
        prop="dosage"
        width="130"
      />

      <el-table-column
        label="汇总单号"
        prop="summaryNo"
        width="160"
      />
    </DataTablePanel>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive, computed, onMounted } from 'vue'
import { ElMessage } from 'element-plus'
import { Download } from '@element-plus/icons-vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import type { DateRangeValue } from '@/components/common/type'

import type {
  DispenseRecordScope,
  DisplayMode,
  DispenseRecordItem,
  SearchFormState,
  InitOptions
} from '../type'
import { DISPLAY_MODES, RECORD_TYPE_OPTIONS } from '../constants'
import { adaptOutpatientRecord, adaptInpatientRecord } from '../adapter'

import {
  loadOutpatientDispenseRecordInit,
  pageOutpatientDispenseRecords,
  exportOutpatientDispenseRecords
} from '@/apis/pharmacy/dispense-record/outpatient'
import type {
  PageOutpatientDispenseRecordsParams
} from '@/apis/pharmacy/dispense-record/outpatient/type'
import type { OutpatientDispenseRecordItemDTO } from '@/apis/pharmacy/dispense-record/outpatient/type'

import {
  loadInpatientDispenseRecordInit,
  pageInpatientDispenseRecords,
  exportInpatientDispenseRecords
} from '@/apis/pharmacy/dispense-record/inpatient'
import type {
  PageInpatientDispenseRecordsParams
} from '@/apis/pharmacy/dispense-record/inpatient/type'
import type { InpatientDispenseRecordItemDTO } from '@/apis/pharmacy/dispense-record/inpatient/type'

const props = defineProps<{
  scope: DispenseRecordScope
}>()

const isInpatient = computed(() => props.scope === 'inpatient')

const initOptions = ref<InitOptions | null>(null)

const dateRange = ref<DateRangeValue>(null)

const searchForm = reactive<SearchFormState>({
  startDate: undefined,
  endDate: undefined,
  recordType: undefined,
  pharmacyId: undefined,
  category: undefined,
  dispenserId: undefined,
  drugKeyword: undefined,
  patientKeyword: undefined
})

const displayMode = ref<DisplayMode>('flat')
const pageIndex = ref(1)
const pageSize = ref(20)
const total = ref(0)
const rawRecords = ref<DispenseRecordItem[]>([])
const tableLoading = ref(false)
const exporting = ref(false)
let requestSeed = 0

const tableData = computed(() => {
  const records = [...rawRecords.value]
  if (displayMode.value === 'flat') return records

  if (displayMode.value === 'prescription') {
    records.sort((a, b) => {
      const groupCmp = a.prescriptionNo.localeCompare(b.prescriptionNo)
      if (groupCmp !== 0) return groupCmp
      return a.dispenseTime.localeCompare(b.dispenseTime)
    })
  } else if (displayMode.value === 'encounter') {
    records.sort((a, b) => {
      const groupCmp = a.visitNo.localeCompare(b.visitNo)
      if (groupCmp !== 0) return groupCmp
      return a.dispenseTime.localeCompare(b.dispenseTime)
    })
  }
  return records
})

type SpanInfo = { rowspan: number; colspan: number } | undefined

function getGroupKey(record: DispenseRecordItem): string {
  if (displayMode.value === 'prescription') {
    return record.prescriptionNo + '_' + record.recordType
  }
  if (displayMode.value === 'encounter') {
    return record.visitNo
  }
  return ''
}

function spanMethod({
  rowIndex,
  columnIndex
}: {
  row: DispenseRecordItem
  column: Record<string, unknown>
  rowIndex: number
  columnIndex: number
}): SpanInfo {
  if (displayMode.value === 'flat') return undefined

  const data = tableData.value
  if (rowIndex >= data.length) return undefined
  const currentRow = data[rowIndex]!
  const currentKey = getGroupKey(currentRow)

  if (!currentKey) return undefined

  const groupStart = data.findIndex((r) => getGroupKey(r) === currentKey)
  if (groupStart !== rowIndex) {
    return { rowspan: 0, colspan: 0 }
  }

  let groupSize = 1
  for (let i = rowIndex + 1; i < data.length; i++) {
    if (getGroupKey(data[i]!) === currentKey) groupSize++
    else break
  }

  const groupColSpanEnd = isInpatient.value ? 10 : 8
  if (columnIndex >= 1 && columnIndex <= groupColSpanEnd) {
    return { rowspan: groupSize, colspan: 1 }
  }

  return undefined
}

function formatMoney(value: number | null | undefined): string {
  if (value == null) return '-'
  return value.toFixed(2)
}

function handleDateChange(val: DateRangeValue) {
  if (val) {
    searchForm.startDate = val[0]
    searchForm.endDate = val[1]
  } else {
    searchForm.startDate = undefined
    searchForm.endDate = undefined
  }
  pageIndex.value = 1
  loadData()
}

function handleSearch() {
  pageIndex.value = 1
  loadData()
}

function handleReset() {
  searchForm.startDate = initOptions.value?.defaultDateRange?.[0]
  searchForm.endDate = initOptions.value?.defaultDateRange?.[1]
  searchForm.recordType = undefined
  searchForm.pharmacyId = undefined
  searchForm.category = undefined
  searchForm.dispenserId = undefined
  searchForm.drugKeyword = undefined
  searchForm.patientKeyword = undefined
  dateRange.value = initOptions.value?.defaultDateRange || null
  pageIndex.value = 1
  loadData()
}

function handlePageChange(page: number) {
  pageIndex.value = page
  loadData()
}

function handlePageSizeChange(size: number) {
  pageSize.value = size
  pageIndex.value = 1
  loadData()
}

async function loadInit() {
  try {
    const initResult = isInpatient.value
      ? await loadInpatientDispenseRecordInit()
      : await loadOutpatientDispenseRecordInit()
    initOptions.value = initResult
    dateRange.value = initResult.defaultDateRange
    searchForm.startDate = initResult.defaultDateRange[0]
    searchForm.endDate = initResult.defaultDateRange[1]
  } catch (e) {
    ElMessage.error('加载初始化数据失败')
  }
}

async function loadData() {
  const currentSeed = ++requestSeed
  tableLoading.value = true

  try {
    const baseParams = {
      startDate: searchForm.startDate,
      endDate: searchForm.endDate,
      recordType: searchForm.recordType,
      pharmacyId: searchForm.pharmacyId,
      category: searchForm.category,
      dispenserId: searchForm.dispenserId,
      drugKeyword: searchForm.drugKeyword,
      patientKeyword: searchForm.patientKeyword,
      pageIndex: pageIndex.value,
      pageSize: pageSize.value
    }

    const result = isInpatient.value
      ? await pageInpatientDispenseRecords(
          baseParams as PageInpatientDispenseRecordsParams
        )
      : await pageOutpatientDispenseRecords(
          baseParams as PageOutpatientDispenseRecordsParams
        )

    if (currentSeed === requestSeed) {
      total.value = result.total
      rawRecords.value = isInpatient.value
        ? (result.records as InpatientDispenseRecordItemDTO[]).map(adaptInpatientRecord)
        : (result.records as OutpatientDispenseRecordItemDTO[]).map(adaptOutpatientRecord)
    }
  } catch (e) {
    if (currentSeed === requestSeed) {
      ElMessage.error('加载记录失败')
      rawRecords.value = []
      total.value = 0
    }
  } finally {
    if (currentSeed === requestSeed) {
      tableLoading.value = false
    }
  }
}

async function handleExport() {
  exporting.value = true
  try {
    const baseParams = {
      startDate: searchForm.startDate,
      endDate: searchForm.endDate,
      recordType: searchForm.recordType,
      pharmacyId: searchForm.pharmacyId,
      category: searchForm.category,
      dispenserId: searchForm.dispenserId,
      drugKeyword: searchForm.drugKeyword,
      patientKeyword: searchForm.patientKeyword,
      pageIndex: 1,
      pageSize: 5000
    }

    const result = isInpatient.value
      ? await exportInpatientDispenseRecords(
          baseParams as PageInpatientDispenseRecordsParams
        )
      : await exportOutpatientDispenseRecords(
          baseParams as PageOutpatientDispenseRecordsParams
        )

    if (result.success) {
      ElMessage.success(result.message)
    } else {
      ElMessage.warning(result.message)
    }
  } catch (e) {
    ElMessage.error('导出失败，请重试')
  } finally {
    exporting.value = false
  }
}

onMounted(async () => {
  await loadInit()
  loadData()
})
</script>

<style scoped>
.dispense-record-page {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.display-mode-bar {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 12px 16px;
  background: var(--el-bg-color);
  border-radius: 8px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
}

.display-mode-bar__label {
  font-size: 14px;
  color: var(--el-text-color-regular);
  font-weight: 500;
}

.display-mode-bar__spacer {
  flex: 1;
}

.amount-cell {
  font-weight: 600;
  color: var(--el-color-primary);
}
</style>
