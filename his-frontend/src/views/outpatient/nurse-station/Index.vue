<script setup lang="ts">
import { computed, onMounted, ref } from 'vue'
import dayjs from 'dayjs'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  cancelNurseExecution,
  getNurseConsumables,
  getNurseExecutionItems,
  pageNursePatientQueue,
  performNurseExecutions,
  printNurseDocuments,
  type NurseConsumableItemVO,
  type NurseExecutionItemVO,
  type NursePatientQueueItemVO,
  type NursePrintDocumentType
} from '@/apis/outpatient/nurse-station'
import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
import type { DateRangeValue } from '@/components/common/type'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import type { PatientSummary } from '@/components/patient/type'

import { toNursePatientSummary } from './adapter'
import NurseExecutionPanel from './components/NurseExecutionPanel.vue'
import NurseFooterBar from './components/NurseFooterBar.vue'
import NurseRightSidebar from './components/NurseRightSidebar.vue'
import { NURSE_PRINT_EMPTY_MESSAGES, resolveNursePrintTargetIds } from './print-targets'

defineOptions({
  name: 'OutpatientNurseStation'
})

const today = dayjs().format('YYYY-MM-DD')
const queueRows = ref<NursePatientQueueItemVO[]>([])
const keyword = ref('')
const dateRange = ref<DateRangeValue>([today, today])
const selectedEncounterId = ref('')
const executionItems = ref<NurseExecutionItemVO[]>([])
const consumableItems = ref<NurseConsumableItemVO[]>([])
const selectedExecutionIds = ref<string[]>([])
const queueLoading = ref(false)
const workbenchLoading = ref(false)
const executing = ref(false)
const cancellingItemId = ref('')
const printingType = ref<'' | NursePrintDocumentType>('')
const executionPanelRef = ref<InstanceType<typeof NurseExecutionPanel> | null>(null)
let workbenchRequestId = 0

const patients = computed(() => queueRows.value.map(toNursePatientSummary))

const currentPatient = computed<PatientSummary | null>(() => {
  return patients.value.find((item) => String(item.id) === selectedEncounterId.value) ?? null
})

const currentPatientId = computed(() => {
  if (!selectedEncounterId.value) return ''
  const row = queueRows.value.find((item) => item.encounterId === selectedEncounterId.value)
  return row?.patientId ?? ''
})

const pendingCount = computed(() => {
  return executionItems.value.filter((item) => item.status === 'PENDING').length
})

async function loadPatientQueue() {
  queueLoading.value = true
  try {
    const result = await pageNursePatientQueue({
      pageIndex: 1,
      pageSize: 100,
      keyword: keyword.value,
      startDate: dateRange.value?.[0],
      endDate: dateRange.value?.[1]
    })
    queueRows.value = result.rows ?? []
    if (
      selectedEncounterId.value &&
      !queueRows.value.some((item) => item.encounterId === selectedEncounterId.value)
    ) {
      clearWorkbench()
    }
  } catch {
    ElMessage.error('患者队列加载失败')
  } finally {
    queueLoading.value = false
  }
}

function clearWorkbench() {
  workbenchRequestId += 1
  selectedEncounterId.value = ''
  executionItems.value = []
  consumableItems.value = []
  selectedExecutionIds.value = []
  executionPanelRef.value?.clearSelection()
}

async function loadWorkbench(encounterId: string) {
  const requestId = ++workbenchRequestId
  workbenchLoading.value = true
  selectedExecutionIds.value = []
  executionPanelRef.value?.clearSelection()
  try {
    const [items, supplies] = await Promise.all([
      getNurseExecutionItems(encounterId),
      getNurseConsumables(encounterId)
    ])
    if (requestId !== workbenchRequestId || encounterId !== selectedEncounterId.value) return
    executionItems.value = items
    consumableItems.value = supplies
  } catch {
    if (requestId === workbenchRequestId) {
      ElMessage.error('患者工作台加载失败')
    }
  } finally {
    if (requestId === workbenchRequestId) {
      workbenchLoading.value = false
    }
  }
}

function handleSearch(value: string) {
  keyword.value = value
  void loadPatientQueue()
}

function handleDateChange(value: DateRangeValue) {
  dateRange.value = value
  void loadPatientQueue()
}

function handleSelectPatient(patient: PatientSummary) {
  const encounterId = String(patient.id)
  if (encounterId === selectedEncounterId.value) return
  selectedEncounterId.value = encounterId
  executionItems.value = []
  consumableItems.value = []
  void loadWorkbench(encounterId)
}

function handleRefreshWorkbench() {
  if (!selectedEncounterId.value) return
  void loadWorkbench(selectedEncounterId.value)
}

function clearSelection() {
  selectedExecutionIds.value = []
  executionPanelRef.value?.clearSelection()
}

function createIdempotencyKey() {
  return globalThis.crypto?.randomUUID?.() ?? `nurse-${Date.now()}-${Math.random()}`
}

function selectedExecutionCommands() {
  const selectedIds = new Set(selectedExecutionIds.value)
  return executionItems.value
    .filter((item) => selectedIds.has(item.itemId))
    .map((item) => ({ itemId: item.itemId, version: item.version }))
}

async function handleExecuteEntry() {
  if (!selectedEncounterId.value || executing.value || cancellingItemId.value) return
  const commands = selectedExecutionCommands()
  if (commands.length === 0) {
    ElMessage.warning('请选择可执行项目')
    return
  }

  try {
    await ElMessageBox.confirm(`确认执行已选择的 ${commands.length} 个项目吗？`, '批量执行', {
      type: 'warning',
      confirmButtonText: '确认执行',
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  executing.value = true
  try {
    const result = await performNurseExecutions({
      encounterId: selectedEncounterId.value,
      items: commands,
      idempotencyKey: createIdempotencyKey()
    })
    if (result.failedItems.length === 0) {
      ElMessage.success(`成功执行 ${result.successItems.length} 个项目`)
    } else if (result.successItems.length > 0) {
      ElMessage.warning(
        `成功 ${result.successItems.length} 项，失败 ${result.failedItems.length} 项：${result.failedItems[0]?.message}`
      )
    } else {
      ElMessage.error(result.failedItems[0]?.message ?? '执行失败')
    }
    await loadWorkbench(selectedEncounterId.value)
    await loadPatientQueue()
  } catch {
    ElMessage.error('批量执行失败')
  } finally {
    executing.value = false
  }
}

async function handleCancelExecute(item: NurseExecutionItemVO) {
  if (!selectedEncounterId.value || cancellingItemId.value) return
  let reason = ''
  try {
    const promptResult = await ElMessageBox.prompt('请输入取消原因', '取消执行', {
      type: 'warning',
      inputPlaceholder: '取消原因不能为空',
      inputValidator: (value) => Boolean(value.trim()) || '请输入取消原因',
      confirmButtonText: '确认取消',
      cancelButtonText: '返回'
    })
    reason = promptResult.value.trim()
  } catch {
    return
  }

  cancellingItemId.value = item.itemId
  try {
    const result = await cancelNurseExecution({
      encounterId: selectedEncounterId.value,
      item: { itemId: item.itemId, version: item.version },
      reason
    })
    if (result.successItems.length > 0) {
      ElMessage.success('已取消最近一次执行')
    } else {
      ElMessage.error(result.failedItems[0]?.message ?? '取消执行失败')
    }
    await loadWorkbench(selectedEncounterId.value)
    await loadPatientQueue()
  } catch {
    ElMessage.error('取消执行失败')
  } finally {
    cancellingItemId.value = ''
  }
}

async function handlePrint(documentType: NursePrintDocumentType) {
  if (!selectedEncounterId.value || printingType.value) return
  const documentIds = resolveNursePrintTargetIds(
    executionItems.value,
    documentType,
    selectedExecutionIds.value
  )
  if (documentIds.length === 0) {
    ElMessage.warning(NURSE_PRINT_EMPTY_MESSAGES[documentType])
    return
  }

  printingType.value = documentType
  try {
    const result = await printNurseDocuments({
      encounterId: selectedEncounterId.value,
      documentIds,
      documentType,
      reprint: false
    })
    if (result.success) {
      ElMessage.success(`${result.message}，打印机：${result.printer}`)
    } else {
      ElMessage.warning(result.message)
    }
  } catch {
    ElMessage.error('打印任务提交失败')
  } finally {
    printingType.value = ''
  }
}

onMounted(() => {
  void loadPatientQueue()
})
</script>

<template>
  <div class="outpatient-nurse-station">
    <StationTriPaneLayout :left-width="300" :right-width="310">
      <template #left>
        <PatientQueuePanel
          v-model:date-range="dateRange"
          :patients="patients"
          :loading="queueLoading"
          :selected-id="selectedEncounterId"
          @search="handleSearch"
          @date-change="handleDateChange"
          @refresh="loadPatientQueue"
          @select="handleSelectPatient"
          @view="handleSelectPatient"
        />
      </template>

      <template #main>
        <section class="nurse-workbench">
          <PatientInfoBar :patient="currentPatient" />
          <div class="nurse-workbench__content">
            <NurseExecutionPanel
              ref="executionPanelRef"
              :items="executionItems"
              :loading="workbenchLoading || executing || Boolean(cancellingItemId)"
              :has-patient="Boolean(currentPatient)"
              @cancel-execute="handleCancelExecute"
              @selection-change="selectedExecutionIds = $event"
              @refresh="handleRefreshWorkbench"
            />
          </div>
          <NurseFooterBar
            :selected-count="selectedExecutionIds.length"
            :pending-count="pendingCount"
            :executing="executing || Boolean(cancellingItemId)"
            @clear="clearSelection"
            @execute="handleExecuteEntry"
          />
        </section>
      </template>

      <template #right>
        <NurseRightSidebar
          :execution-items="executionItems"
          :items="consumableItems"
          :loading="workbenchLoading"
          :printing-type="printingType"
          :selected-count="selectedExecutionIds.length"
          :can-print="Boolean(currentPatient) && !executing && !cancellingItemId && !printingType"
          :patient-id="currentPatientId"
          @print="handlePrint"
        />
      </template>
    </StationTriPaneLayout>
  </div>
</template>

<style scoped>
.outpatient-nurse-station {
  box-sizing: border-box;
  height: calc(100% - var(--el-tabs-header-height, 40px));
  max-height: calc(100% - var(--el-tabs-header-height, 40px));
  min-height: 0;
  padding: 8px;
  overflow: hidden;
  background: var(--el-fill-color-extra-light);
}

.nurse-workbench {
  display: flex;
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  padding: 12px;
}

.nurse-workbench__content {
  min-height: 0;
  flex: 1;
  margin-top: 10px;
  overflow: hidden;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 8px;
}
</style>
