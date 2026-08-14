<script setup lang="ts">
import dayjs from 'dayjs'
import { ElMessage, ElMessageBox } from 'element-plus'
import { computed, ref, watch } from 'vue'

import {
  cancelNurseStationDispenseSummaries,
  createNurseStationDispenseSummary,
  getNurseStationDispenseSummaryDetail,
  queryNurseStationDispensePending,
  queryNurseStationDispenseSummaries,
  submitNurseStationDispenseSummaries
} from '@/apis/inpatient/nurse-station'
import type {
  NurseStationDispenseMedicineCategory,
  NurseStationDispenseOrderRowVO,
  NurseStationDispensePatientGroupVO,
  NurseStationDispenseSummaryHeaderVO,
  NurseStationDispenseSummaryLineVO,
  NurseStationDispenseTherapyFilter,
  NurseStationDispenseTimePointVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'
import DispenseSummaryDetailDialog from './components/DispenseSummaryDetailDialog.vue'

defineOptions({ name: 'InpatientNurseStationDispenseSummary' })

const props = withDefaults(defineProps<{ patients?: NurseStationPatientSummary[] }>(), {
  patients: () => []
})

type DisplayMode = 'DETAIL' | 'SUMMARY'

interface SelectableTimePoint {
  encounterId: string
  slotKey: string
  point: NurseStationDispenseTimePointVO
}

const displayMode = ref<DisplayMode>('DETAIL')
const medicineCategory = ref<NurseStationDispenseMedicineCategory>('WEST')
const therapyFilter = ref<NurseStationDispenseTherapyFilter>('ALL')
const queryDeadline = ref(dayjs().endOf('day').format('YYYY-MM-DD HH:mm:ss'))
const loading = ref(false)
const actionLoading = ref(false)
const pendingGroups = ref<NurseStationDispensePatientGroupVO[]>([])
const pendingTotal = ref(0)
const selectedSlotKeys = ref(new Set<string>())
const summaryRows = ref<NurseStationDispenseSummaryHeaderVO[]>([])
const summaryTotal = ref(0)
const summaryPageIndex = ref(1)
const selectedSummaryRows = ref<NurseStationDispenseSummaryHeaderVO[]>([])
const detailVisible = ref(false)
const detailLoading = ref(false)
const detailHeader = ref<NurseStationDispenseSummaryHeaderVO | null>(null)
const detailLines = ref<NurseStationDispenseSummaryLineVO[]>([])
let requestSerial = 0

const encounterIds = computed(() => props.patients.map((patient) => patient.encounterId))
const encounterIdsKey = computed(() => encounterIds.value.join('|'))

/**
 * 查询结果只包含有待汇总时点的患者。这里重新按照左侧选择顺序排列，护士切换筛选条件后，
 * 患者卡片顺序仍与左侧保持一致，不会因为后端分组顺序变化而跳动。
 */
const visiblePatientGroups = computed(() => {
  const groupMap = new Map(pendingGroups.value.map((group) => [group.encounterId, group]))
  return props.patients.map((patient) => {
    const group = groupMap.get(patient.encounterId)
    return (
      group ?? {
        encounterId: patient.encounterId,
        patientId: patient.patientId,
        patientName: patient.patientName,
        bedDisplay: `${patient.houseName ?? ''} ${patient.bedName ?? ''}`.trim() || '—',
        orderRows: []
      }
    )
  })
})

/** 把所有医嘱时点展开，供“全选”和创建汇总单共同使用。 */
const allSelectablePoints = computed(() => {
  const slots: SelectableTimePoint[] = []
  for (const group of pendingGroups.value) {
    for (const order of group.orderRows) {
      for (const point of order.timePoints) {
        slots.push({
          encounterId: group.encounterId,
          point,
          slotKey: buildSlotKey(group.encounterId, point.dispenseId)
        })
      }
    }
  }
  return slots
})

const selectedAll = computed(
  () =>
    allSelectablePoints.value.length > 0 &&
    selectedSlotKeys.value.size === allSelectablePoints.value.length
)

const pendingSummarySelection = computed(() =>
  selectedSummaryRows.value.filter((row) => row.statusEnum === 'SUMMARIZED')
)

function buildSlotKey(encounterId: string, dispenseId: string) {
  return `${encounterId}|${dispenseId}`
}

function formatQuantity(point: NurseStationDispenseTimePointVO) {
  return `${point.quantity}${point.unitCodeText}`
}

function formatSlotLabel(point: NurseStationDispenseTimePointVO) {
  return `${dayjs(point.plannedDispenseTime).format('HH:mm')} ${formatQuantity(point)}`
}

/** 同一天的发药时点放在同一行，复刻原型中的“日期 + 多个时点”布局。 */
function groupTimePoints(points: NurseStationDispenseTimePointVO[]) {
  const groups = new Map<string, NurseStationDispenseTimePointVO[]>()
  for (const point of points) {
    const date = dayjs(point.plannedDispenseTime).format('MM-DD')
    const rows = groups.get(date) ?? []
    rows.push(point)
    groups.set(date, rows)
  }
  return [...groups.entries()].map(([date, rows]) => ({
    date,
    points: rows.sort((left, right) =>
      left.plannedDispenseTime.localeCompare(right.plannedDispenseTime)
    )
  }))
}

function patientSummary(encounterId: string) {
  return props.patients.find((patient) => patient.encounterId === encounterId)
}

function isSlotSelected(encounterId: string, dispenseId: string) {
  return selectedSlotKeys.value.has(buildSlotKey(encounterId, dispenseId))
}

/** Set 必须创建新实例，Vue 才能稳定通知依赖它的全选状态和模板重新计算。 */
function toggleSlot(encounterId: string, dispenseId: string, checked: boolean) {
  const next = new Set(selectedSlotKeys.value)
  const key = buildSlotKey(encounterId, dispenseId)
  if (checked) next.add(key)
  else next.delete(key)
  selectedSlotKeys.value = next
}

function toggleAll(checked: boolean) {
  selectedSlotKeys.value = checked
    ? new Set(allSelectablePoints.value.map((item) => item.slotKey))
    : new Set()
}

/** Element Plus 把开关事件声明成联合类型，页面在边界处统一转换成真正的布尔值。 */
function handleToggleAll(value: string | number | boolean) {
  toggleAll(Boolean(value))
}

/** 复选框事件同样先做布尔转换，再交给严格类型的业务函数。 */
function handleSlotChange(
  encounterId: string,
  dispenseId: string,
  checked: string | number | boolean
) {
  toggleSlot(encounterId, dispenseId, Boolean(checked))
}

async function loadPending() {
  const currentEncounterIds = encounterIds.value
  selectedSlotKeys.value = new Set()
  pendingGroups.value = []
  pendingTotal.value = 0
  if (!currentEncounterIds.length) return
  if (!dayjs(queryDeadline.value).isValid()) {
    ElMessage.warning('请选择有效的截止时间')
    return
  }

  const serial = ++requestSerial
  loading.value = true
  try {
    const result = await queryNurseStationDispensePending({
      pageIndex: 1,
      pageSize: 500,
      encounterIds: currentEncounterIds,
      medicineCategory: medicineCategory.value,
      therapyFilter: therapyFilter.value,
      queryDeadline: queryDeadline.value
    })
    if (serial !== requestSerial || encounterIdsKey.value !== currentEncounterIds.join('|')) return
    pendingGroups.value = result.patientGroups
    pendingTotal.value = result.total
    // 原型默认勾选当前查询到的全部时点，护士可以再取消不需要领取的项目。
    selectedSlotKeys.value = new Set(
      result.patientGroups.flatMap((group) =>
        group.orderRows.flatMap((order) =>
          order.timePoints.map((point) => buildSlotKey(group.encounterId, point.dispenseId))
        )
      )
    )
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '查询待汇总明细失败')
  } finally {
    if (serial === requestSerial) loading.value = false
  }
}

async function loadSummaries() {
  const serial = ++requestSerial
  selectedSummaryRows.value = []
  loading.value = true
  try {
    const result = await queryNurseStationDispenseSummaries({
      pageIndex: summaryPageIndex.value,
      pageSize: 20
    })
    if (serial !== requestSerial) return
    summaryRows.value = result.rows ?? []
    summaryTotal.value = result.total
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '查询汇总单失败')
  } finally {
    if (serial === requestSerial) loading.value = false
  }
}

async function runCurrentQuery() {
  if (displayMode.value === 'DETAIL') await loadPending()
  else await loadSummaries()
}

/**
 * 选择患者、药品分类或长临条件后自动刷新。请求序号能阻止慢请求覆盖用户刚切换的新条件。
 */
watch(
  [encounterIdsKey, medicineCategory, therapyFilter, displayMode],
  () => {
    summaryPageIndex.value = 1
    void runCurrentQuery()
  },
  { immediate: true }
)

async function createSummary() {
  if (actionLoading.value) return
  const selected = allSelectablePoints.value.filter((item) =>
    selectedSlotKeys.value.has(item.slotKey)
  )
  if (!selected.length) {
    ElMessage.warning('请先勾选待发药时间点')
    return
  }
  try {
    await ElMessageBox.confirm(`确认汇总已选 ${selected.length} 条待发药明细？`, '汇总领药', {
      type: 'warning',
      confirmButtonText: '确定',
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  actionLoading.value = true
  try {
    const result = await createNurseStationDispenseSummary({
      dispenseItems: selected.map((item) => ({
        encounterId: item.encounterId,
        dispenseId: item.point.dispenseId
      }))
    })
    ElMessage.success(result.message)
    await loadPending()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '汇总领药失败')
  } finally {
    actionLoading.value = false
  }
}

function handleSummarySelection(rows: NurseStationDispenseSummaryHeaderVO[]) {
  selectedSummaryRows.value = rows
}

function isPendingSummary(row: NurseStationDispenseSummaryHeaderVO) {
  return row.statusEnum === 'SUMMARIZED'
}

function statusTagType(status: NurseStationDispenseSummaryHeaderVO['statusEnum']) {
  if (status === 'DISPENSED') return 'success'
  if (status === 'SUMMARIZED') return 'warning'
  return 'info'
}

function summaryStatusText(status: NurseStationDispenseSummaryHeaderVO['statusEnum']) {
  if (status === 'SUMMARIZED') return '待发送'
  if (status === 'WAIT_REVIEW') return '已发送'
  return '已发药'
}

async function viewDetail(row: NurseStationDispenseSummaryHeaderVO) {
  detailVisible.value = true
  detailLoading.value = true
  detailHeader.value = row
  detailLines.value = []
  try {
    detailLines.value = await getNurseStationDispenseSummaryDetail(row.summaryNo)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '读取汇总单详情失败')
  } finally {
    detailLoading.value = false
  }
}

async function submitRows(rows: NurseStationDispenseSummaryHeaderVO[]) {
  if (actionLoading.value) return
  if (!rows.length) {
    ElMessage.warning('请勾选待发送状态的汇总单')
    return
  }
  try {
    await ElMessageBox.confirm(`确认发送 ${rows.length} 张汇总单至药房？`, '发送汇总单', {
      type: 'warning'
    })
  } catch {
    return
  }
  actionLoading.value = true
  try {
    const result = await submitNurseStationDispenseSummaries({
      summaryNos: rows.map((row) => row.summaryNo)
    })
    ElMessage.success(result.message || `已发送 ${result.summaryNos.length} 张汇总单`)
    await loadSummaries()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '发送汇总单失败，请刷新后重试')
  } finally {
    actionLoading.value = false
  }
}

async function cancelRows() {
  if (actionLoading.value) return
  const rows = pendingSummarySelection.value
  if (!rows.length) {
    ElMessage.warning('请勾选待发送状态的汇总单')
    return
  }
  try {
    await ElMessageBox.confirm(
      `确认撤销 ${rows.length} 张汇总单？对应发药时点会恢复为待汇总。`,
      '撤销汇总单',
      { type: 'warning' }
    )
  } catch {
    return
  }
  actionLoading.value = true
  try {
    const result = await cancelNurseStationDispenseSummaries({
      summaryNos: rows.map((row) => row.summaryNo)
    })
    ElMessage.success(result.message || `已撤销 ${result.summaryNos.length} 张汇总单`)
    await loadSummaries()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '撤销汇总单失败，请刷新后重试')
  } finally {
    actionLoading.value = false
  }
}

function changeSummaryPage(page: number) {
  summaryPageIndex.value = page
  void loadSummaries()
}

/** 模板表格插槽会把行推断为 unknown，单独适配后能保持业务函数拥有严格类型。 */
function orderExtension(row: unknown) {
  const order = row as NurseStationDispenseOrderRowVO
  return `${order.dosageInstruction}；${order.methodText}，${order.frequencyText}`
}

/** 以下三个函数只负责把 Element Plus 的通用表格行收窄为本页汇总单类型。 */
function isPendingSummaryFromTable(row: unknown) {
  return isPendingSummary(row as NurseStationDispenseSummaryHeaderVO)
}

function viewDetailFromTable(row: unknown) {
  return viewDetail(row as NurseStationDispenseSummaryHeaderVO)
}

function submitSingleFromTable(row: unknown) {
  return submitRows([row as NurseStationDispenseSummaryHeaderVO])
}
</script>

<template>
  <section class="dispense-summary-module">
    <el-empty v-if="patients.length === 0" description="请先从左侧选择患者" />

    <div v-else class="dispense-workspace">
      <header class="dispense-toolbar">
        <div class="toolbar-filters">
          <el-radio-group v-model="medicineCategory">
            <el-radio-button value="WEST">西药</el-radio-button>
            <el-radio-button value="TCM">中药</el-radio-button>
          </el-radio-group>
          <el-radio-group v-model="displayMode">
            <el-radio-button value="DETAIL">医嘱</el-radio-button>
            <el-radio-button value="SUMMARY">汇总单</el-radio-button>
          </el-radio-group>
          <template v-if="displayMode === 'DETAIL'">
            <el-radio-group v-model="therapyFilter">
              <el-radio value="ALL">全部</el-radio>
              <el-radio value="LONG">长期</el-radio>
              <el-radio value="TEMP">临时</el-radio>
            </el-radio-group>
            <span class="deadline-label">截止时间</span>
            <el-date-picker
              v-model="queryDeadline"
              class="deadline-picker"
              type="datetime"
              value-format="YYYY-MM-DD HH:mm:ss"
              format="YYYY-MM-DD HH:mm:ss"
              placeholder="请选择截止时间"
            />
          </template>
          <el-button :loading="loading" @click="runCurrentQuery">查询</el-button>
        </div>

        <div class="toolbar-actions">
          <span class="record-count">
            {{ displayMode === 'DETAIL' ? `${pendingTotal}个时点` : `${summaryTotal}张单据` }}
          </span>
          <template v-if="displayMode === 'DETAIL'">
            <label class="select-all-label">
              全选
              <el-switch
                :model-value="selectedAll"
                :disabled="allSelectablePoints.length === 0"
                @change="handleToggleAll"
              />
            </label>
            <el-button
              type="primary"
              :disabled="selectedSlotKeys.size === 0 || actionLoading"
              @click="createSummary"
            >
              汇总领药
            </el-button>
          </template>
          <template v-else>
            <el-button
              type="primary"
              :disabled="pendingSummarySelection.length === 0 || actionLoading"
              @click="submitRows(pendingSummarySelection)"
            >
              发送
            </el-button>
            <el-button
              type="danger"
              plain
              :disabled="pendingSummarySelection.length === 0 || actionLoading"
              @click="cancelRows"
            >
              撤销
            </el-button>
          </template>
        </div>
      </header>

      <div v-loading="loading || actionLoading" class="dispense-content">
        <div v-if="displayMode === 'DETAIL'" class="dispense-patient-list">
          <section
            v-for="group in visiblePatientGroups"
            :key="group.encounterId"
            class="dispense-patient-card"
          >
            <header class="dispense-patient-card__header">
              <div>
                <strong>{{ group.patientName }}</strong>
                <span>
                  {{ patientSummary(group.encounterId)?.genderText || '—' }} /
                  {{ patientSummary(group.encounterId)?.ageText || '—' }}
                </span>
                <el-tag size="small" effect="plain">
                  {{ patientSummary(group.encounterId)?.statusText || '在院' }}
                </el-tag>
              </div>
              <div class="patient-meta">
                <span>{{ group.bedDisplay }}</span>
                <span>住院号：{{ patientSummary(group.encounterId)?.encounterBusNo || '—' }}</span>
                <span>诊断：{{ patientSummary(group.encounterId)?.diagnosisSummary || '—' }}</span>
              </div>
            </header>

            <el-table
              v-if="group.orderRows.length"
              :data="group.orderRows"
              stripe
              border
              row-key="medicationRequestId"
              class="dispense-order-table"
            >
              <el-table-column label="类型" width="100" align="center">
                <template #default="{ row }">{{ row.therapyTypeText }}/药品</template>
              </el-table-column>
              <el-table-column label="医嘱内容" min-width="280">
                <template #default="{ row }">
                  <div class="order-content">
                    <strong>{{ row.medicationName }}【{{ row.specificationText }}】</strong>
                    <span>{{ orderExtension(row) }}</span>
                  </div>
                </template>
              </el-table-column>
              <el-table-column prop="pharmacyName" label="取药药房" min-width="120" />
              <el-table-column label="领药时间" min-width="260">
                <template #default="{ row }">
                  <div class="dispense-schedule-list">
                    <div
                      v-for="dayGroup in groupTimePoints(row.timePoints)"
                      :key="row.medicationRequestId + '-' + dayGroup.date"
                      class="dispense-schedule-row"
                    >
                      <span class="schedule-date">{{ dayGroup.date }}</span>
                      <div class="schedule-slots">
                        <el-checkbox
                          v-for="point in dayGroup.points"
                          :key="point.dispenseId"
                          class="dispense-slot"
                          border
                          size="small"
                          :value="point.dispenseId"
                          :model-value="isSlotSelected(group.encounterId, point.dispenseId)"
                          @change="
                            (checked: string | number | boolean) =>
                              handleSlotChange(group.encounterId, point.dispenseId, checked)
                          "
                        >
                          {{ formatSlotLabel(point) }}
                        </el-checkbox>
                      </div>
                    </div>
                  </div>
                </template>
              </el-table-column>
            </el-table>
            <el-empty v-else description="当前筛选下暂无待汇总明细" :image-size="70" />
          </section>
        </div>

        <div v-else class="summary-table-card">
          <el-table
            :data="summaryRows"
            stripe
            border
            row-key="summaryNo"
            height="100%"
            data-testid="dispense-summary-table"
            @selection-change="handleSummarySelection"
          >
            <el-table-column type="selection" width="48" :selectable="isPendingSummary" />
            <el-table-column
              prop="summaryNo"
              label="单据号"
              min-width="170"
              show-overflow-tooltip
            />
            <el-table-column
              prop="desLocationId"
              label="目标位置"
              min-width="120"
              show-overflow-tooltip
            />
            <el-table-column label="状态" width="100" align="center">
              <template #default="{ row }">
                <el-tag size="small" :type="statusTagType(row.statusEnum)">
                  {{ summaryStatusText(row.statusEnum) }}
                </el-tag>
              </template>
            </el-table-column>
            <el-table-column prop="itemCount" label="发药明细数" width="100" align="center" />
            <el-table-column prop="plannedDispenseTime" label="计划发药时间" min-width="150" />
            <el-table-column label="发药时间" min-width="150">
              <template #default="{ row }">{{ row.dispenseTime || '—' }}</template>
            </el-table-column>
            <el-table-column label="操作" width="140" fixed="right" align="center">
              <template #default="{ row }">
                <el-button link type="primary" @click="viewDetailFromTable(row)">详情</el-button>
                <el-button
                  link
                  type="primary"
                  :disabled="!isPendingSummaryFromTable(row) || actionLoading"
                  @click="submitSingleFromTable(row)"
                >
                  发送
                </el-button>
              </template>
            </el-table-column>
          </el-table>
          <footer v-if="summaryTotal" class="summary-pagination">
            <el-pagination
              :current-page="summaryPageIndex"
              :page-size="20"
              :total="summaryTotal"
              layout="total, prev, pager, next"
              @current-change="changeSummaryPage"
            />
          </footer>
        </div>
      </div>
    </div>

    <DispenseSummaryDetailDialog
      v-model:visible="detailVisible"
      :header="detailHeader"
      :lines="detailLines"
      :loading="detailLoading"
    />
  </section>
</template>

<style scoped>
.dispense-summary-module {
  display: flex;
  height: 100%;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  background: #f2f4f7;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.dispense-workspace {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
  flex-direction: column;
  gap: 8px;
  padding: 8px;
}

.dispense-toolbar {
  display: flex;
  min-height: 58px;
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

.toolbar-filters,
.toolbar-actions,
.select-all-label {
  display: flex;
  align-items: center;
  gap: 8px;
}

.toolbar-filters {
  min-width: 0;
  flex-wrap: wrap;
}

.toolbar-actions {
  flex: 0 0 auto;
}

.deadline-label,
.record-count,
.select-all-label {
  color: #606266;
  -webkit-text-fill-color: #606266;
  white-space: nowrap;
}

.deadline-picker {
  width: 205px !important;
}

.dispense-content {
  min-width: 0;
  min-height: 0;
  flex: 1;
  overflow: hidden;
}

.dispense-patient-list {
  height: 100%;
  padding-right: 2px;
  overflow: auto;
}

.dispense-patient-card {
  margin-bottom: 8px;
  overflow: hidden;
  border: 1px solid #e4e7ed;
  border-radius: 7px;
  background: #fff;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.dispense-patient-card__header {
  display: flex;
  min-height: 54px;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding: 8px 12px;
  border-bottom: 1px solid #e4e7ed;
  background: #f8fafc;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.dispense-patient-card__header > div {
  display: flex;
  align-items: center;
  gap: 9px;
}

.dispense-patient-card__header strong {
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 16px;
}

.dispense-patient-card__header span,
.patient-meta span {
  color: #606266;
  -webkit-text-fill-color: #606266;
}

.patient-meta {
  min-width: 0;
  justify-content: flex-end;
  font-size: 13px;
}

.patient-meta span:last-child {
  max-width: 220px;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.order-content {
  display: flex;
  flex-direction: column;
  gap: 4px;
}

.order-content strong {
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.order-content span {
  color: #6b7280;
  -webkit-text-fill-color: #6b7280;
  font-size: 12px;
}

.dispense-schedule-list {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.dispense-schedule-row {
  display: flex;
  align-items: flex-start;
  gap: 8px;
}

.schedule-date {
  min-width: 38px;
  padding-top: 4px;
  color: #606266;
  -webkit-text-fill-color: #606266;
  font-size: 12px;
}

.schedule-slots {
  display: flex;
  flex-wrap: wrap;
  gap: 6px;
}

.dispense-slot {
  margin-right: 0;
}

.dispense-order-table,
.summary-table-card {
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.dispense-order-table :deep(.el-table),
.dispense-order-table :deep(.el-table th.el-table__cell),
.dispense-order-table :deep(.el-table td.el-table__cell),
.summary-table-card :deep(.el-table),
.summary-table-card :deep(.el-table th.el-table__cell),
.summary-table-card :deep(.el-table td.el-table__cell) {
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.summary-table-card {
  display: flex;
  height: 100%;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  border: 1px solid #e4e7ed;
  border-radius: 6px;
  background: #fff;
}

.summary-table-card :deep(.el-table) {
  min-height: 0;
  flex: 1;
}

.summary-pagination {
  display: flex;
  min-height: 46px;
  flex: 0 0 auto;
  align-items: center;
  justify-content: flex-end;
  padding: 0 12px;
  border-top: 1px solid #e4e7ed;
}

.dispense-summary-module > :deep(.el-empty) {
  min-height: 0;
  flex: 1;
}

@media (max-width: 1366px) {
  .dispense-toolbar {
    align-items: flex-start;
  }

  .deadline-picker {
    width: 185px !important;
  }

  .patient-meta span:last-child {
    display: none;
  }
}
</style>
