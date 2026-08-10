<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  executeNurseStationOrders,
  queryNurseStationOrderExecutions,
  type NurseStationOrderExecuteRowVO
} from '@/apis/inpatient/nurse-station'
import { useUserStore } from '@/stores/user'

import type { NurseStationPatientSummary } from '../type'

defineOptions({ name: 'NurseStationOrderExecute' })

const props = defineProps<{
  patients: NurseStationPatientSummary[]
}>()

const emit = defineEmits<{
  patientsChanged: []
}>()

interface PatientExecuteCard {
  patient: NurseStationPatientSummary
  orders: NurseStationOrderExecuteRowVO[]
}

const WRITE_BATCH_LIMIT = 50
const userStore = useUserStore()
const loading = ref(false)
const actionLoading = ref(false)
const ordersByEncounterId = ref(new Map<string, NurseStationOrderExecuteRowVO[]>())
const selectedOrderIds = ref(new Set<string>())
const expandedPatientIds = ref<string[]>([])
let requestSerial = 0

const encounterIds = computed(() => props.patients.map((patient) => patient.encounterId))
const encounterIdsKey = computed(() => encounterIds.value.join('|'))
const performerId = computed(() => userStore.getUser?.id ?? '')
const visibleCards = computed<PatientExecuteCard[]>(() =>
  props.patients.map((patient) => ({
    patient,
    orders: ordersByEncounterId.value.get(patient.encounterId) ?? []
  }))
)
const allVisibleOrders = computed(() => visibleCards.value.flatMap((card) => card.orders))
const selectedOrders = computed(() =>
  allVisibleOrders.value.filter((order) => selectedOrderIds.value.has(order.id))
)
const selectedAll = computed({
  get: () =>
    allVisibleOrders.value.length > 0 &&
    selectedOrderIds.value.size === allVisibleOrders.value.length,
  set: (checked: boolean) => {
    selectedOrderIds.value = checked
      ? new Set(allVisibleOrders.value.map((order) => order.id))
      : new Set()
  }
})

function toggleOrderSelection(orderId: string, checked: boolean) {
  const next = new Set(selectedOrderIds.value)
  if (checked) next.add(orderId)
  else next.delete(orderId)
  selectedOrderIds.value = next
}

function asExecuteRow(row: unknown): NurseStationOrderExecuteRowVO {
  return row as NurseStationOrderExecuteRowVO
}

async function loadOrders() {
  const serial = ++requestSerial
  selectedOrderIds.value = new Set()
  ordersByEncounterId.value = new Map()
  if (!encounterIds.value.length) return

  loading.value = true
  try {
    const result = await queryNurseStationOrderExecutions({
      encounterIds: encounterIds.value,
      pageSize: 100
    })
    if (serial !== requestSerial) return
    ordersByEncounterId.value = new Map(
      result.groups.map((group) => [group.encounterId, group.orders])
    )
    selectedOrderIds.value = new Set(
      result.groups.flatMap((group) => group.orders.map((order) => order.id))
    )
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '加载待执行医嘱失败')
  } finally {
    if (serial === requestSerial) loading.value = false
  }
}

watch(encounterIdsKey, () => void loadOrders(), { immediate: true })
watch(
  () => props.patients,
  (patients) => {
    expandedPatientIds.value = patients.map((patient) => patient.encounterId)
  },
  { immediate: true }
)

async function handleStartExecution() {
  const orders = selectedOrders.value
  if (!orders.length) {
    ElMessage.warning('请先勾选医嘱')
    return
  }
  if (orders.length > WRITE_BATCH_LIMIT) {
    ElMessage.warning(`单次最多处理 ${WRITE_BATCH_LIMIT} 条医嘱`)
    return
  }
  if (!performerId.value) {
    ElMessage.warning('当前登录用户缺少执行人 ID，请重新登录后重试')
    return
  }

  try {
    await ElMessageBox.confirm(`确认开始执行已选 ${orders.length} 条医嘱？`, '开始执行', {
      type: 'warning',
      confirmButtonText: '确定',
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  actionLoading.value = true
  try {
    const result = await executeNurseStationOrders({
      performerId: performerId.value,
      serviceRequestIds: orders.map((order) => order.id)
    })
    if (result.processedCount > 0) {
      ElMessage.success(`已开始执行 ${result.processedCount} 条医嘱`)
    }
    if (!result.success) {
      const failedCount = result.results.length - result.processedCount
      ElMessage.warning(`${failedCount} 条医嘱处理失败，请刷新后重试`)
    }
    await loadOrders()
    emit('patientsChanged')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '开始执行失败')
  } finally {
    actionLoading.value = false
  }
}

function statusText(statusEnum: string) {
  return statusEnum.toLowerCase() === 'verified' ? '已校对' : statusEnum || '—'
}

function intentText(intentEnum?: string) {
  const normalized = intentEnum?.toLowerCase()
  if (normalized === 'long_term') return '长期'
  if (normalized === 'temporary') return '临时'
  return intentEnum || '—'
}

function quantityText(order: NurseStationOrderExecuteRowVO) {
  if (order.quantity === undefined) return '—'
  return `${order.quantity}${order.unitCode ? ` ${order.unitCode}` : ''}`
}

function noteText(order: NurseStationOrderExecuteRowVO) {
  return [order.remark, order.entrustContent].filter(Boolean).join('；') || '—'
}
</script>

<template>
  <section v-loading="loading || actionLoading" class="order-execute-module">
    <header class="order-execute-toolbar">
      <div class="order-execute-toolbar__summary">
        <el-tag type="primary" effect="light">已校对待开始</el-tag>
        <span>{{ visibleCards.length }} 位患者</span>
        <span>{{ allVisibleOrders.length }} 条诊疗医嘱</span>
      </div>
      <div class="order-execute-toolbar__actions">
        <label class="select-all-control">
          全选
          <el-switch v-model="selectedAll" :disabled="allVisibleOrders.length === 0" size="small" />
        </label>
        <el-button size="small" plain @click="loadOrders">刷新</el-button>
        <el-button
          size="small"
          type="primary"
          :disabled="selectedOrders.length === 0"
          @click="handleStartExecution"
        >
          开始执行
        </el-button>
      </div>
    </header>

    <div v-if="visibleCards.length" class="order-execute-scroll">
      <el-collapse v-model="expandedPatientIds" class="patient-execute-collapse">
        <el-collapse-item
          v-for="card in visibleCards"
          :key="card.patient.encounterId"
          :name="card.patient.encounterId"
          class="patient-execute-card"
        >
          <template #title>
            <div class="patient-execute-card__header">
              <div class="patient-execute-card__identity">
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

          <div class="patient-execute-card__body">
            <el-table
              v-if="card.orders.length"
              :data="card.orders"
              row-key="listRowKey"
              stripe
              class="order-execute-table"
            >
              <el-table-column label="选择" align="center" width="58">
                <template #default="{ row }">
                  <el-checkbox
                    :model-value="selectedOrderIds.has(row.id)"
                    @change="toggleOrderSelection(row.id, Boolean($event))"
                  />
                </template>
              </el-table-column>
              <el-table-column label="状态" align="center" width="88">
                <template #default="{ row }">
                  <el-tag size="small" effect="light">{{ statusText(row.statusEnum) }}</el-tag>
                </template>
              </el-table-column>
              <el-table-column label="业务号 / 处方号" min-width="175">
                <template #default="{ row }">
                  <div class="identifier-cell">
                    <strong>{{ row.busNo || row.id }}</strong>
                    <small v-if="row.prescriptionNo">{{ row.prescriptionNo }}</small>
                  </div>
                </template>
              </el-table-column>
              <el-table-column label="分类 / 长临" align="center" min-width="125">
                <template #default="{ row }">
                  <span>{{ row.categoryEnum || '—' }}</span>
                  <span class="type-divider"> / </span>
                  <span>{{ intentText(row.intentEnum) }}</span>
                </template>
              </el-table-column>
              <el-table-column label="数量" align="center" min-width="92">
                <template #default="{ row }">{{ quantityText(asExecuteRow(row)) }}</template>
              </el-table-column>
              <el-table-column label="开立时间" align="center" min-width="155">
                <template #default="{ row }">{{ row.authoredTime || '—' }}</template>
              </el-table-column>
              <el-table-column label="备注 / 嘱托" min-width="230">
                <template #default="{ row }">
                  <span class="note-text" :title="noteText(asExecuteRow(row))">
                    {{ noteText(asExecuteRow(row)) }}
                  </span>
                </template>
              </el-table-column>
            </el-table>
            <el-empty v-else :image-size="54" description="当前患者暂无已校对诊疗医嘱" />
          </div>
        </el-collapse-item>
      </el-collapse>
    </div>
    <el-empty v-else class="order-execute-empty" description="请先从左侧选择患者" />
  </section>
</template>

<style scoped>
.order-execute-module {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  background: #eef9fd;
  color: var(--nurse-text-regular, #606266);
}

.order-execute-toolbar {
  display: flex;
  min-height: 50px;
  flex-wrap: wrap;
  align-items: center;
  justify-content: space-between;
  gap: 10px;
  padding: 6px 10px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.order-execute-toolbar__summary,
.order-execute-toolbar__actions,
.select-all-control {
  display: flex;
  align-items: center;
  gap: 8px;
}

.order-execute-toolbar__summary,
.select-all-control {
  color: var(--nurse-text-secondary, #6b7280);
  font-size: 13px;
}

.order-execute-scroll {
  min-height: 0;
  flex: 1;
  overflow: auto;
  padding: 10px;
}

.patient-execute-collapse {
  border: 0;
}

.patient-execute-card {
  margin-bottom: 10px;
  overflow: hidden;
  border: 2px solid var(--el-border-color-lighter);
  border-radius: 12px;
  background: var(--el-bg-color);
}

.patient-execute-card:last-child {
  margin-bottom: 0;
}

.patient-execute-collapse :deep(.el-collapse-item__header) {
  min-height: 44px;
  border-bottom: 0;
  background: var(--el-bg-color);
  color: var(--nurse-text-primary, #303133);
  line-height: normal;
}

.patient-execute-collapse :deep(.el-collapse-item__arrow) {
  order: -1;
  margin: 0 6px 0 10px;
}

.patient-execute-collapse :deep(.el-collapse-item__wrap),
.patient-execute-collapse :deep(.el-collapse-item__content) {
  border-bottom: 0;
}

.patient-execute-collapse :deep(.el-collapse-item__content) {
  padding-bottom: 0;
}

.patient-execute-card__header {
  display: flex;
  width: 100%;
  min-width: 0;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding-right: 10px;
}

.patient-execute-card__identity {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 9px;
  color: var(--nurse-text-regular, #606266);
  font-size: 14px;
}

.patient-execute-card__identity > strong {
  color: var(--nurse-text-primary, #303133);
  font-size: 16px;
  font-weight: 600;
}

.patient-execute-card__identity > strong.is-unassigned {
  color: var(--el-color-warning);
}

.patient-name {
  color: var(--nurse-text-primary, #303133);
  font-weight: 500;
}

.patient-diagnosis {
  max-width: 260px;
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
  color: #b86122;
  font-weight: 600;
}

.patient-execute-card__body {
  padding: 8px 10px 10px;
}

.order-execute-table {
  width: 100%;
  --el-table-text-color: var(--nurse-text-primary, #303133);
  --el-table-header-text-color: var(--nurse-text-regular, #606266);
}

.order-execute-table :deep(th.el-table__cell) {
  padding: 5px 0;
  border-right: 0;
  background: #eef9fd;
  font-weight: 600;
}

.order-execute-table :deep(td.el-table__cell) {
  padding: 8px 0;
  border-right: 0;
}

.order-execute-table :deep(.el-table__inner-wrapper::before) {
  display: none;
}

.identifier-cell {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 3px;
}

.identifier-cell strong,
.identifier-cell small,
.note-text {
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.identifier-cell small,
.type-divider {
  color: var(--nurse-text-secondary, #6b7280);
}

.note-text {
  display: block;
}

.patient-execute-card__body :deep(.el-empty) {
  padding: 16px 0;
}

.order-execute-empty {
  min-height: 0;
  flex: 1;
}

@media (max-width: 1366px) {
  .patient-diagnosis {
    max-width: 160px;
  }
}
</style>
