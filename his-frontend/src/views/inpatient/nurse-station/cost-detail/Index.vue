<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import dayjs from 'dayjs'
import { ElMessage, type TagProps } from 'element-plus'

import {
  queryNurseStationCostDetail,
  queryNurseStationCostSummary,
  type NurseStationCostDetailLineVO,
  type NurseStationCostPresentationMode,
  type NurseStationCostSummaryLineVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'

defineOptions({ name: 'NurseStationCostDetail' })

const props = defineProps<{
  patients: NurseStationPatientSummary[]
}>()

type QuickDay = 'today' | 'yesterday'

const selectedPatient = computed(() => (props.patients.length === 1 ? props.patients[0] : null))
const loading = ref(false)
const quickDay = ref<QuickDay>('today')
const today = dayjs().format('YYYY-MM-DD')
const dateRange = ref<string[]>([today, today])
const presentationMode = ref<NurseStationCostPresentationMode>('DETAIL')
const detailLines = ref<NurseStationCostDetailLineVO[]>([])
const summaryLines = ref<NurseStationCostSummaryLineVO[]>([])
const totalDetailCount = ref(0)
const pageIndex = ref(1)
const pageSize = ref(20)
let requestSerial = 0

const activeItemCount = computed(() =>
  presentationMode.value === 'DETAIL' ? totalDetailCount.value : summaryLines.value.length
)

function formatMoney(value: number | undefined) {
  return `￥${Number(value ?? 0).toFixed(2)}`
}

function formatUnitPrice(value: number | undefined) {
  return value == null ? '—' : Number(value).toFixed(4)
}

function formatDetailQuantity(row: NurseStationCostDetailLineVO) {
  return `${row.quantityValue} ${row.quantityUnit || ''}`.trim()
}

function formatDetailQuantityFromTable(row: unknown) {
  return formatDetailQuantity(row as NurseStationCostDetailLineVO)
}

function formatStatus(statusEnum: string) {
  const statusText: Record<string, string> = {
    CHARGED: '已收费',
    PENDING: '待收费',
    PENDING_SETTLEMENT: '待结算',
    REFUNDED: '已退费',
    SETTLED: '已结算'
  }
  return statusText[statusEnum] ?? statusEnum ?? '—'
}

function resolveStatusTagType(statusEnum: string): TagProps['type'] {
  if (statusEnum.includes('REFUND')) return 'danger'
  if (statusEnum.includes('PENDING')) return 'warning'
  if (statusEnum.includes('SETTLED') || statusEnum.includes('CHARGED')) return 'success'
  return 'info'
}

function resolveSummaryRowKey(row: NurseStationCostSummaryLineVO) {
  return `${row.typeCode}-${row.summaryDate}`
}

async function queryCostDetail() {
  const patient = selectedPatient.value
  if (!patient) return
  const [startDate, endDate] = dateRange.value
  if (!startDate || !endDate || dayjs(startDate).isAfter(dayjs(endDate))) {
    ElMessage.warning('请选择有效的查询日期')
    return
  }

  const serial = ++requestSerial
  const encounterId = patient.encounterId
  loading.value = true
  try {
    if (presentationMode.value === 'DETAIL') {
      const result = await queryNurseStationCostDetail({
        pageIndex: pageIndex.value,
        pageSize: pageSize.value,
        encounterId,
        patientId: patient.patientId,
        startDate,
        endDate
      })
      if (serial !== requestSerial || selectedPatient.value?.encounterId !== encounterId) return
      detailLines.value = result.rows ?? []
      totalDetailCount.value = result.total
    } else {
      const result = await queryNurseStationCostSummary({ encounterId, startDate, endDate })
      if (serial !== requestSerial || selectedPatient.value?.encounterId !== encounterId) return
      summaryLines.value = result
    }
    if (serial !== requestSerial || selectedPatient.value?.encounterId !== encounterId) return
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '查询费用明细失败')
  } finally {
    if (serial === requestSerial) loading.value = false
  }
}

watch(
  selectedPatient,
  async () => {
    requestSerial += 1
    detailLines.value = []
    summaryLines.value = []
    totalDetailCount.value = 0
    pageIndex.value = 1
    if (!selectedPatient.value) return
    await queryCostDetail()
  },
  { immediate: true }
)

function changeQuickDay(day: QuickDay) {
  quickDay.value = day
  const target = day === 'today' ? dayjs() : dayjs().subtract(1, 'day')
  const date = target.format('YYYY-MM-DD')
  dateRange.value = [date, date]
  pageIndex.value = 1
  void queryCostDetail()
}

function resetFilters() {
  changeQuickDay('today')
}

function changePresentationMode(mode: NurseStationCostPresentationMode) {
  if (presentationMode.value === mode) return
  presentationMode.value = mode
  pageIndex.value = 1
  void queryCostDetail()
}

function handlePageChange(page: number) {
  pageIndex.value = page
  void queryCostDetail()
}
</script>

<template>
  <section class="cost-detail-module">
    <el-empty v-if="!selectedPatient" description="请先从左侧选择一位患者" />

    <div v-else class="cost-detail-workspace">
      <header class="cost-patient-header">
        <div class="cost-patient-identity">
          <strong>{{ selectedPatient.patientName }}</strong>
          <span
            >{{ selectedPatient.genderText || '—' }} / {{ selectedPatient.ageText || '—' }}</span
          >
          <el-tag size="small" effect="plain">{{ selectedPatient.statusText || '在院' }}</el-tag>
        </div>
        <dl>
          <div>
            <dt>床位</dt>
            <dd>{{ selectedPatient.wardName }} {{ selectedPatient.bedName }}</dd>
          </div>
          <div>
            <dt>住院号</dt>
            <dd>{{ selectedPatient.encounterBusNo || '—' }}</dd>
          </div>
          <div>
            <dt>诊断</dt>
            <dd>{{ selectedPatient.diagnosisSummary || '—' }}</dd>
          </div>
          <div class="cost-balance">
            <dt>可用余额</dt>
            <dd>{{ formatMoney(selectedPatient.availableBalance) }}</dd>
          </div>
        </dl>
      </header>

      <section class="cost-filter-card">
        <div class="filter-left">
          <el-radio-group
            :model-value="quickDay"
            @change="(value: string | number | boolean | undefined) => changeQuickDay(value as QuickDay)"
          >
            <el-radio-button value="today">今日</el-radio-button>
            <el-radio-button value="yesterday">昨日</el-radio-button>
          </el-radio-group>
          <el-date-picker
            v-model="dateRange"
            type="daterange"
            value-format="YYYY-MM-DD"
            format="YYYY-MM-DD"
            range-separator="至"
            start-placeholder="开始日期"
            end-placeholder="结束日期"
          />
        </div>
        <div>
          <el-button @click="resetFilters">重置</el-button>
          <el-button type="primary" :loading="loading" @click="queryCostDetail">查询</el-button>
        </div>
      </section>

      <section class="cost-data-card">
        <header class="cost-data-header">
          <nav class="presentation-tabs" aria-label="费用展示模式">
            <button
              v-for="option in [
                { code: 'DETAIL' as const, display: '明细' },
                { code: 'SUMMARY' as const, display: '汇总' }
              ]"
              :key="option.code"
              :class="{ 'is-active': presentationMode === option.code }"
              @click="changePresentationMode(option.code)"
            >
              {{ option.display }}
            </button>
          </nav>
          <div class="cost-item-count">
            {{ presentationMode === 'DETAIL' ? '明细' : '汇总' }}项数：{{ activeItemCount }} 项
          </div>
        </header>

        <div v-loading="loading" class="cost-table-wrap">
          <el-table
            v-if="presentationMode === 'DETAIL'"
            :data="detailLines"
            border
            stripe
            height="100%"
            row-key="id"
            data-testid="cost-detail-table"
          >
            <el-table-column
              prop="chargeName"
              label="项目名称"
              min-width="180"
              show-overflow-tooltip
            />
            <el-table-column label="数量" width="92" align="center">
              <template #default="{ row }">{{ formatDetailQuantityFromTable(row) }}</template>
            </el-table-column>
            <el-table-column label="单价" width="100" align="right">
              <template #default="{ row }">{{ formatUnitPrice(row.unitPrice) }}</template>
            </el-table-column>
            <el-table-column label="金额" width="108" align="right">
              <template #default="{ row }">
                <strong>{{ formatMoney(row.totalPrice) }}</strong>
              </template>
            </el-table-column>
            <el-table-column prop="typeCode" label="费用分类" width="110" />
            <el-table-column prop="occurrenceTime" label="业务时间" min-width="156" />
            <el-table-column label="费用状态" width="110" align="center">
              <template #default="{ row }">
                <el-tag size="small" :type="resolveStatusTagType(row.statusEnum)">{{
                  formatStatus(row.statusEnum)
                }}</el-tag>
              </template>
            </el-table-column>
          </el-table>

          <el-table
            v-else
            :data="summaryLines"
            border
            stripe
            height="100%"
            :row-key="resolveSummaryRowKey"
            data-testid="cost-summary-table"
          >
            <el-table-column prop="summaryDate" label="汇总日期" min-width="140" />
            <el-table-column prop="typeCode" label="费用类型编码" min-width="140" />
            <el-table-column label="费用类型" min-width="160">
              <template #default="{ row }">{{ row.typeName || row.typeCode }}</template>
            </el-table-column>
            <el-table-column prop="count" label="笔数" width="100" align="center" />
            <el-table-column label="总金额" width="140" align="right">
              <template #default="{ row }"
                ><strong>{{ formatMoney(row.totalAmount) }}</strong></template
              >
            </el-table-column>
          </el-table>

          <el-empty
            v-if="!loading && activeItemCount === 0"
            class="cost-empty"
            description="当前查询条件下暂无费用数据"
            :image-size="86"
          />
        </div>

        <footer v-if="presentationMode === 'DETAIL' && totalDetailCount" class="cost-pagination">
          <el-pagination
            :current-page="pageIndex"
            :page-size="pageSize"
            :total="totalDetailCount"
            layout="total, prev, pager, next"
            @current-change="handlePageChange"
          />
        </footer>
      </section>
    </div>
  </section>
</template>

<style scoped>
.cost-detail-module {
  display: flex;
  height: 100%;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  background: #f2f4f7;
  color: #303133;
  -webkit-text-fill-color: currentColor;
}

.cost-detail-workspace {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
  flex-direction: column;
  gap: 8px;
  padding: 0 8px 8px;
}

.cost-patient-header {
  padding: 12px 14px 10px;
  border: 1px solid #e4e7ed;
  border-top: 0;
  border-radius: 0 0 6px 6px;
  background: #fff;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.cost-patient-identity {
  display: flex;
  align-items: center;
  gap: 10px;
}
.cost-patient-identity strong {
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 18px;
}
.cost-patient-identity span {
  color: #606266;
  -webkit-text-fill-color: #606266;
}
.cost-patient-header dl {
  display: grid;
  grid-template-columns: repeat(5, minmax(130px, 1fr));
  gap: 12px;
  margin: 10px 0 0;
}
.cost-patient-header dl > div {
  display: flex;
  min-width: 0;
  gap: 6px;
}
.cost-patient-header dt {
  flex: 0 0 auto;
  color: #6b7280;
  -webkit-text-fill-color: #6b7280;
}
.cost-patient-header dd {
  margin: 0;
  overflow: hidden;
  color: #303133;
  -webkit-text-fill-color: #303133;
  text-overflow: ellipsis;
  white-space: nowrap;
}
.cost-balance dd {
  color: #16a34a;
  -webkit-text-fill-color: #16a34a;
  font-weight: 700;
}

.cost-filter-card {
  display: flex;
  min-height: 54px;
  flex: 0 0 auto;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding: 8px 12px;
  border: 1px solid #e4e7ed;
  border-radius: 6px;
  background: #fff;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.filter-left {
  display: flex;
  align-items: center;
  gap: 8px;
}
.filter-left :deep(.el-date-editor) {
  width: 280px;
}

.cost-data-card {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
  flex-direction: column;
  overflow: hidden;
  border: 1px solid #e4e7ed;
  border-radius: 6px;
  background: #fff;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.cost-data-header {
  display: flex;
  min-height: 58px;
  flex: 0 0 auto;
  align-items: center;
  gap: 12px;
  padding: 0 12px;
  overflow-x: auto;
  border-bottom: 1px solid #e4e7ed;
}

.presentation-tabs {
  display: flex;
  height: 58px;
  flex: 0 0 auto;
  align-items: stretch;
}
.presentation-tabs button {
  position: relative;
  min-width: 74px;
  padding: 0 14px;
  cursor: pointer;
  border: 0;
  background: transparent;
  color: #606266;
  -webkit-text-fill-color: #606266;
  font: inherit;
}
.presentation-tabs button.is-active {
  color: #409eff;
  -webkit-text-fill-color: #409eff;
  font-weight: 600;
}
.presentation-tabs button.is-active::after {
  position: absolute;
  right: 12px;
  bottom: 0;
  left: 12px;
  height: 2px;
  background: #409eff;
  content: '';
}

.cost-item-count {
  margin-left: auto;
  flex: 0 0 auto;
  color: #606266;
  -webkit-text-fill-color: #606266;
  white-space: nowrap;
}

.cost-table-wrap {
  position: relative;
  min-width: 0;
  min-height: 0;
  flex: 1;
  overflow: hidden;
}
.cost-table-wrap :deep(.el-table),
.cost-table-wrap :deep(.el-table th.el-table__cell),
.cost-table-wrap :deep(.el-table td.el-table__cell) {
  color: #303133;
  -webkit-text-fill-color: #303133;
}
.cost-empty {
  position: absolute;
  background: #fff;
  inset: 0;
}
.cost-pagination {
  display: flex;
  min-height: 46px;
  flex: 0 0 auto;
  align-items: center;
  justify-content: flex-end;
  padding: 0 12px;
  border-top: 1px solid #e4e7ed;
}
.cost-detail-module > :deep(.el-empty) {
  min-height: 0;
  flex: 1;
}

@media (max-width: 1366px) {
  .cost-patient-header {
    padding: 9px 12px 8px;
  }
  .cost-patient-header dl {
    grid-template-columns: repeat(3, minmax(130px, 1fr));
    gap: 6px 12px;
  }
  .cost-data-header {
    min-height: 52px;
  }
  .presentation-tabs {
    height: 52px;
  }
}
</style>
