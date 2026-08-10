<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  queryNurseStationOrderAudits,
  rejectNurseStationOrders,
  verifyNurseStationOrders,
  type NurseStationOrderAuditRowVO,
  type NurseStationOrderAuditStatus
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'

defineOptions({ name: 'NurseStationOrderAudit' })

const props = defineProps<{
  patients: NurseStationPatientSummary[]
}>()

const emit = defineEmits<{
  patientsChanged: []
}>()

interface PatientOrderCard {
  patient: NurseStationPatientSummary
  orders: NurseStationOrderAuditRowVO[]
}

const statusTabs: Array<{ label: string; value: NurseStationOrderAuditStatus }> = [
  { label: '未校对', value: 'PENDING' },
  { label: '已校对', value: 'PASSED' }
]

const activeStatus = ref<NurseStationOrderAuditStatus>('PENDING')
const loading = ref(false)
const actionLoading = ref(false)
const ordersByEncounterId = ref(new Map<string, NurseStationOrderAuditRowVO[]>())
const selectedRowKeys = ref(new Set<string>())
const expandedPatientIds = ref<string[]>([])
let requestSerial = 0

const encounterIds = computed(() => props.patients.map((patient) => patient.encounterId))
const visibleCards = computed<PatientOrderCard[]>(() =>
  props.patients.map((patient) => ({
    patient,
    orders: ordersByEncounterId.value.get(patient.encounterId) ?? []
  }))
)
const allVisibleOrderRows = computed(() => visibleCards.value.flatMap((card) => card.orders))
/** Deduplicate writable rows by backend request identity. */
const writableVisibleOrderRows = computed(() => {
  const seen = new Set<string>()
  return allVisibleOrderRows.value.filter((row) => {
    const key = `${row.orderType}:${row.requestId}`
    if (seen.has(key)) return false
    seen.add(key)
    return true
  })
})
const writeActionsEnabled = computed(() => activeStatus.value === 'PENDING')
const selectedAll = computed({
  get: () =>
    writableVisibleOrderRows.value.length > 0 &&
    selectedRowKeys.value.size === writableVisibleOrderRows.value.length,
  set: (checked: boolean) => {
    selectedRowKeys.value = checked
      ? new Set(writableVisibleOrderRows.value.map((row) => row.listRowKey))
      : new Set()
  }
})

/** Reload when the selected patients or status changes and ignore stale responses. */
async function loadOrders() {
  const serial = ++requestSerial
  selectedRowKeys.value = new Set()
  ordersByEncounterId.value = new Map()
  if (!encounterIds.value.length) return

  loading.value = true
  try {
    const result = await queryNurseStationOrderAudits({
      patientContexts: props.patients.map(({ encounterId, patientId, patientName }) => ({
        encounterId,
        patientId,
        patientName
      })),
      auditStatus: activeStatus.value
    })
    if (serial !== requestSerial) return
    ordersByEncounterId.value = new Map(
      result.groups.map((group) => [group.encounterId, group.orders])
    )
    // Select all returned orders by default.
    selectedRowKeys.value = new Set(
      result.groups.flatMap((group) => group.orders.map((row) => row.listRowKey))
    )
  } catch (error) {
    if (serial !== requestSerial) return
    ElMessage.error(error instanceof Error ? error.message : '加载医嘱校对列表失败')
  } finally {
    if (serial === requestSerial) loading.value = false
  }
}

watch([encounterIds, activeStatus], () => void loadOrders(), { immediate: true })

watch(
  () => props.patients,
  (patients) => {
    expandedPatientIds.value = patients.map((patient) => patient.encounterId)
  },
  { immediate: true }
)

function isSelected(row: NurseStationOrderAuditRowVO) {
  return selectedRowKeys.value.has(row.listRowKey)
}

function toggleRowSelection(row: NurseStationOrderAuditRowVO, checked: boolean) {
  const next = new Set(selectedRowKeys.value)
  if (checked) next.add(row.listRowKey)
  else next.delete(row.listRowKey)
  selectedRowKeys.value = next
}

function collectSelectedRows() {
  return writableVisibleOrderRows.value.filter((row) => selectedRowKeys.value.has(row.listRowKey))
}

function validateSelection(rows: NurseStationOrderAuditRowVO[]) {
  if (!rows.length) {
    ElMessage.warning('请先勾选医嘱')
    return false
  }
  return true
}

function mapWriteItems(rows: NurseStationOrderAuditRowVO[]) {
  return rows.map((row) => ({
    orderType: row.orderType,
    requestId: row.requestId
  }))
}

async function finishWrite(processedCount: number, actionText: string) {
  if (processedCount > 0) ElMessage.success(`已成功${actionText} ${processedCount} 条医嘱`)
  else ElMessage.warning('未处理任何医嘱，请刷新后重试')
  await loadOrders()
  emit('patientsChanged')
}

/** Verify selected orders on the pending tab. */
async function handleVerify() {
  if (!writeActionsEnabled.value) return
  if (actionLoading.value) return
  const rows = collectSelectedRows()
  if (!validateSelection(rows)) return
  try {
    await ElMessageBox.confirm(`确认校对通过已选 ${rows.length} 条医嘱？`, '校对通过', {
      type: 'warning',
      confirmButtonText: '确定',
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  actionLoading.value = true
  try {
    const result = await verifyNurseStationOrders({
      items: mapWriteItems(rows)
    })
    await finishWrite(result.processedCount, '校对')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '校对医嘱失败，请刷新后重试')
  } finally {
    actionLoading.value = false
  }
}

/** Reject selected orders with one required reason. */
async function handleReject() {
  if (!writeActionsEnabled.value) return
  if (actionLoading.value) return
  const rows = collectSelectedRows()
  if (!validateSelection(rows)) return

  let reason: string
  try {
    const result = await ElMessageBox.prompt('请输入退回原因', '退回医嘱', {
      confirmButtonText: '确定退回',
      cancelButtonText: '取消',
      inputType: 'textarea',
      inputPlaceholder: '退回原因',
      inputValidator: (value) => {
        const normalized = value.trim()
        if (!normalized) return '请输入退回原因'
        return normalized.length <= 500 || '退回原因最多 500 字'
      }
    })
    reason = String(result.value ?? '').trim()
  } catch {
    return
  }

  actionLoading.value = true
  try {
    const result = await rejectNurseStationOrders({
      items: mapWriteItems(rows),
      reason
    })
    await finishWrite(result.processedCount, '退回')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '退回医嘱失败，请刷新后重试')
  } finally {
    actionLoading.value = false
  }
}

function orderKindText(row: NurseStationOrderAuditRowVO) {
  return row.orderType === 'MED_REQUEST' ? '药品' : '诊疗'
}

/**
 * Element Plus exposes a generic table row type, so restore the page row type here.
 */
function asAuditRow(row: unknown): NurseStationOrderAuditRowVO {
  return row as NurseStationOrderAuditRowVO
}
</script>

<template>
  <section v-loading="loading || actionLoading" class="order-audit-module">
    <header class="order-audit-toolbar">
      <div class="order-audit-toolbar__filters">
        <el-radio-group v-model="activeStatus" size="small">
          <el-radio-button v-for="item in statusTabs" :key="item.value" :value="item.value">
            {{ item.label }}
          </el-radio-button>
        </el-radio-group>
        <el-button size="small" plain @click="loadOrders">查询</el-button>
      </div>
      <div class="order-audit-toolbar__actions">
        <span>{{ visibleCards.length }}位患者</span>
        <label class="select-all-control">
          全选
          <el-switch
            v-model="selectedAll"
            :disabled="writableVisibleOrderRows.length === 0 || !writeActionsEnabled"
            size="small"
          />
        </label>
        <el-button
          size="small"
          type="primary"
          :disabled="!writeActionsEnabled"
          @click="handleVerify"
        >
          校对通过
        </el-button>
        <el-button
          size="small"
          type="danger"
          plain
          :disabled="!writeActionsEnabled"
          @click="handleReject"
        >
          退回
        </el-button>
      </div>
    </header>

    <div v-if="visibleCards.length" class="order-audit-scroll">
      <el-collapse v-model="expandedPatientIds" class="patient-order-collapse">
        <el-collapse-item
          v-for="card in visibleCards"
          :key="card.patient.encounterId"
          :name="card.patient.encounterId"
          class="patient-order-card"
        >
          <template #title>
            <div class="patient-order-card__header">
              <div class="patient-order-card__identity">
                <strong :class="{ 'is-unassigned': !card.patient.bedName }">
                  {{ card.patient.bedName || '未分床' }}
                </strong>
                <span class="patient-name">{{ card.patient.patientName }}</span>
                <span>{{ card.patient.ageText || '—' }}</span>
                <span class="patient-diagnosis" :title="card.patient.diagnosisSummary">
                  {{ card.patient.diagnosisSummary || '—' }}
                </span>
              </div>
              <span class="patient-balance">
                预交余额
                <strong>{{ Number(card.patient.availableBalance ?? 0).toFixed(2) }} 元</strong>
              </span>
            </div>
          </template>

          <div class="patient-order-card__body">
            <el-table
              v-if="card.orders.length"
              :data="card.orders"
              row-key="listRowKey"
              stripe
              class="order-audit-table"
            >
              <el-table-column align="center" width="52">
                <template #default="{ row }">
                  <el-checkbox
                    :model-value="isSelected(asAuditRow(row))"
                    :disabled="!writeActionsEnabled"
                    @change="toggleRowSelection(asAuditRow(row), Boolean($event))"
                  />
                </template>
              </el-table-column>
              <el-table-column label="类型" align="center" min-width="108">
                <template #default="{ row }">
                  <span>{{ orderKindText(asAuditRow(row)) }}</span>
                </template>
              </el-table-column>
              <el-table-column label="医嘱内容" min-width="330">
                <template #default="{ row }">
                  <span>{{ row.content || '—' }}</span>
                </template>
              </el-table-column>
            </el-table>
            <el-empty v-else :image-size="54" description="当前筛选下暂无医嘱" />
          </div>
        </el-collapse-item>
      </el-collapse>
    </div>
    <el-empty v-else class="order-audit-empty" description="请先从左侧选择患者" />
  </section>
</template>

<style scoped>
.order-audit-module {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  background: #eef9fd;
  color: var(--nurse-text-regular, #606266);
}

.order-audit-toolbar {
  display: flex;
  min-height: 50px;
  flex-wrap: wrap;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  padding: 6px 10px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.order-audit-toolbar__filters,
.order-audit-toolbar__actions,
.select-all-control {
  display: flex;
  align-items: center;
  gap: 8px;
}

.order-audit-toolbar__filters {
  min-width: 0;
  flex-wrap: wrap;
}

.order-audit-toolbar__actions {
  flex-wrap: wrap;
  justify-content: flex-end;
  color: var(--nurse-text-secondary, #6b7280);
  font-size: 12px;
}

.select-all-control {
  color: var(--nurse-text-regular, #606266);
}

.order-audit-scroll {
  min-height: 0;
  flex: 1;
  overflow: auto;
  padding: 10px;
}

.patient-order-collapse {
  border: 0;
}

.patient-order-card {
  margin-bottom: 10px;
  overflow: hidden;
  border: 2px solid var(--el-border-color-lighter);
  border-radius: 12px;
  background: var(--el-bg-color);
}

.patient-order-card:last-child {
  margin-bottom: 0;
}

.patient-order-collapse :deep(.el-collapse-item__header) {
  min-height: 44px;
  border-bottom: 0;
  background: var(--el-bg-color);
  color: var(--nurse-text-primary, #303133);
  line-height: normal;
}

.patient-order-collapse :deep(.el-collapse-item__arrow) {
  order: -1;
  margin: 0 6px 0 10px;
}

.patient-order-collapse :deep(.el-collapse-item__wrap),
.patient-order-collapse :deep(.el-collapse-item__content) {
  border-bottom: 0;
}

.patient-order-collapse :deep(.el-collapse-item__content) {
  padding-bottom: 0;
}

.patient-order-card__header {
  display: flex;
  width: 100%;
  min-width: 0;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding-right: 10px;
}

.patient-order-card__identity {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 9px;
  color: var(--nurse-text-regular, #606266);
  font-size: 14px;
}

.patient-order-card__identity > strong {
  color: var(--nurse-text-primary, #303133);
  font-size: 16px;
  font-weight: 600;
}

.patient-order-card__identity > strong.is-unassigned {
  color: var(--el-color-warning);
}

.patient-name {
  color: var(--nurse-text-primary, #303133);
  font-weight: 500;
}

.patient-diagnosis {
  max-width: 280px;
  overflow: hidden;
  color: var(--nurse-text-secondary, #6b7280);
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-balance {
  flex-shrink: 0;
  color: var(--nurse-text-secondary, #6b7280);
  font-size: 13px;
}

.patient-balance strong {
  margin-left: 4px;
  color: #c46f2f;
  font-weight: 600;
}

.patient-order-card__body {
  padding: 8px 10px 10px;
}

.order-audit-table {
  width: 100%;
  --el-table-text-color: var(--nurse-text-primary, #303133);
  --el-table-header-text-color: var(--nurse-text-regular, #606266);
}

.order-audit-table :deep(th.el-table__cell) {
  padding: 5px 0;
  border-right: 0;
  background: #eef9fd;
  font-weight: 600;
}

.order-audit-table :deep(td.el-table__cell) {
  padding: 8px 0;
  border-right: 0;
  color: var(--nurse-text-primary, #303133);
}

.order-audit-table :deep(.el-table__inner-wrapper::before) {
  display: none;
}

.patient-order-card__body :deep(.el-empty) {
  padding: 16px 0;
}

.order-audit-empty {
  min-height: 0;
  flex: 1;
}

@media (max-width: 1366px) {
  .order-audit-toolbar {
    align-items: flex-start;
  }

  .patient-diagnosis {
    max-width: 180px;
  }
}
</style>
