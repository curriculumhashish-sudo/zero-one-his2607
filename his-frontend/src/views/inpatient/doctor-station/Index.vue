<script setup lang="ts">
import { computed, onMounted, reactive, ref, watch } from 'vue'
import { DocumentAdd, Refresh, Select, SwitchButton } from '@element-plus/icons-vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  inpatientDoctorStationCapabilities,
  isInpatientDoctorStationRealApiEnabled,
  type InpatientDischargeAdviceVO,
  type InpatientMedicalRecordHistoryItemVO
} from '@/apis/inpatient/doctor-station'
import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import type { ActionToolbarItem, CommonComponentKey } from '@/components/common/type'
import { DoctorStationHeader } from '@/components/clinical'
import {
  ClinicalTcmPrescriptionWorkspace,
  type ClinicalTcmPrescriptionContext,
  type ClinicalTcmPrescriptionWorkspaceExpose
} from '@/components/clinical/tcm-prescription'
import {
  ClinicalOrderEditor,
  type ClinicalOrderContext,
  type ClinicalOrderDraft
} from '@/components/clinical/order'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import type { PatientSummary } from '@/components/patient/type'

import { inpatientAdviceAdapter } from './advice/inpatientAdviceAdapter'
import InpatientDiagnosisEditor from './components/InpatientDiagnosisEditor.vue'
import InpatientDiagnosisHistoryPanel from './components/InpatientDiagnosisHistoryPanel.vue'
import InpatientDischargeAdviceDialog from './components/InpatientDischargeAdviceDialog.vue'
import InpatientMedicalRecordEditor from './components/InpatientMedicalRecordEditor.vue'
import InpatientPatientContextPanel from './components/InpatientPatientContextPanel.vue'
import InpatientMedicalRecordHistoryPanel from './components/InpatientMedicalRecordHistoryPanel.vue'
import { INPATIENT_DOCTOR_STATION_TABS } from './constants'
import { useInpatientDiagnosis } from './composables/useInpatientDiagnosis'
import { useInpatientMedicalRecord } from './composables/useInpatientMedicalRecord'
import {
  useInpatientMedicalOrder,
  type InpatientMedicalOrderAction
} from './composables/useInpatientMedicalOrder'
import { useInpatientMedicalRecordHistory } from './composables/useInpatientMedicalRecordHistory'
import { useInpatientPatientQueue } from './composables/useInpatientPatientQueue'
import type {
  InpatientDiagnosisItem,
  InpatientDoctorStationAction,
  InpatientDoctorStationTab
} from './type'

defineOptions({
  name: 'InpatientDoctorStation'
})

const activeTab = ref<InpatientDoctorStationTab>('medicalRecord')
const leftCollapsed = ref(false)
const rightCollapsed = ref(false)
const tcmWorkspaceRef = ref<ClinicalTcmPrescriptionWorkspaceExpose | null>(null)
const tcmDirty = ref(false)
const tcmLoading = ref(false)
const dischargeDialogVisible = ref(false)
const submittedDischargeEncounterIds = reactive(new Set<string>())

const queue = useInpatientPatientQueue()
const {
  changePatientPage,
  loadPatients,
  patients,
  queueLoading,
  queueQuery,
  queueTotal,
  reconcileDischarge,
  searchPatients,
  selectPatient,
  selectedPatient,
  selectedPatientId,
  selectedQueueItem
} = queue

const medicalRecordHistory = useInpatientMedicalRecordHistory()
const {
  changeHistoryPage,
  clearHistorySelection,
  historyError,
  historyLoading,
  historyQuery,
  historyRows,
  historyTotal,
  loadHistory,
  reloadHistory,
  resetHistory,
  searchHistory,
  selectHistoryRecord,
  selectedRecordId
} = medicalRecordHistory

const inpatientMedicalRecord = useInpatientMedicalRecord()
const {
  applyDocumentType: applyMedicalRecordDocumentType,
  content: medicalRecordContent,
  dirty: medicalRecordDirty,
  documentTypeCode: medicalRecordDocumentTypeCode,
  documentTypes: medicalRecordDocumentTypes,
  error: medicalRecordError,
  initialized: medicalRecordInitialized,
  loadDocumentTypes: loadMedicalRecordDocumentTypes,
  loadMedicalRecord,
  loading: medicalRecordLoading,
  readonly: medicalRecordReadonly,
  record: currentMedicalRecord,
  resetMedicalRecord,
  saveMedicalRecordDraft,
  saving: medicalRecordSaving,
  startNewMedicalRecord,
  title: medicalRecordTitle
} = inpatientMedicalRecord

const inpatientDiagnosis = useInpatientDiagnosis()
const {
  addDiagnosisFromBrief,
  diagnosisBriefs,
  diagnosisBriefsError,
  diagnosisBriefsLoading,
  diagnoses,
  diagnosesDirty,
  diagnosesError,
  diagnosesLoading,
  diagnosesSaving,
  loadDiagnoses,
  loadDiagnosisBriefs,
  resetDiagnoses,
  saveDiagnoses
} = inpatientDiagnosis

const inpatientMedicalOrder = useInpatientMedicalOrder()
const {
  actionAvailability: orderActionAvailability,
  canEditOrder,
  executeOrderAction,
  lastOrderActionResult,
  loadOrderCatalog,
  loadOrders,
  orderMutationLoading,
  orders,
  ordersDirty,
  ordersError,
  ordersLoading,
  ordersUpdatedAt,
  resetOrders,
  saveOrders,
  selectOrders,
  selectedOrderCount,
  selectionDisabledRow
} = inpatientMedicalOrder

const activeTabLabel = computed(
  () => INPATIENT_DOCTOR_STATION_TABS.find((tab) => tab.key === activeTab.value)?.label ?? ''
)

const orderContext = computed<ClinicalOrderContext | null>(() => {
  const patient = selectedQueueItem.value
  if (!patient) return null
  return {
    scope: 'inpatient',
    encounterKey: patient.encounterId,
    patientId: patient.patientId,
    departmentName: patient.departmentName
  }
})

const tcmPrescriptionContext = computed<ClinicalTcmPrescriptionContext | null>(() => {
  const patient = selectedQueueItem.value
  if (!patient) return null
  return {
    scope: 'inpatient',
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    diagnoses: diagnoses.value
      .filter((item) => Boolean(item.id))
      .map((item) => ({ id: item.id!, name: item.description }))
  }
})

const existingDiagnosisDescriptions = computed(() =>
  diagnoses.value.map((diagnosis) => diagnosis.description)
)

const stationHeaderActions = computed<ActionToolbarItem[]>(() => {
  const patient = selectedQueueItem.value
  const actions: ActionToolbarItem[] = [
    {
      key: 'discharge',
      label: '出院',
      type: 'warning',
      icon: SwitchButton,
      disabled:
        !patient ||
        submittedDischargeEncounterIds.has(patient.encounterId) ||
        medicalRecordSaving.value ||
        diagnosesSaving.value ||
        orderMutationLoading.value ||
        tcmLoading.value
    },
    {
      key: 'refresh',
      label: '刷新',
      icon: Refresh,
      loading:
        queueLoading.value ||
        medicalRecordLoading.value ||
        diagnosesLoading.value ||
        ordersLoading.value ||
        tcmLoading.value,
      disabled:
        medicalRecordSaving.value ||
        diagnosesSaving.value ||
        orderMutationLoading.value ||
        tcmLoading.value
    }
  ]

  if (activeTab.value === 'medicalRecord') {
    actions.unshift(
      {
        key: 'newMedicalRecord',
        label: '新建病历',
        icon: DocumentAdd,
        disabled:
          !patient ||
          !inpatientDoctorStationCapabilities.medicalRecordDocumentTypes ||
          medicalRecordLoading.value ||
          medicalRecordSaving.value
      },
      {
        key: 'saveMedicalRecord',
        label: '保存病历',
        type: 'primary',
        icon: Select,
        loading: medicalRecordSaving.value,
        disabled:
          !patient ||
          !medicalRecordInitialized.value ||
          medicalRecordReadonly.value ||
          !medicalRecordDirty.value
      }
    )
  }

  if (activeTab.value === 'diagnosis') {
    actions.unshift({
      key: 'saveDiagnosis',
      label: '保存诊断',
      type: 'primary',
      icon: Select,
      loading: diagnosesSaving.value,
      disabled: !selectedQueueItem.value || !diagnosesDirty.value
    })
  }

  return actions
})

async function handleSaveDiagnosis() {
  const patient = selectedQueueItem.value
  if (!patient) {
    ElMessage.warning('请先选择患者')
    return
  }
  try {
    const changedCount = await saveDiagnoses(patient.encounterId, patient.patientId)
    if (changedCount === false) return
    ElMessage.success(
      changedCount > 0 ? `住院诊断保存成功，共更新 ${changedCount} 条` : '诊断无变化'
    )
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '住院诊断保存失败')
  }
}

async function handleHeaderAction(action: ActionToolbarItem) {
  const key = action.key as InpatientDoctorStationAction
  if (key === 'newMedicalRecord') {
    await handleNewMedicalRecord()
    return
  }
  if (key === 'saveMedicalRecord') {
    await handleSaveMedicalRecord()
    return
  }
  if (key === 'saveDiagnosis') {
    await handleSaveDiagnosis()
    return
  }
  if (key === 'saveOrder') {
    await handleSaveOrders()
    return
  }
  if (key === 'saveTcmPrescription') {
    await tcmWorkspaceRef.value?.save()
    return
  }
  if (key === 'discharge') {
    handleOpenDischargeAdvice()
    return
  }
  if (key === 'refresh') {
    await handleRefreshPatients()
  }
}

function handleDiagnosisChange(items: InpatientDiagnosisItem[]) {
  diagnoses.value = items
}

function handleOrderChange(items: ClinicalOrderDraft[]) {
  orders.value = items
}

async function confirmDiscardMedicalRecordChanges() {
  if (!medicalRecordDirty.value) return true
  try {
    await ElMessageBox.confirm(
      '当前病历有未保存修改，继续操作将放弃这些修改。',
      '放弃未保存病历？',
      {
        type: 'warning',
        confirmButtonText: '放弃修改',
        cancelButtonText: '继续编辑'
      }
    )
    return true
  } catch {
    return false
  }
}

async function handleNewMedicalRecord() {
  if (!inpatientDoctorStationCapabilities.medicalRecordDocumentTypes) {
    ElMessage.warning('后端暂未提供病历文书类型接口，Real 模式下无法新建病历')
    return
  }
  if (!selectedQueueItem.value) {
    ElMessage.warning('请先选择患者')
    return
  }
  if (!(await confirmDiscardMedicalRecordChanges())) return
  clearHistorySelection()
  startNewMedicalRecord()
}

async function handleSelectMedicalRecord(record: InpatientMedicalRecordHistoryItemVO) {
  if (record.id === currentMedicalRecord.value?.id && medicalRecordInitialized.value) return
  if (!(await confirmDiscardMedicalRecordChanges())) return
  await Promise.all([selectHistoryRecord(record), loadMedicalRecord(record.id)])
}

async function loadSelectedHistoryMedicalRecord() {
  const record = historyRows.value.find((item) => item.id === selectedRecordId.value)
  if (record) await loadMedicalRecord(record.id)
  else resetMedicalRecord()
}

async function handleSearchMedicalRecordHistory(keyword: string) {
  if (!(await confirmDiscardMedicalRecordChanges())) return
  await searchHistory(keyword)
  await loadSelectedHistoryMedicalRecord()
}

async function handleMedicalRecordHistoryPageChange(pageIndex: number) {
  if (!(await confirmDiscardMedicalRecordChanges())) return
  await changeHistoryPage(pageIndex)
  await loadSelectedHistoryMedicalRecord()
}

async function handleReloadMedicalRecordHistory() {
  if (!(await confirmDiscardMedicalRecordChanges())) return
  await reloadHistory()
  await loadSelectedHistoryMedicalRecord()
}

async function handleSaveMedicalRecord() {
  const patient = selectedQueueItem.value
  if (!patient) {
    ElMessage.warning('请先选择患者')
    return
  }
  try {
    const saved = await saveMedicalRecordDraft(patient)
    if (!saved) return
    await reloadHistory()
    ElMessage.success('住院病历草稿保存成功')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '住院病历保存失败')
  }
}

function handleRetryMedicalRecord() {
  const recordId = currentMedicalRecord.value?.id ?? selectedRecordId.value
  if (recordId) void loadMedicalRecord(recordId)
}

function dirtyClinicalModuleNames() {
  return [
    medicalRecordDirty.value ? '病历' : '',
    diagnosesDirty.value ? '诊断' : '',
    ordersDirty.value ? '医嘱' : '',
    tcmDirty.value ? '中医处方' : ''
  ].filter(Boolean)
}

function handleOpenDischargeAdvice() {
  const patient = selectedQueueItem.value
  if (!patient) {
    ElMessage.warning('请先选择患者')
    return
  }
  if (submittedDischargeEncounterIds.has(patient.encounterId)) {
    ElMessage.warning('该患者的出院医嘱已提交，请勿重复提交')
    return
  }
  const dirtyModules = dirtyClinicalModuleNames()
  if (dirtyModules.length > 0) {
    if (medicalRecordDirty.value) activeTab.value = 'medicalRecord'
    else if (diagnosesDirty.value) activeTab.value = 'diagnosis'
    else if (ordersDirty.value) activeTab.value = 'orders'
    else if (tcmDirty.value) activeTab.value = 'tcmPrescription'
    ElMessage.warning(`请先保存当前患者的${dirtyModules.join('、')}修改，再开立出院医嘱`)
    return
  }
  dischargeDialogVisible.value = true
}

async function handleDischargeAdviceSuccess(advice: InpatientDischargeAdviceVO) {
  submittedDischargeEncounterIds.add(advice.encounterId)
  const reconcileStatus = await reconcileDischarge(advice.encounterId)
  if (reconcileStatus === 'removed') {
    ElMessage.success('出院医嘱已开立，患者已移出在院队列')
  } else if (reconcileStatus === 'still-present') {
    ElMessage.warning('医嘱已提交，但队列状态未同步，请勿重复提交并联系管理员')
  } else {
    ElMessage.warning('医嘱已提交，队列刷新失败，请手动刷新确认，勿重复提交')
  }
}

async function handleSaveOrders() {
  const context = orderContext.value
  if (!context) {
    ElMessage.warning('请先选择患者')
    return
  }

  try {
    const changedCount = await saveOrders(context)
    if (changedCount === false) return
    ElMessage.success(
      changedCount > 0 ? `住院医嘱保存成功，共更新 ${changedCount} 条` : '医嘱无变化'
    )
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '住院医嘱保存失败')
  }
}

const orderActionText: Record<InpatientMedicalOrderAction, string> = {
  delete: '删除',
  group: '并组',
  ungroup: '拆组',
  sign: '签发',
  withdraw: '撤回',
  stop: '停嘱',
  cancelStop: '取消停嘱'
}

async function handleOrderAction(action: InpatientMedicalOrderAction) {
  const context = orderContext.value
  if (!context) {
    ElMessage.warning('请先选择患者')
    return
  }

  const actionText = orderActionText[action]
  try {
    let stopReason: number | undefined
    if (action === 'stop' && isInpatientDoctorStationRealApiEnabled) {
      const promptResult = await ElMessageBox.prompt('请输入停嘱原因码（整数）', '停嘱医嘱', {
        inputPattern: /^\d+$/,
        inputErrorMessage: '停嘱原因码必须是整数',
        confirmButtonText: '确定停嘱',
        cancelButtonText: '取消'
      })
      stopReason = Number(promptResult.value)
    } else {
      await ElMessageBox.confirm(
        `确定要${actionText}已选择的 ${selectedOrderCount.value} 条医嘱吗？`,
        `${actionText}医嘱`,
        {
          type: action === 'delete' || action === 'stop' ? 'warning' : 'info',
          confirmButtonText: `确定${actionText}`,
          cancelButtonText: '取消'
        }
      )
    }
    const changedCount = await executeOrderAction(action, context, { stopReason })
    if ((lastOrderActionResult.value?.failedCount ?? 0) > 0) {
      ElMessage.warning(`部分医嘱${actionText}失败，请查看后端返回的失败原因`)
    }
    ElMessage.success(`${actionText}成功，共处理 ${changedCount} 条医嘱`)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : `${actionText}医嘱失败`)
  }
}

async function confirmDiscardClinicalChanges() {
  const dirtyModules = dirtyClinicalModuleNames()
  if (dirtyModules.length === 0) return true

  try {
    await ElMessageBox.confirm(
      `当前患者有未保存的${dirtyModules.join('和')}修改，继续操作将放弃这些修改。`,
      '放弃未保存修改？',
      {
        type: 'warning',
        confirmButtonText: '放弃修改',
        cancelButtonText: '继续编辑'
      }
    )
    return true
  } catch {
    return false
  }
}

async function handleSelectPatient(patient: PatientSummary) {
  if (patient.id === selectedPatientId.value) return
  if (!(await confirmDiscardClinicalChanges())) return
  selectPatient(patient)
}

async function handleSearchPatients(keyword: string) {
  if (!(await confirmDiscardClinicalChanges())) return
  await searchPatients(keyword)
}

async function handlePatientPageChange(pageIndex: number) {
  if (!(await confirmDiscardClinicalChanges())) return
  await changePatientPage(pageIndex)
}

async function handleRefreshPatients() {
  if (!(await confirmDiscardClinicalChanges())) return
  await loadPatients()
  if (tcmPrescriptionContext.value) {
    await tcmWorkspaceRef.value?.reload()
  }
}

function handleHeaderTabChange(tab: CommonComponentKey) {
  if (INPATIENT_DOCTOR_STATION_TABS.some((item) => item.key === tab)) {
    activeTab.value = tab as InpatientDoctorStationTab
  }
}

watch(
  selectedQueueItem,
  (patient) => {
    if (!patient) {
      resetHistory()
      resetMedicalRecord()
      resetDiagnoses()
      resetOrders()
      return
    }

    const encounterId = patient.encounterId
    void Promise.all([
      (async () => {
        await Promise.all([
          inpatientDoctorStationCapabilities.medicalRecordDocumentTypes
            ? loadMedicalRecordDocumentTypes().catch((error) => {
                ElMessage.error(error instanceof Error ? error.message : '病历文书类型加载失败')
              })
            : Promise.resolve([]),
          loadHistory(patient.patientId, encounterId)
        ])
        if (selectedQueueItem.value?.encounterId !== encounterId) return
        const firstRecord = historyRows.value[0]
        if (firstRecord) await loadMedicalRecord(firstRecord.id)
        else resetMedicalRecord()
      })(),
      loadDiagnoses(encounterId, patient.patientId),
      loadDiagnosisBriefs(),
      loadOrders(encounterId, patient.patientId)
    ])
  },
  { immediate: true }
)

onMounted(() => {
  void loadPatients()
})
</script>

<template>
  <div class="inpatient-doctor-station">
    <StationTriPaneLayout
      v-model:left-collapsed="leftCollapsed"
      v-model:right-collapsed="rightCollapsed"
      left-width="320px"
      right-width="300px"
      :gap="8"
    >
      <template #left>
        <PatientQueuePanel
          :patients="patients"
          :loading="queueLoading"
          :selected-id="selectedPatientId"
          :pagination="{
            pageIndex: queueQuery.pageIndex,
            pageSize: queueQuery.pageSize,
            total: queueTotal
          }"
          title="在院患者"
          keyword-placeholder="姓名/住院号/床号"
          :show-date-filter="false"
          @search="handleSearchPatients"
          @page-change="handlePatientPageChange"
          @refresh="handleRefreshPatients"
          @select="handleSelectPatient"
          @view="handleSelectPatient"
        />
      </template>

      <template #main>
        <section class="inpatient-doctor-station__main">
          <PatientInfoBar :patient="selectedPatient" display-mode="inpatient-registration" />

          <DoctorStationHeader
            :active-tab="activeTab"
            :tabs="INPATIENT_DOCTOR_STATION_TABS"
            :actions="stationHeaderActions"
            :max-visible="activeTab === 'medicalRecord' ? 5 : 4"
            @action="handleHeaderAction"
            @tab-change="handleHeaderTabChange"
          />

          <div class="inpatient-doctor-station__workspace">
            <template v-if="activeTab === 'medicalRecord'">
              <div class="inpatient-doctor-station__medical-record">
                <el-alert
                  v-if="!inpatientDoctorStationCapabilities.medicalRecordDocumentTypes"
                  class="inpatient-doctor-station__capability-alert"
                  title="Real 模式暂仅支持查看已有病历"
                  type="warning"
                  :closable="false"
                  show-icon
                />
                <InpatientMedicalRecordEditor
                  :patient-selected="Boolean(selectedPatient)"
                  :initialized="medicalRecordInitialized"
                  :record="currentMedicalRecord"
                  :document-types="medicalRecordDocumentTypes"
                  :document-type-code="medicalRecordDocumentTypeCode"
                  :title="medicalRecordTitle"
                  :content="medicalRecordContent"
                  :loading="medicalRecordLoading || historyLoading"
                  :error="medicalRecordError || historyError"
                  :dirty="medicalRecordDirty"
                  :readonly="medicalRecordReadonly"
                  @update:document-type-code="medicalRecordDocumentTypeCode = $event"
                  @update:title="medicalRecordTitle = $event"
                  @update:content="medicalRecordContent = $event"
                  @document-type-change="applyMedicalRecordDocumentType"
                  @retry="handleRetryMedicalRecord"
                />
              </div>
            </template>

            <InpatientDiagnosisEditor
              v-else-if="activeTab === 'diagnosis'"
              :model-value="diagnoses"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="Boolean(selectedQueueItem)"
              :loading="diagnosesLoading"
              :saving="diagnosesSaving"
              :error="diagnosesError"
              :dirty="diagnosesDirty"
              @update:model-value="handleDiagnosisChange"
              @retry="
                selectedQueueItem &&
                loadDiagnoses(selectedQueueItem.encounterId, selectedQueueItem.patientId)
              "
            />

            <div v-else-if="activeTab === 'orders'" class="inpatient-doctor-station__orders">
              <el-alert
                v-if="!inpatientDoctorStationCapabilities.medicalOrderCatalog"
                title="后端暂未提供医嘱目录接口，Real 模式下仅支持维护已有医嘱"
                type="warning"
                :closable="false"
                show-icon
              />
              <ClinicalOrderEditor
                :model-value="orders"
                :context="orderContext"
                :adapter="inpatientAdviceAdapter"
                :load-catalog="
                  inpatientDoctorStationCapabilities.medicalOrderCatalog
                    ? loadOrderCatalog
                    : undefined
                "
                :patient-selected="Boolean(selectedPatient)"
                :accepted="Boolean(selectedQueueItem)"
                :loading="ordersLoading"
                :error="ordersError"
                :dirty="ordersDirty"
                :updated-at="ordersUpdatedAt"
                :show-filters="false"
                :show-stop-actions="true"
                :show-delete-action="false"
                :show-group-actions="false"
                :show-history-action="false"
                :show-create-action="false"
                :show-quantity-column="false"
                :show-edit-fields="false"
                :stop-disabled="!orderActionAvailability.stop"
                :cancel-stop-disabled="!orderActionAvailability.cancelStop"
                :selectable="true"
                :can-edit-order="canEditOrder"
                :selection-disabled-row="selectionDisabledRow"
                inactive-title="当前患者不可开立医嘱"
                inactive-description="仅在患者住院诊疗阶段允许维护医嘱"
                readonly-text="住院医嘱只读"
                @update:model-value="handleOrderChange"
                @selection-change="selectOrders"
                @sign="handleOrderAction('sign')"
                @withdraw="handleOrderAction('withdraw')"
                @stop="handleOrderAction('stop')"
                @cancel-stop="handleOrderAction('cancelStop')"
                @retry="
                  selectedQueueItem &&
                  loadOrders(selectedQueueItem.encounterId, selectedQueueItem.patientId)
                "
                @catalog-error="ElMessage.error('医嘱项目目录加载失败，请重试')"
              />
            </div>

            <EmptyState
              v-else-if="activeTab !== 'tcmPrescription'"
              :title="selectedPatient ? `${activeTabLabel}工作区` : '请先选择患者'"
              :description="
                selectedPatient
                  ? '患者队列和住院上下文已就绪，下一阶段接入具体业务功能'
                  : '从左侧在院患者列表选择患者后开始诊疗'
              "
            />

            <ClinicalTcmPrescriptionWorkspace
              v-show="activeTab === 'tcmPrescription'"
              ref="tcmWorkspaceRef"
              :context="tcmPrescriptionContext"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="Boolean(selectedQueueItem)"
              @dirty-change="tcmDirty = $event"
              @loading-change="tcmLoading = $event"
            />
          </div>
        </section>
      </template>

      <template #right>
        <InpatientMedicalRecordHistoryPanel
          v-if="activeTab === 'medicalRecord'"
          :rows="historyRows"
          :selected-id="selectedRecordId"
          :loading="historyLoading"
          :error="historyError"
          :keyword="historyQuery.keyword ?? ''"
          :page-index="historyQuery.pageIndex"
          :page-size="historyQuery.pageSize"
          :total="historyTotal"
          @update:keyword="historyQuery.keyword = $event"
          @search="handleSearchMedicalRecordHistory"
          @select="handleSelectMedicalRecord"
          @page-change="handleMedicalRecordHistoryPageChange"
          @retry="handleReloadMedicalRecordHistory"
        />
        <InpatientDiagnosisHistoryPanel
          v-else-if="activeTab === 'diagnosis'"
          :rows="diagnosisBriefs"
          :existing-descriptions="existingDiagnosisDescriptions"
          :loading="diagnosisBriefsLoading"
          :error="diagnosisBriefsError"
          :readonly="!selectedQueueItem"
          @add="addDiagnosisFromBrief"
          @retry="loadDiagnosisBriefs()"
        />
        <InpatientPatientContextPanel v-else :patient="selectedQueueItem" />
      </template>
    </StationTriPaneLayout>

    <InpatientDischargeAdviceDialog
      v-model="dischargeDialogVisible"
      :patient="selectedQueueItem"
      @success="handleDischargeAdviceSuccess"
    />
  </div>
</template>

<style scoped>
.inpatient-doctor-station {
  box-sizing: border-box;
  width: 100%;
  height: calc(100% - var(--el-tabs-header-height, 40px));
  min-width: 0;
  min-height: 0;
  padding: 8px;
  overflow: hidden;
  background: var(--el-fill-color-lighter);
}

.inpatient-doctor-station__main {
  display: flex;
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  gap: 8px;
  padding: 8px;
  overflow: hidden;
}

.inpatient-doctor-station__workspace {
  display: flex;
  min-height: 0;
  flex: 1;
  /* 内容超过可视高度时必须从顶部开始，避免居中布局产生负偏移并裁掉标题。 */
  align-items: stretch;
  justify-content: center;
  overflow: auto;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.inpatient-doctor-station__orders {
  display: flex;
  width: 100%;
  min-width: 0;
  min-height: 100%;
  flex-direction: column;
  align-self: stretch;
}

.inpatient-doctor-station__medical-record {
  display: flex;
  width: 100%;
  min-width: 0;
  min-height: 100%;
  flex-direction: column;
  align-self: stretch;
}

.inpatient-doctor-station__medical-record :deep(.clinical-medical-record-editor) {
  min-height: 0;
  flex: 1;
}

.inpatient-doctor-station__capability-alert {
  padding: 6px 10px;
}

.inpatient-doctor-station__capability-alert :deep(.el-alert__icon) {
  width: 14px;
  font-size: 14px;
}

.inpatient-doctor-station__capability-alert :deep(.el-alert__title) {
  font-size: 12px;
  line-height: 18px;
}

.inpatient-doctor-station__orders :deep(.clinical-order-editor) {
  min-height: 0;
  flex: 1;
}

.inpatient-doctor-station :deep(.patient-queue-panel) {
  border: 0;
}
</style>
