<script setup lang="ts">
import { computed, ref } from 'vue'
import { ElMessage } from 'element-plus'

import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
import { DoctorStationHeader } from '@/components/clinical'
import { ClinicalDiagnosisEditor } from '@/components/clinical/diagnosis'
import { ClinicalOrderEditor } from '@/components/clinical/order'
import {
  ClinicalTcmPrescriptionWorkspace,
  type ClinicalTcmPrescriptionContext,
  type ClinicalTcmPrescriptionWorkspaceExpose
} from '@/components/clinical/tcm-prescription'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'

import DoctorStationAssistantPanel from './components/DoctorStationAssistantPanel.vue'
import AdviceOrderHistoryDrawer from './components/AdviceOrderHistoryDrawer.vue'
import DiagnosisHistoryDrawer from './components/DiagnosisHistoryDrawer.vue'
import DoctorQueueStatusFilter from './components/DoctorQueueStatusFilter.vue'
import InpatientAdmissionDialog from './components/InpatientAdmissionDialog.vue'
import MedicalRecordHistoryDialog from './components/MedicalRecordHistoryDialog.vue'
import MedicalRecordPanel from './components/MedicalRecordPanel.vue'
import PrescriptionSheetDialog from './components/PrescriptionSheetDialog.vue'
import RefundApplicationDialog from './components/RefundApplicationDialog.vue'
import {
  useDoctorAdviceHistory,
  useDoctorAdviceOrders,
  useDoctorDiagnosis,
  useDoctorDiagnosisHistory,
  useDoctorMedicalRecord,
  useDoctorMedicalRecordHistory,
  useDoctorPatientContext,
  useDoctorPatientQueue,
  useDoctorStationWorkflow
} from './composables'
import { DOCTOR_STATION_TABS } from './constants'
import type { DoctorStationTab } from './type'

defineOptions({
  name: 'OutpatientDoctorStation'
})

// Index.vue 只保留页面级 UI 状态和 composable 组装，不再直接实现接口请求。
const activeTab = ref<DoctorStationTab>('record')
const leftCollapsed = ref(false)
const rightCollapsed = ref(false)
const tcmWorkspaceRef = ref<ClinicalTcmPrescriptionWorkspaceExpose | null>(null)
const tcmDirty = ref(false)
const tcmLoading = ref(false)

const patientQueueState = useDoctorPatientQueue()
const patientContextState = useDoctorPatientContext()

const medicalRecordState = useDoctorMedicalRecord({
  selectedPatientId: patientQueueState.selectedPatientId,
  patientAccepted: patientContextState.patientAccepted
})

const medicalRecordHistoryState = useDoctorMedicalRecordHistory({
  patientContext: patientContextState.patientContext
})

const diagnosisState = useDoctorDiagnosis({
  selectedPatientId: patientQueueState.selectedPatientId,
  patientAccepted: patientContextState.patientAccepted
})

const diagnosisHistoryState = useDoctorDiagnosisHistory({
  patientContext: patientContextState.patientContext
})

const adviceOrderState = useDoctorAdviceOrders({
  selectedPatientId: patientQueueState.selectedPatientId,
  selectedPatient: patientQueueState.selectedPatient,
  patientContext: patientContextState.patientContext,
  patientAccepted: patientContextState.patientAccepted
})

const adviceHistoryState = useDoctorAdviceHistory({
  patientContext: patientContextState.patientContext
})

/** 公共中医处方组件只依赖通用就诊上下文，门诊侧负责提供场景标识。 */
const tcmPrescriptionContext = computed<ClinicalTcmPrescriptionContext | null>(() => {
  const current = patientContextState.patientContext.value
  if (!current) return null
  return {
    scope: 'outpatient',
    encounterId: current.encounterId,
    patientId: current.patientId,
    diagnoses: diagnosisState.diagnoses.value
      .filter((item) => Boolean(item.recordId))
      .map((item) => ({ id: item.recordId!, name: item.name }))
  }
})

async function reloadTcmPrescription() {
  await tcmWorkspaceRef.value?.reload()
}

const workflow = useDoctorStationWorkflow({
  activeTab,
  queue: patientQueueState,
  context: patientContextState,
  medicalRecord: medicalRecordState,
  diagnosis: diagnosisState,
  adviceOrders: adviceOrderState,
  tcmPrescription: {
    dirty: tcmDirty,
    loading: tcmLoading,
    reload: reloadTcmPrescription
  }
})

// 解构出的成员均为 ref/computed 或函数，模板可以继续使用原来的变量名。
const {
  patients,
  selectedPatientId,
  selectedPatient,
  queueDate,
  queueLoading,
  selectedQueueStatus,
  searchPatients,
  changeQueueDate,
  changeQueueStatus
} = patientQueueState

const { patientContext, contextLoading, contextError, patientAccepted, patientReadonly } =
  patientContextState

const {
  medicalRecord,
  medicalRecordContent,
  medicalRecordLoading,
  medicalRecordSaving,
  medicalRecordError,
  medicalRecordDirty,
  loadCurrentMedicalRecord
} = medicalRecordState

const {
  historyRows,
  historyTotal,
  historyKeyword,
  historyLoading,
  historyError,
  historyQuery,
  historyDialogVisible,
  selectedHistoryItem,
  historyDetail,
  historyDetailLoading,
  historyDetailError,
  setHistoryActive,
  updateHistoryKeyword,
  searchHistory,
  changeHistoryPage,
  loadHistoryList,
  openHistoryDetail,
  closeHistoryDetail,
  retryHistoryDetail
} = medicalRecordHistoryState

const {
  diagnosisList,
  diagnoses,
  diagnosesLoading,
  diagnosesError,
  diagnosesDirty,
  loadCurrentDiagnoses,
  loadDiagnosisDictionary,
  scheduleDiagnosisAutoSave
} = diagnosisState

const {
  diagnosisHistoryVisible,
  patientDiagnosisHistoryRows,
  patientDiagnosisHistoryTotal,
  patientDiagnosisHistoryLoading,
  patientDiagnosisHistoryError,
  patientDiagnosisHistoryQuery,
  loadPatientDiagnosisHistory,
  openDiagnosisHistory,
  closeDiagnosisHistory,
  updatePatientDiagnosisHistoryQuery
} = diagnosisHistoryState

const {
  adviceOrders,
  adviceOrdersLoading,
  adviceOrdersActionLoading,
  adviceOrdersError,
  adviceOrdersTotal,
  adviceOrderQuery,
  adviceOrderContext,
  ordersDirty,
  adviceAdapter: outpatientAdviceAdapter,
  loadCurrentAdviceOrders,
  loadAdviceOrderCatalog,
  updateAdviceOrderQuery,
  scheduleAdviceOrderAutoSave,
  signSelectedAdviceOrders,
  withdrawSelectedAdviceOrders
} = adviceOrderState

const {
  adviceHistoryVisible,
  historyEncounters: adviceHistoryEncounters,
  historyEncounterTotal: adviceHistoryEncounterTotal,
  historyEncounterLoading: adviceHistoryEncounterLoading,
  historyEncounterError: adviceHistoryEncounterError,
  historyEncounterQuery: adviceHistoryEncounterQuery,
  selectedHistoryEncounter: selectedAdviceHistoryEncounter,
  historyAdviceOrders,
  historyAdviceTotal,
  historyAdviceLoading,
  historyAdviceError,
  historyAdviceQuery,
  openAdviceHistory,
  closeAdviceHistory,
  loadHistoryEncounters: loadAdviceHistoryEncounters,
  selectHistoryEncounter: selectAdviceHistoryEncounter,
  updateHistoryEncounterQuery: updateAdviceHistoryEncounterQuery,
  loadHistoryAdviceOrders,
  updateHistoryAdviceQuery
} = adviceHistoryState

const {
  admissionDialogVisible,
  refundDialogVisible,
  prescriptionDialogVisible,
  stationHeaderActions,
  selectPatient,
  handleAcceptPatient,
  handleSaveMedicalRecord,
  handleHeaderTabChange,
  handleHeaderAction
} = workflow
</script>

<template>
  <!-- 页面外壳必须有明确高度，三栏组件才能正确计算独立滚动区域。 -->
  <div class="doctor-station-page">
    <!--
      v-model:left-collapsed 是双向绑定的简写：
      布局组件发出折叠事件时，leftCollapsed 会自动更新。
    -->
    <StationTriPaneLayout
      v-model:left-collapsed="leftCollapsed"
      v-model:right-collapsed="rightCollapsed"
      left-width="320px"
      right-width="300px"
      :gap="8"
    >
      <!-- 左侧插槽：患者查询和患者队列。 -->
      <template #left>
        <section class="doctor-station-page__queue">
          <DoctorQueueStatusFilter
            v-model="selectedQueueStatus"
            :loading="queueLoading"
            @change="changeQueueStatus"
          />
          <PatientQueuePanel
            :patients="patients"
            :loading="queueLoading"
            :selected-id="selectedPatientId"
            :date="queueDate"
            :show-refresh="false"
            :show-department="false"
            primary-number-mode="patient-no"
            keyword-placeholder="患者姓名"
            @search="searchPatients"
            @single-date-change="changeQueueDate"
            @select="selectPatient"
            @view="selectPatient"
          />
        </section>
      </template>

      <!-- 中间插槽：医生最主要的诊疗工作区。 -->
      <template #main>
        <section class="doctor-station-page__main">
          <!-- patient 为 null 时，公共组件会自动显示“未选择患者”。 -->
          <PatientInfoBar
            :patient="selectedPatient"
            display-mode="outpatient-registration"
            :phone="patientContext?.phone"
            :doctor-name="patientContext?.attendingDoctorName"
          />

          <!--
            公共 Header 只渲染配置；门诊页签和按钮规则由工作流 composable 提供。
            将来住院医生站可以复用相同外壳，并传入自己的配置。
          -->
          <DoctorStationHeader
            :active-tab="activeTab"
            :tabs="DOCTOR_STATION_TABS"
            :actions="stationHeaderActions"
            :max-visible="stationHeaderActions.length"
            @action="handleHeaderAction"
            @tab-change="handleHeaderTabChange"
          />

          <!-- 使用 v-show 保留四个编辑器实例，切换页签不会丢失未保存草稿。 -->
          <div class="doctor-station-page__workspace">
            <MedicalRecordPanel
              v-show="activeTab === 'record'"
              v-model="medicalRecordContent"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="patientAccepted"
              :readonly="patientReadonly"
              :loading="medicalRecordLoading"
              :error="medicalRecordError"
              :dirty="medicalRecordDirty"
              :saving="medicalRecordSaving"
              :updated-at="medicalRecord?.updatedAt"
              @save="handleSaveMedicalRecord"
              @retry="selectedPatientId !== null && loadCurrentMedicalRecord(selectedPatientId)"
            />

            <ClinicalDiagnosisEditor
              v-show="activeTab === 'diagnosis'"
              v-model="diagnoses"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="patientAccepted"
              :readonly="patientReadonly"
              :allow-set-primary="false"
              :loading="diagnosesLoading"
              :error="diagnosesError"
              :dirty="diagnosesDirty"
              auto-save
              :updated-at="diagnosisList?.updatedAt"
              :load-dictionary="loadDiagnosisDictionary"
              @retry="selectedPatientId !== null && loadCurrentDiagnoses(selectedPatientId)"
              @change="scheduleDiagnosisAutoSave()"
              @open-history="openDiagnosisHistory"
              @dictionary-error="ElMessage.error('诊断字典加载失败，请重试')"
            />

            <ClinicalOrderEditor
              v-show="activeTab === 'orders'"
              v-model="adviceOrders"
              :context="adviceOrderContext"
              :adapter="outpatientAdviceAdapter"
              :load-catalog="loadAdviceOrderCatalog"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="patientAccepted"
              :readonly="patientReadonly"
              :loading="adviceOrdersLoading"
              :action-loading="adviceOrdersActionLoading"
              :error="adviceOrdersError"
              :dirty="ordersDirty"
              auto-save
              :show-filters="false"
              :keyword="adviceOrderQuery.keyword"
              :category="adviceOrderQuery.category"
              :status="adviceOrderQuery.status"
              :page="adviceOrderQuery.pageIndex"
              :page-size="adviceOrderQuery.pageSize"
              :total="adviceOrdersTotal"
              @retry="selectedPatientId !== null && loadCurrentAdviceOrders(selectedPatientId)"
              @catalog-error="ElMessage.error('医嘱项目加载失败，请重试')"
              @query-change="updateAdviceOrderQuery"
              @change="scheduleAdviceOrderAutoSave()"
              @sign="signSelectedAdviceOrders"
              @withdraw="withdrawSelectedAdviceOrders"
              @open-history="openAdviceHistory"
            />

            <ClinicalTcmPrescriptionWorkspace
              v-show="activeTab === 'tcmPrescription'"
              ref="tcmWorkspaceRef"
              :context="tcmPrescriptionContext"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="patientAccepted"
              :readonly="patientReadonly"
              :show-refresh-action="false"
              @dirty-change="tcmDirty = $event"
              @loading-change="tcmLoading = $event"
            />
          </div>
        </section>
      </template>

      <!-- 右侧插槽：历史病历和就诊信息通过页签共享右栏。 -->
      <template #right>
        <DoctorStationAssistantPanel
          :patient-selected="Boolean(selectedPatient)"
          :patient="selectedPatient"
          :context="patientContext"
          :context-loading="contextLoading"
          :context-error="contextError"
          :history-rows="historyRows"
          :history-keyword="historyKeyword"
          :history-page="historyQuery.pageIndex"
          :history-page-size="historyQuery.pageSize"
          :history-total="historyTotal"
          :history-loading="historyLoading"
          :history-error="historyError"
          @retry-context="handleAcceptPatient"
          @history-active-change="setHistoryActive"
          @update-history-keyword="updateHistoryKeyword"
          @search-history="searchHistory"
          @change-history-page="changeHistoryPage"
          @select-history="openHistoryDetail"
          @retry-history="loadHistoryList"
        />
      </template>
    </StationTriPaneLayout>

    <!--
      办理住院属于门诊就诊的后续业务动作，使用独立弹窗承载。
      提交成功只创建待登记住院申请，不自动改变当前门诊状态。
    -->
    <InpatientAdmissionDialog
      v-model="admissionDialogVisible"
      :patient="selectedPatient"
      :context="patientContext"
      :diagnoses="diagnoses"
      :load-diagnosis-dictionary="loadDiagnosisDictionary"
    />

    <!-- 退费独立查询收费清单，不改变当前病历、诊断和医嘱草稿。 -->
    <RefundApplicationDialog v-model="refundDialogVisible" :patient="selectedPatient" />

    <!-- 处方单仅用于查看本次和历史正式处方，不修改当前医嘱草稿。 -->
    <PrescriptionSheetDialog v-model="prescriptionDialogVisible" :patient="selectedPatient" />

    <!-- 历史病历使用独立只读弹窗，不覆盖中间区域正在编辑的本次病历。 -->
    <MedicalRecordHistoryDialog
      :model-value="historyDialogVisible"
      :item="selectedHistoryItem"
      :record="historyDetail"
      :loading="historyDetailLoading"
      :error="historyDetailError"
      @update:model-value="!$event && closeHistoryDetail()"
      @retry="retryHistoryDetail"
    />

    <!-- 患者历史诊断独立分页查询，只读展示，不会写入或覆盖本次诊断草稿。 -->
    <DiagnosisHistoryDrawer
      :model-value="diagnosisHistoryVisible"
      :rows="patientDiagnosisHistoryRows"
      :total="patientDiagnosisHistoryTotal"
      :loading="patientDiagnosisHistoryLoading"
      :error="patientDiagnosisHistoryError"
      :page="patientDiagnosisHistoryQuery.pageIndex"
      :page-size="patientDiagnosisHistoryQuery.pageSize"
      :keyword="patientDiagnosisHistoryQuery.keyword"
      :date-range="patientDiagnosisHistoryQuery.dateRange"
      @update:model-value="!$event && closeDiagnosisHistory()"
      @query-change="updatePatientDiagnosisHistoryQuery"
      @retry="loadPatientDiagnosisHistory"
    />

    <!-- 历史医嘱只读查看，不修改本次医嘱草稿和未保存状态。 -->
    <AdviceOrderHistoryDrawer
      :model-value="adviceHistoryVisible"
      :encounters="adviceHistoryEncounters"
      :encounter-total="adviceHistoryEncounterTotal"
      :encounter-loading="adviceHistoryEncounterLoading"
      :encounter-error="adviceHistoryEncounterError"
      :encounter-page="adviceHistoryEncounterQuery.pageIndex"
      :encounter-page-size="adviceHistoryEncounterQuery.pageSize"
      :encounter-keyword="adviceHistoryEncounterQuery.keyword"
      :selected-encounter="selectedAdviceHistoryEncounter"
      :orders="historyAdviceOrders"
      :order-total="historyAdviceTotal"
      :order-loading="historyAdviceLoading"
      :order-error="historyAdviceError"
      :order-page="historyAdviceQuery.pageIndex"
      :order-page-size="historyAdviceQuery.pageSize"
      :order-keyword="historyAdviceQuery.keyword"
      @update:model-value="!$event && closeAdviceHistory()"
      @select-encounter="selectAdviceHistoryEncounter"
      @encounter-query-change="updateAdviceHistoryEncounterQuery"
      @order-query-change="updateHistoryAdviceQuery"
      @retry-encounters="loadAdviceHistoryEncounters"
      @retry-orders="loadHistoryAdviceOrders"
    />
  </div>
</template>

<style scoped>
/*
 * 医生站位于 HomeView 的内容区域内。
 * 减去顶部标签栏高度后，剩余空间全部交给三栏工作站。
 */
.doctor-station-page {
  background: var(--el-fill-color-lighter);
  box-sizing: border-box;
  width: 100%;
  height: calc(100% - var(--el-tabs-header-height, 40px));
  min-width: 0;
  min-height: 0;
  padding: 8px;
  overflow: hidden;
}

/* 中间区域纵向排列：患者信息栏、操作区、具体工作区。 */
.doctor-station-page__main {
  display: flex;
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  gap: 8px;
  padding: 8px;
  overflow: hidden;
}

/* 状态筛选固定在顶部，患者队列占满左栏剩余空间并独立滚动。 */
.doctor-station-page__queue {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
}

.doctor-station-page__queue :deep(.patient-queue-panel) {
  min-height: 0;
  flex: 1;
  border: 0;
}

/* flex: 1 让具体业务区占满中间栏剩余高度。 */
.doctor-station-page__workspace {
  display: flex;
  min-height: 0;
  flex: 1;
  overflow: auto;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}
</style>
