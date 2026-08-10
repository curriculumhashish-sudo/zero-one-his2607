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
  type NurseConsumableItemVO,
  type NurseExecutionItemVO,
  type NursePatientQueueItemVO
} from '@/apis/outpatient/nurse-station'
import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import type { PatientSummary } from '@/components/patient/type'
import { useUserStore } from '@/stores/user'

import { toNursePatientSummary } from './adapter'
import NurseExecutionPanel from './components/NurseExecutionPanel.vue'
import NurseFooterBar from './components/NurseFooterBar.vue'
import NurseRightSidebar from './components/NurseRightSidebar.vue'

defineOptions({
  name: 'OutpatientNurseStation'
})

const userStore = useUserStore()
const today = dayjs().format('YYYY-MM-DD')
const QUEUE_PAGE_SIZE = 10
const queueRows = ref<NursePatientQueueItemVO[]>([])
const queueDate = ref(today)
const queuePageIndex = ref(1)
const queueTotal = ref(0)
const selectedEncounterId = ref('')
const executionItems = ref<NurseExecutionItemVO[]>([])
const consumableItems = ref<NurseConsumableItemVO[]>([])
const selectedExecutionIds = ref<string[]>([])
const queueLoading = ref(false)
const workbenchLoading = ref(false)
const executing = ref(false)
const cancellingItemId = ref('')
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

async function loadPatientQueue() {
  queueLoading.value = true
  try {
    const result = await pageNursePatientQueue({
      pageIndex: queuePageIndex.value,
      pageSize: QUEUE_PAGE_SIZE,
      bookingDate: queueDate.value
    })
    queueRows.value = result.rows ?? []
    queueTotal.value = result.total
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
  const queueRow = queueRows.value.find((item) => item.encounterId === encounterId)
  if (!queueRow?.patientId) {
    ElMessage.error('当前队列记录缺少患者 ID，无法查询执行项目')
    return
  }
  workbenchLoading.value = true
  selectedExecutionIds.value = []
  executionPanelRef.value?.clearSelection()
  try {
    const [items, supplies] = await Promise.all([
      getNurseExecutionItems(queueRow.patientId, encounterId),
      getNurseConsumables(queueRow.patientId, encounterId)
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

function handleQueueDateChange(value: string) {
  // 日期不可为空；清空或重置时恢复今天。
  queueDate.value = value || today
  queuePageIndex.value = 1
  void loadPatientQueue()
}

function handleQueuePageChange(pageIndex: number) {
  if (pageIndex === queuePageIndex.value) return
  queuePageIndex.value = pageIndex
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
    .map((item) => ({ itemId: item.itemId }))
}

async function handleExecuteEntry() {
  if (!selectedEncounterId.value || executing.value || cancellingItemId.value) return
  const commands = selectedExecutionCommands()
  if (commands.length === 0) {
    ElMessage.warning('请选择可执行项目')
    return
  }
  const executorId = String(userStore.getUser?.practitionerId || userStore.getUser?.id || '').trim()
  if (!executorId) {
    ElMessage.error('缺少当前操作人信息，无法执行项目')
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
      idempotencyKey: createIdempotencyKey(),
      executorId,
      executorName: userStore.getUser?.realName || userStore.getUser?.username
    })
    if (result.failedCount === 0) {
      ElMessage.success(`成功执行 ${result.successCount} 个项目`)
    } else if (result.successCount > 0) {
      ElMessage.warning(
        `成功 ${result.successCount} 项，失败 ${result.failedCount} 项：${result.failedItems[0]?.reason}`
      )
    } else {
      ElMessage.error(result.failedItems[0]?.reason ?? '执行失败')
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
      item: { itemId: item.itemId },
      reason,
      operatorId:
        String(userStore.getUser?.practitionerId || userStore.getUser?.id || '').trim() ||
        undefined,
      operatorName: userStore.getUser?.realName || userStore.getUser?.username,
      idempotencyKey: createIdempotencyKey()
    })
    if (result.successCount > 0) {
      ElMessage.success('已取消最近一次执行')
    } else {
      ElMessage.error(result.failedItems[0]?.reason ?? '取消执行失败')
    }
    await loadWorkbench(selectedEncounterId.value)
    await loadPatientQueue()
  } catch {
    ElMessage.error('取消执行失败')
  } finally {
    cancellingItemId.value = ''
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
          :date="queueDate"
          :patients="patients"
          :loading="queueLoading"
          :selected-id="selectedEncounterId"
          :show-department="false"
          :show-keyword="false"
          primary-number-mode="patient-no"
          :pagination="{
            pageIndex: queuePageIndex,
            pageSize: QUEUE_PAGE_SIZE,
            total: queueTotal
          }"
          @single-date-change="handleQueueDateChange"
          @page-change="handleQueuePageChange"
          @refresh="loadPatientQueue"
          @select="handleSelectPatient"
          @view="handleSelectPatient"
        />
      </template>

      <template #main>
        <section class="nurse-workbench">
          <PatientInfoBar :patient="currentPatient" display-mode="outpatient-registration" />
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
          :patient-id="currentPatientId"
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
