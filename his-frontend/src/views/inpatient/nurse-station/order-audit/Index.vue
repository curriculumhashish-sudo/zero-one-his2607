<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  queryNurseStationOrderAudits,
  rejectNurseStationOrders,
  verifyNurseStationOrders,
  type NurseStationOrderAuditMedicineCategory,
  type NurseStationOrderAuditRowVO,
  type NurseStationOrderAuditStatus,
  type NurseStationOrderAuditTherapyFilter
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

const WRITE_BATCH_LIMIT = 50
const statusTabs: Array<{ label: string; value: NurseStationOrderAuditStatus }> = [
  { label: '未校对', value: 'PENDING' },
  { label: '已校对', value: 'PASSED' },
  { label: '已停止', value: 'STOPPED' },
  { label: '已作废', value: 'SUSPENDED' }
]
const medicineTabs: Array<{
  label: string
  value: NurseStationOrderAuditMedicineCategory
}> = [
  { label: '西药', value: 'WEST' },
  { label: '中药', value: 'TCM' }
]

const activeStatus = ref<NurseStationOrderAuditStatus>('PENDING')
const activeMedicineCategory = ref<NurseStationOrderAuditMedicineCategory>('WEST')
const activeTherapyType = ref<NurseStationOrderAuditTherapyFilter>('ALL')
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
const writeActionsEnabled = computed(() => activeStatus.value === 'PENDING')
const selectedAll = computed({
  get: () =>
    allVisibleOrderRows.value.length > 0 &&
    selectedRowKeys.value.size === allVisibleOrderRows.value.length,
  set: (checked: boolean) => {
    selectedRowKeys.value = checked
      ? new Set(allVisibleOrderRows.value.map((row) => row.listRowKey))
      : new Set()
  }
})

/** 筛选或左侧患者变化时重新加载，并防止较慢请求覆盖新结果。 */
async function loadOrders() {
  const serial = ++requestSerial
  selectedRowKeys.value = new Set()
  ordersByEncounterId.value = new Map()
  if (!encounterIds.value.length) return

  loading.value = true
  try {
    const result = await queryNurseStationOrderAudits({
      encounterIds: encounterIds.value,
      auditStatus: activeStatus.value,
      therapyType: activeTherapyType.value,
      medicineCategory: activeMedicineCategory.value
    })
    if (serial !== requestSerial) return
    ordersByEncounterId.value = new Map(
      result.groups.map((group) => [group.encounterId, group.orders])
    )
    // 与原型一致，查询完成后默认勾选当前列表全部医嘱。
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

watch(
  [encounterIds, activeStatus, activeMedicineCategory, activeTherapyType],
  () => void loadOrders(),
  { immediate: true }
)

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
  return allVisibleOrderRows.value.filter((row) => selectedRowKeys.value.has(row.listRowKey))
}

function validateSelection(rows: NurseStationOrderAuditRowVO[]) {
  if (!rows.length) {
    ElMessage.warning('请先勾选医嘱')
    return false
  }
  if (rows.length > WRITE_BATCH_LIMIT) {
    ElMessage.warning(`单次最多处理 ${WRITE_BATCH_LIMIT} 条医嘱`)
    return false
  }
  return true
}

function createIdempotencyKey(action: string) {
  return `${action}-${Date.now()}-${Math.random().toString(36).slice(2, 10)}`
}

function mapWriteItems(rows: NurseStationOrderAuditRowVO[]) {
  return rows.map((row) => ({
    orderType: row.orderType,
    requestId: row.requestId,
    expectedVersion: row.version
  }))
}

async function finishWrite(processedCount: number, actionText: string) {
  if (processedCount > 0) ElMessage.success(`已成功${actionText} ${processedCount} 条医嘱`)
  else ElMessage.warning('未处理任何医嘱，请刷新后重试')
  await loadOrders()
  emit('patientsChanged')
}

/** 批量校对通过，仅未校对页签可执行。 */
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
      idempotencyKey: createIdempotencyKey('verify'),
      items: mapWriteItems(rows)
    })
    await finishWrite(result.processedCount, '校对')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '校对医嘱失败，请刷新后重试')
  } finally {
    actionLoading.value = false
  }
}

/** 批量退回，并允许录入统一原因。 */
async function handleReject() {
  if (!writeActionsEnabled.value) return
  if (actionLoading.value) return
  const rows = collectSelectedRows()
  if (!validateSelection(rows)) return

  let reason = ''
  try {
    const result = await ElMessageBox.prompt('请输入退回原因（可选）', '退回医嘱', {
      confirmButtonText: '确定退回',
      cancelButtonText: '取消',
      inputType: 'textarea',
      inputPlaceholder: '退回原因',
      inputValidator: (value) => !value || value.length <= 500 || '退回原因最多 500 字'
    })
    reason = String(result.value ?? '').trim()
  } catch {
    return
  }

  actionLoading.value = true
  try {
    const result = await rejectNurseStationOrders({
      idempotencyKey: createIdempotencyKey('reject'),
      items: mapWriteItems(rows),
      reason: reason || undefined
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

function therapyClass(row: NurseStationOrderAuditRowVO) {
  if (row.therapyEnum === 'LONG_TERM') return 'is-long'
  if (row.therapyEnum === 'TEMPORARY') return 'is-temporary'
  return 'is-discharge'
}

function orderTitle(row: NurseStationOrderAuditRowVO) {
  if (row.tcmFlag) return row.adviceDisplayName
  return row.specificationText
    ? `${row.adviceDisplayName}【${row.specificationText}】`
    : row.adviceDisplayName
}

function dosageText(row: NurseStationOrderAuditRowVO) {
  const dose = row.dosageDisplay || '—'
  return row.tcmFlag && row.chineseHerbsDoseQuantity
    ? `${dose} ${row.chineseHerbsDoseQuantity}付`
    : dose
}

function extensionText(row: NurseStationOrderAuditRowVO) {
  return [row.usageDisplay, row.frequencyDisplay].filter(Boolean).join('，')
}

/**
 * Element Plus 表格插槽只提供通用 DefaultRow 类型。
 * 数据源本身已经由 NurseStationOrderAuditRowVO 约束，这里集中恢复类型，避免模板到处断言。
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
        <el-radio-group v-model="activeMedicineCategory" size="small">
          <el-radio-button v-for="item in medicineTabs" :key="item.value" :value="item.value">
            {{ item.label }}
          </el-radio-button>
        </el-radio-group>
        <el-radio-group v-model="activeTherapyType" size="small" class="therapy-filter">
          <el-radio value="ALL">全部</el-radio>
          <el-radio value="LONG_TERM">长期</el-radio>
          <el-radio value="TEMPORARY">临时</el-radio>
        </el-radio-group>
        <el-button size="small" plain @click="loadOrders">查询</el-button>
      </div>
      <div class="order-audit-toolbar__actions">
        <span>{{ visibleCards.length }}位患者</span>
        <label class="select-all-control">
          全选
          <el-switch
            v-model="selectedAll"
            :disabled="allVisibleOrderRows.length === 0 || !writeActionsEnabled"
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
                  <span class="type-divider"> / </span>
                  <span class="therapy-text" :class="therapyClass(asAuditRow(row))">
                    {{ row.therapyDisplay || '—' }}
                  </span>
                </template>
              </el-table-column>
              <el-table-column label="医嘱内容" min-width="330">
                <template #default="{ row }">
                  <div class="order-content">
                    <div class="order-content__detail">
                      <div>
                        <strong>{{ orderTitle(asAuditRow(row)) }}</strong>
                        <span>{{ dosageText(asAuditRow(row)) }}</span>
                      </div>
                      <small v-if="extensionText(asAuditRow(row))">
                        {{ extensionText(asAuditRow(row)) }}
                      </small>
                    </div>
                    <span
                      v-if="row.statusEnum === 'STOPPED' && row.stopTime"
                      class="stop-badge"
                      title="已停嘱"
                    >
                      停
                    </span>
                  </div>
                </template>
              </el-table-column>
              <el-table-column
                prop="performerLocationDisplay"
                label="执行科室"
                align="center"
                min-width="105"
              />
              <el-table-column
                prop="authoredTime"
                label="签发时间"
                align="center"
                min-width="145"
              />
              <el-table-column
                prop="requesterName"
                label="开立医生"
                align="center"
                min-width="96"
              />
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

.therapy-filter :deep(.el-radio) {
  height: auto;
  margin-right: 8px;
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

.type-divider {
  color: var(--nurse-text-secondary, #6b7280);
}

.therapy-text {
  font-weight: 500;
}

.therapy-text.is-long {
  color: #94654f;
}

.therapy-text.is-temporary {
  color: #267b9b;
}

.therapy-text.is-discharge {
  color: #4e7c35;
}

.order-content {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 6px;
}

.order-content__detail {
  min-width: 0;
  flex: 1;
}

.order-content__detail > div {
  display: flex;
  min-width: 0;
  align-items: baseline;
  justify-content: space-between;
  gap: 12px;
}

.order-content__detail strong {
  min-width: 0;
  overflow: hidden;
  color: var(--nurse-text-primary, #303133);
  font-weight: 500;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.order-content__detail span {
  flex-shrink: 0;
  color: var(--nurse-text-regular, #606266);
  font-size: 12px;
}

.order-content__detail small {
  display: block;
  margin-top: 3px;
  padding-top: 2px;
  border-top: 1px dashed #cbd5e1;
  color: #2563eb;
  font-size: 12px;
}

.stop-badge {
  display: inline-flex;
  width: 20px;
  height: 20px;
  flex-shrink: 0;
  align-items: center;
  justify-content: center;
  border-radius: 4px;
  background: var(--el-color-danger);
  color: #fff;
  font-size: 12px;
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
