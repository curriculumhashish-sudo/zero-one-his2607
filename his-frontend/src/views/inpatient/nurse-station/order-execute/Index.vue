<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import dayjs from 'dayjs'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  cancelNurseStationExecutions,
  executeNurseStationOrders,
  queryNurseStationOrderExecutions,
  skipNurseStationOrders,
  type NurseStationOrderExecuteMedicineCategory,
  type NurseStationOrderExecuteRowVO,
  type NurseStationOrderExecuteSlotVO,
  type NurseStationOrderExecuteStatus,
  type NurseStationOrderExecuteTherapyFilter,
  type NurseStationOrderExecuteWriteItemDTO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'

defineOptions({ name: 'NurseStationOrderExecute' })

const props = defineProps<{
  /** 左侧患者面板中已经勾选的患者。 */
  patients: NurseStationPatientSummary[]
}>()

const emit = defineEmits<{
  /** 写操作成功后通知父页面刷新左侧患者数量和顶部统计。 */
  patientsChanged: []
}>()

interface PatientExecuteCard {
  patient: NurseStationPatientSummary
  orders: NurseStationOrderExecuteRowVO[]
}

interface SelectedSlot {
  row: NurseStationOrderExecuteRowVO
  slot: NurseStationOrderExecuteSlotVO
}

const WRITE_BATCH_LIMIT = 50
const statusTabs: Array<{ label: string; value: NurseStationOrderExecuteStatus }> = [
  { label: '待执行', value: 'PENDING' },
  { label: '已执行', value: 'EXECUTED' },
  { label: '不执行', value: 'SKIPPED' },
  { label: '取消执行', value: 'CANCELLED' }
]
const medicineTabs: Array<{
  label: string
  value: NurseStationOrderExecuteMedicineCategory
}> = [
  { label: '西药', value: 'WEST' },
  { label: '中药', value: 'TCM' }
]

const activeStatus = ref<NurseStationOrderExecuteStatus>('PENDING')
const activeMedicineCategory = ref<NurseStationOrderExecuteMedicineCategory>('WEST')
const activeTherapyType = ref<NurseStationOrderExecuteTherapyFilter>('ALL')
const executionDeadline = ref(dayjs().endOf('day').format('YYYY-MM-DD HH:mm:ss'))
const loading = ref(false)
const actionLoading = ref(false)
const ordersByEncounterId = ref(new Map<string, NurseStationOrderExecuteRowVO[]>())
const selectedSlotKeys = ref(new Set<string>())
const expandedPatientIds = ref<string[]>([])
let requestSerial = 0

const encounterIds = computed(() => props.patients.map((patient) => patient.encounterId))
const visibleCards = computed<PatientExecuteCard[]>(() =>
  props.patients.map((patient) => ({
    patient,
    orders: ordersByEncounterId.value.get(patient.encounterId) ?? []
  }))
)
const allVisibleSlots = computed<SelectedSlot[]>(() =>
  visibleCards.value.flatMap((card) =>
    card.orders.flatMap((row) => row.slots.map((slot) => ({ row, slot })))
  )
)

/** 不执行页签是历史只读列表，其余三个页签都可以勾选计划点。 */
const slotSelectionEnabled = computed(() => activeStatus.value !== 'SKIPPED')
const executeActionEnabled = computed(
  () => activeStatus.value === 'PENDING' || activeStatus.value === 'CANCELLED'
)
const skipActionEnabled = computed(() => activeStatus.value === 'PENDING')
const cancelActionEnabled = computed(() => activeStatus.value === 'EXECUTED')

/**
 * 全选状态只统计页面当前可见的计划点。
 * 这里使用 Set 保存键值，避免勾选一个时误改其它患者或其它筛选页签的数据。
 */
const selectedAll = computed({
  get: () =>
    allVisibleSlots.value.length > 0 &&
    selectedSlotKeys.value.size === allVisibleSlots.value.length,
  set: (checked: boolean) => {
    selectedSlotKeys.value =
      checked && slotSelectionEnabled.value
        ? new Set(allVisibleSlots.value.map(({ row, slot }) => buildSlotKey(row, slot)))
        : new Set()
  }
})

function buildSlotKey(row: NurseStationOrderExecuteRowVO, slot: NurseStationOrderExecuteSlotVO) {
  return `${row.orderType}:${row.orderId}:${slot.planTime}`
}

function isSlotSelected(row: NurseStationOrderExecuteRowVO, slot: NurseStationOrderExecuteSlotVO) {
  return selectedSlotKeys.value.has(buildSlotKey(row, slot))
}

function toggleSlotSelection(
  row: NurseStationOrderExecuteRowVO,
  slot: NurseStationOrderExecuteSlotVO,
  checked: boolean
) {
  const next = new Set(selectedSlotKeys.value)
  const key = buildSlotKey(row, slot)
  if (checked) next.add(key)
  else next.delete(key)
  selectedSlotKeys.value = next
}

/**
 * 按日期分组计划时点，让同一天的多个执行时间显示在同一行。
 * 返回新数组只用于展示，不会修改接口返回的原始顺序和状态。
 */
function groupSlotsByDate(slots: NurseStationOrderExecuteSlotVO[]) {
  const groups = new Map<string, NurseStationOrderExecuteSlotVO[]>()
  slots.forEach((slot) => {
    const date = dayjs(slot.planTime).format('MM-DD')
    const bucket = groups.get(date) ?? []
    bucket.push(slot)
    groups.set(date, bucket)
  })
  return [...groups.entries()].map(([date, groupedSlots]) => ({ date, slots: groupedSlots }))
}

/**
 * 组合筛选或左侧患者变化后重新查询。
 * requestSerial 用来丢弃较慢的旧请求，避免快速切换页签时旧结果覆盖新结果。
 */
async function loadOrders() {
  const serial = ++requestSerial
  selectedSlotKeys.value = new Set()
  ordersByEncounterId.value = new Map()
  if (!encounterIds.value.length) return
  if (!executionDeadline.value) {
    ElMessage.warning('请选择执行截止时间')
    return
  }

  loading.value = true
  try {
    const result = await queryNurseStationOrderExecutions({
      encounterIds: encounterIds.value,
      executeStatus: activeStatus.value,
      therapyType: activeTherapyType.value,
      medicineCategory: activeMedicineCategory.value,
      executionDeadline: executionDeadline.value
    })
    if (serial !== requestSerial) return
    ordersByEncounterId.value = new Map(
      result.groups.map((group) => [group.encounterId, group.orders])
    )
    // 与原型一致：查询完成后默认全选当前可操作计划点，减少护士重复点击。
    if (slotSelectionEnabled.value) {
      selectedSlotKeys.value = new Set(
        result.groups.flatMap((group) =>
          group.orders.flatMap((row) => row.slots.map((slot) => buildSlotKey(row, slot)))
        )
      )
    }
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '加载医嘱执行列表失败')
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

function collectSelectedSlots() {
  return allVisibleSlots.value.filter(({ row, slot }) => isSlotSelected(row, slot))
}

function validateSelectedSlots(slots: SelectedSlot[]) {
  if (!slots.length) {
    ElMessage.warning('请先勾选计划执行时点')
    return false
  }
  if (slots.length > WRITE_BATCH_LIMIT) {
    ElMessage.warning(`单次最多处理 ${WRITE_BATCH_LIMIT} 个计划点`)
    return false
  }
  return true
}

function createIdempotencyKey(action: string) {
  return `${action}-${Date.now()}-${Math.random().toString(36).slice(2, 10)}`
}

/**
 * Element Plus 的表格插槽把 row 声明成通用 DefaultRow，无法自动保留 data 的泛型。
 * 业务数据实际来自强类型 orders 数组，因此在模板调用强类型函数前统一从这里恢复类型。
 */
function asExecuteRow(row: unknown): NurseStationOrderExecuteRowVO {
  return row as NurseStationOrderExecuteRowVO
}

/**
 * 将页面行和时点转换成写接口需要的结构。
 * accountId 来自患者队列；真实后端会用它校验住院账户并完成绑定项目记账。
 */
function mapWriteItems(slots: SelectedSlot[], includeExecutionDetails: boolean) {
  const patientMap = new Map(props.patients.map((patient) => [patient.encounterId, patient]))
  const actualTime = dayjs().format('YYYY-MM-DD HH:mm:ss')
  return slots.map(({ row, slot }): NurseStationOrderExecuteWriteItemDTO => {
    const patient = patientMap.get(row.encounterId)
    return {
      orderId: row.orderId,
      orderType: row.orderType,
      encounterId: row.encounterId,
      planTime: slot.planTime,
      accountId: includeExecutionDetails ? patient?.accountId : undefined,
      actualTime: includeExecutionDetails ? actualTime : undefined,
      bindItems: includeExecutionDetails ? row.bindItems.map((item) => ({ ...item })) : undefined,
      prescriptionNo: includeExecutionDetails ? row.prescriptionNo : undefined,
      lineRequestIds:
        includeExecutionDetails && row.lineRequestIds ? [...row.lineRequestIds] : undefined
    }
  })
}

async function finishWrite(processedCount: number, actionText: string) {
  if (processedCount > 0) ElMessage.success(`已成功${actionText} ${processedCount} 个计划点`)
  else ElMessage.warning('没有计划点被处理，请刷新后重试')
  await loadOrders()
  emit('patientsChanged')
}

/** 批量执行待执行或已取消的计划点。 */
async function handleExecute() {
  if (!executeActionEnabled.value) return
  const slots = collectSelectedSlots()
  if (!validateSelectedSlots(slots)) return
  const items = mapWriteItems(slots, true)
  if (items.some((item) => !item.accountId)) {
    ElMessage.warning('部分患者缺少住院账户，请刷新患者列表后重试')
    return
  }

  try {
    await ElMessageBox.confirm(`确认执行已选 ${items.length} 个计划点？`, '执行医嘱', {
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
      idempotencyKey: createIdempotencyKey('execute'),
      items
    })
    await finishWrite(result.processedCount, '执行')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '执行医嘱失败')
  } finally {
    actionLoading.value = false
  }
}

/** 批量登记不执行；Mock 暂不强制原因，真实接口可直接接收这里的可选字段。 */
async function handleSkip() {
  if (!skipActionEnabled.value) return
  const slots = collectSelectedSlots()
  if (!validateSelectedSlots(slots)) return
  try {
    await ElMessageBox.confirm(`确认将已选 ${slots.length} 个计划点登记为不执行？`, '不执行', {
      type: 'warning',
      confirmButtonText: '确定',
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  actionLoading.value = true
  try {
    const result = await skipNurseStationOrders({
      idempotencyKey: createIdempotencyKey('skip'),
      items: mapWriteItems(slots, false)
    })
    await finishWrite(result.processedCount, '登记不执行')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '登记不执行失败')
  } finally {
    actionLoading.value = false
  }
}

/** 批量取消已执行记录。取消原因会写入请求，便于后端留痕审计。 */
async function handleCancel() {
  if (!cancelActionEnabled.value) return
  const slots = collectSelectedSlots()
  if (!validateSelectedSlots(slots)) return
  if (slots.some(({ slot }) => !slot.procedureId)) {
    ElMessage.warning('部分计划点缺少执行记录 ID，请刷新后重试')
    return
  }

  let reason = ''
  try {
    const promptResult = await ElMessageBox.prompt(
      `取消已选 ${slots.length} 个计划点后，已发药项目可能生成待退记录。请输入原因：`,
      '取消执行',
      {
        type: 'warning',
        inputType: 'textarea',
        inputPlaceholder: '请输入取消原因',
        inputValidator: (value) => Boolean(value?.trim()) || '取消原因不能为空',
        confirmButtonText: '确定取消',
        cancelButtonText: '返回'
      }
    )
    reason = String(promptResult.value).trim()
  } catch {
    return
  }

  actionLoading.value = true
  try {
    const result = await cancelNurseStationExecutions({
      idempotencyKey: createIdempotencyKey('cancel'),
      reason,
      items: slots.map(({ row, slot }) => ({
        encounterId: row.encounterId,
        procedureId: slot.procedureId as string
      }))
    })
    await finishWrite(result.processedCount, '取消执行')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '取消执行失败')
  } finally {
    actionLoading.value = false
  }
}

function orderKindText(row: NurseStationOrderExecuteRowVO) {
  return row.orderType === 'MED_REQUEST' ? '药品' : '诊疗'
}

function therapyClass(row: NurseStationOrderExecuteRowVO) {
  return row.therapyEnum === 'LONG_TERM' ? 'is-long' : 'is-temporary'
}

function orderTitle(row: NurseStationOrderExecuteRowVO) {
  if (row.tcmFlag) return row.adviceDisplayName
  return row.specificationText
    ? `${row.adviceDisplayName}【${row.specificationText}】`
    : row.adviceDisplayName
}

function dosageText(row: NurseStationOrderExecuteRowVO) {
  const dose = row.dosageDisplay || '—'
  return row.tcmFlag && row.chineseHerbsDoseQuantity
    ? `${dose} ${row.chineseHerbsDoseQuantity}付`
    : dose
}

function extensionText(row: NurseStationOrderExecuteRowVO) {
  return [row.usageDisplay, row.frequencyDisplay].filter(Boolean).join('，')
}

function activeStatusText() {
  return statusTabs.find((item) => item.value === activeStatus.value)?.label ?? '—'
}

function statusTagType() {
  if (activeStatus.value === 'EXECUTED') return 'success'
  if (activeStatus.value === 'SKIPPED') return 'warning'
  if (activeStatus.value === 'CANCELLED') return 'danger'
  return 'primary'
}
</script>

<template>
  <section v-loading="loading || actionLoading" class="order-execute-module">
    <header class="order-execute-toolbar">
      <div class="order-execute-toolbar__filters">
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
        <span class="deadline-label">截止时间</span>
        <el-date-picker
          v-model="executionDeadline"
          type="datetime"
          value-format="YYYY-MM-DD HH:mm:ss"
          format="YYYY-MM-DD HH:mm:ss"
          placeholder="请选择截止时间"
          class="deadline-picker"
          size="small"
        />
        <el-button size="small" plain @click="loadOrders">查询</el-button>
      </div>

      <div class="order-execute-toolbar__actions">
        <span>{{ visibleCards.length }}位患者</span>
        <label v-if="slotSelectionEnabled" class="select-all-control">
          全选
          <el-switch v-model="selectedAll" :disabled="allVisibleSlots.length === 0" size="small" />
        </label>
        <el-button
          size="small"
          type="primary"
          :disabled="!executeActionEnabled"
          @click="handleExecute"
        >
          执行医嘱
        </el-button>
        <el-button
          size="small"
          type="warning"
          plain
          :disabled="!skipActionEnabled"
          @click="handleSkip"
        >
          不执行
        </el-button>
        <el-button
          size="small"
          type="danger"
          plain
          :disabled="!cancelActionEnabled"
          @click="handleCancel"
        >
          取消执行
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
              <el-table-column label="状态" align="center" width="88">
                <template #default>
                  <el-tag size="small" effect="light" :type="statusTagType()">
                    {{ activeStatusText() }}
                  </el-tag>
                </template>
              </el-table-column>
              <el-table-column label="类型" align="center" min-width="100">
                <template #default="{ row }">
                  <span>{{ orderKindText(asExecuteRow(row)) }}</span>
                  <span class="type-divider"> / </span>
                  <span class="therapy-text" :class="therapyClass(asExecuteRow(row))">
                    {{ row.therapyDisplay || '—' }}
                  </span>
                </template>
              </el-table-column>
              <el-table-column label="医嘱内容" min-width="285">
                <template #default="{ row }">
                  <div class="order-content">
                    <div class="order-content__title">
                      <strong>{{ orderTitle(asExecuteRow(row)) }}</strong>
                      <span>{{ dosageText(asExecuteRow(row)) }}</span>
                    </div>
                    <small v-if="extensionText(asExecuteRow(row))">
                      {{ extensionText(asExecuteRow(row)) }}
                    </small>
                  </div>
                </template>
              </el-table-column>
              <el-table-column label="计划执行时间" min-width="205">
                <template #default="{ row }">
                  <div class="schedule-list">
                    <div
                      v-for="schedule in groupSlotsByDate(asExecuteRow(row).slots)"
                      :key="schedule.date"
                      class="schedule-row"
                    >
                      <span>{{ schedule.date }}</span>
                      <label
                        v-for="slot in schedule.slots"
                        :key="slot.planTime"
                        class="schedule-slot"
                      >
                        <el-checkbox
                          :model-value="isSlotSelected(asExecuteRow(row), slot)"
                          :disabled="!slotSelectionEnabled"
                          @change="toggleSlotSelection(asExecuteRow(row), slot, Boolean($event))"
                        />
                        <span>{{ dayjs(slot.planTime).format('HH:mm') }}</span>
                      </label>
                    </div>
                  </div>
                </template>
              </el-table-column>
              <el-table-column label="绑定项目" min-width="245">
                <template #default="{ row }">
                  <div v-if="row.bindItems.length" class="bind-item-list">
                    <div v-for="item in row.bindItems" :key="item.bindingId" class="bind-item">
                      <span :title="item.displayName">
                        {{ item.displayName }}
                        <small v-if="item.specificationText"
                          >【{{ item.specificationText }}】</small
                        >
                      </span>
                      <el-input-number
                        v-model="item.quantity"
                        :min="1"
                        :step="1"
                        :precision="0"
                        :disabled="!executeActionEnabled"
                        controls-position="right"
                        size="small"
                      />
                      <em>{{ item.unitText || '' }}</em>
                    </div>
                  </div>
                  <span v-else class="muted-text">—</span>
                </template>
              </el-table-column>
              <el-table-column label="开始 / 停止时间" align="center" min-width="150">
                <template #default="{ row }">
                  <div class="start-stop-time">
                    <span>{{ row.authoredTime || '—' }}</span>
                    <span v-if="row.stopTime">{{ row.stopTime }}</span>
                  </div>
                </template>
              </el-table-column>
            </el-table>
            <el-empty v-else :image-size="54" description="当前筛选下暂无执行计划" />
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
  gap: 8px;
  padding: 6px 10px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.order-execute-toolbar__filters,
.order-execute-toolbar__actions,
.select-all-control {
  display: flex;
  align-items: center;
  gap: 8px;
}

.order-execute-toolbar__filters {
  min-width: 0;
  flex: 1;
  flex-wrap: wrap;
}

.order-execute-toolbar__actions {
  flex-wrap: wrap;
  justify-content: flex-end;
  color: var(--nurse-text-secondary, #6b7280);
  font-size: 12px;
}

.therapy-filter :deep(.el-radio) {
  height: auto;
  margin-right: 8px;
}

.deadline-label,
.select-all-control {
  color: var(--nurse-text-regular, #606266);
  font-size: 13px;
}

.deadline-picker {
  width: 188px !important;
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
  color: var(--nurse-text-regular, #606266);
  font-weight: 600;
}

.order-execute-table :deep(td.el-table__cell) {
  padding: 8px 0;
  border-right: 0;
  color: var(--nurse-text-primary, #303133);
}

.order-execute-table :deep(.el-table__inner-wrapper::before) {
  display: none;
}

.type-divider,
.muted-text {
  color: var(--nurse-text-secondary, #6b7280);
}

.therapy-text {
  font-weight: 500;
}

.therapy-text.is-long {
  color: #8a573f;
}

.therapy-text.is-temporary {
  color: #1f6f8d;
}

.order-content__title {
  display: flex;
  min-width: 0;
  align-items: baseline;
  justify-content: space-between;
  gap: 10px;
}

.order-content__title strong {
  min-width: 0;
  overflow: hidden;
  color: var(--nurse-text-primary, #303133);
  font-weight: 500;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.order-content__title span {
  flex-shrink: 0;
  color: var(--nurse-text-regular, #606266);
  font-size: 12px;
}

.order-content small {
  display: block;
  margin-top: 3px;
  padding-top: 2px;
  border-top: 1px dashed #b9c7d8;
  color: #1d5fbf;
  font-size: 12px;
}

.schedule-list {
  display: flex;
  flex-direction: column;
  gap: 4px;
}

.schedule-row,
.schedule-slot {
  display: flex;
  align-items: center;
}

.schedule-row {
  flex-wrap: wrap;
  gap: 7px;
  color: var(--nurse-text-regular, #606266);
}

.schedule-row > span {
  min-width: 40px;
  font-size: 12px;
}

.schedule-slot {
  gap: 3px;
  color: var(--nurse-text-primary, #303133);
  font-size: 13px;
}

.schedule-slot :deep(.el-checkbox) {
  height: auto;
  margin-right: 0;
}

.bind-item-list {
  display: flex;
  flex-direction: column;
  gap: 5px;
}

.bind-item {
  display: grid;
  grid-template-columns: minmax(90px, 1fr) 82px auto;
  align-items: center;
  gap: 5px;
}

.bind-item > span {
  min-width: 0;
  overflow: hidden;
  color: var(--nurse-text-primary, #303133);
  text-overflow: ellipsis;
  white-space: nowrap;
}

.bind-item small {
  color: var(--nurse-text-secondary, #6b7280);
}

.bind-item em {
  color: var(--nurse-text-regular, #606266);
  font-size: 12px;
  font-style: normal;
}

.bind-item :deep(.el-input-number) {
  width: 82px;
}

.start-stop-time {
  display: flex;
  flex-direction: column;
  gap: 3px;
  color: var(--nurse-text-regular, #606266);
  font-size: 12px;
}

.patient-execute-card__body :deep(.el-empty) {
  padding: 16px 0;
}

.order-execute-empty {
  min-height: 0;
  flex: 1;
}

@media (max-width: 1366px) {
  .order-execute-toolbar {
    align-items: flex-start;
  }

  .patient-diagnosis {
    max-width: 160px;
  }

  .deadline-picker {
    width: 174px !important;
  }
}
</style>
