import { computed, nextTick, onBeforeUnmount, onMounted, ref, watch, type Ref } from 'vue'
import {
  CircleCheck,
  Clock,
  Document,
  Money,
  OfficeBuilding,
  Select
} from '@element-plus/icons-vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import { completePatient, leavePatient } from '@/apis/outpatient/doctor-station'
import type { ActionToolbarItem, CommonComponentKey } from '@/components/common/type'
import type { PatientSummary } from '@/components/patient/type'

import type { DoctorStationAction, DoctorStationTab } from '../type'
import type { useDoctorAdviceOrders } from './useDoctorAdviceOrders'
import type { useDoctorDiagnosis } from './useDoctorDiagnosis'
import type { useDoctorMedicalRecord } from './useDoctorMedicalRecord'
import type { useDoctorPatientContext } from './useDoctorPatientContext'
import type { useDoctorPatientQueue } from './useDoctorPatientQueue'

interface DoctorStationWorkflowOptions {
  activeTab: Ref<DoctorStationTab>
  queue: ReturnType<typeof useDoctorPatientQueue>
  context: ReturnType<typeof useDoctorPatientContext>
  medicalRecord: ReturnType<typeof useDoctorMedicalRecord>
  diagnosis: ReturnType<typeof useDoctorDiagnosis>
  adviceOrders: ReturnType<typeof useDoctorAdviceOrders>
  tcmPrescription: {
    dirty: Ref<boolean>
    loading: Ref<boolean>
    reload: () => Promise<void>
  }
}

/**
 * 编排跨模块业务流程。
 *
 * 患者队列、上下文和四个临床模块各自管理数据；这里只处理需要同时协调
 * 多个模块的接诊、暂离、完诊、自动保存和未保存保护。
 */
export function useDoctorStationWorkflow(options: DoctorStationWorkflowOptions) {
  const { activeTab, queue, context, medicalRecord, diagnosis, adviceOrders, tcmPrescription } =
    options

  const accepting = ref(false)
  const leaving = ref(false)
  const completing = ref(false)
  const admissionDialogVisible = ref(false)
  const refundDialogVisible = ref(false)
  const prescriptionDialogVisible = ref(false)

  const unsavedSectionNames = computed(() =>
    [
      medicalRecord.medicalRecordDirty.value ? '病历' : '',
      diagnosis.diagnosesDirty.value ? '诊断' : '',
      adviceOrders.ordersDirty.value ? '医嘱' : '',
      tcmPrescription.dirty.value ? '中医处方' : ''
    ].filter(Boolean)
  )

  const stationSaving = computed(
    () =>
      medicalRecord.medicalRecordSaving.value ||
      diagnosis.diagnosesSaving.value ||
      adviceOrders.adviceOrdersSaving.value ||
      tcmPrescription.loading.value
  )
  const stationDataLoading = computed(
    () =>
      context.contextLoading.value ||
      medicalRecord.medicalRecordLoading.value ||
      diagnosis.diagnosesLoading.value ||
      adviceOrders.adviceOrdersLoading.value ||
      tcmPrescription.loading.value
  )
  const patientTransitioning = computed(() => accepting.value || leaving.value || completing.value)
  const selectedVisitStatus = computed(() => queue.selectedPatient.value?.statusCode)

  const stationHeaderActions = computed<ActionToolbarItem[]>(() => {
    const patientSelected = Boolean(queue.selectedPatient.value)
    const patientContextLoaded = Boolean(context.patientContext.value)
    const actions: ActionToolbarItem[] = [
      {
        key: 'accept',
        label: '接诊',
        type: 'primary',
        icon: Select,
        disabled:
          !patientSelected ||
          selectedVisitStatus.value !== 'arrived' ||
          context.contextLoading.value ||
          leaving.value ||
          completing.value,
        loading: accepting.value
      },
      {
        key: 'complete',
        label: '完诊',
        type: 'warning',
        icon: CircleCheck,
        disabled:
          !patientContextLoaded ||
          !context.patientAccepted.value ||
          context.patientReadonly.value ||
          patientTransitioning.value ||
          stationSaving.value ||
          stationDataLoading.value,
        loading: completing.value
      },
      {
        key: 'leave',
        label: '暂离',
        icon: Clock,
        disabled:
          !patientContextLoaded ||
          !context.patientAccepted.value ||
          context.patientReadonly.value ||
          patientTransitioning.value ||
          stationSaving.value ||
          stationDataLoading.value,
        loading: leaving.value
      },
      {
        key: 'refund',
        label: '退费',
        icon: Money,
        disabled:
          !patientSelected ||
          !queue.selectedPatient.value?.patientNo ||
          patientTransitioning.value ||
          stationSaving.value ||
          stationDataLoading.value
      },
      {
        key: 'prescription',
        label: '处方单',
        icon: Document,
        disabled:
          !patientSelected ||
          !queue.selectedPatient.value?.patientNo ||
          patientTransitioning.value ||
          stationSaving.value ||
          stationDataLoading.value
      },
      {
        key: 'admission',
        label: '办理住院',
        type: 'primary',
        icon: OfficeBuilding,
        disabled:
          !patientContextLoaded ||
          !context.patientAccepted.value ||
          context.patientReadonly.value ||
          patientTransitioning.value ||
          stationSaving.value ||
          stationDataLoading.value
      }
    ]

    return actions
  })

  let patientSwitchPrompting = false
  let pendingPatientToSelect: PatientSummary | null = null

  async function selectPatient(patient: PatientSummary) {
    if (patient.id === queue.selectedPatientId.value) return

    pendingPatientToSelect = patient
    if (patientSwitchPrompting) return

    if (queue.selectedPatientId.value === null || unsavedSectionNames.value.length === 0) {
      queue.commitSelectedPatient(patient)
      pendingPatientToSelect = null
      return
    }

    patientSwitchPrompting = true
    try {
      await ElMessageBox.confirm(
        '当前患者的' +
          unsavedSectionNames.value.join('、') +
          '尚未提交。建议先继续编辑并等待自动保存；如果继续切换，这些修改将会丢失。',
        '存在未保存的诊疗数据',
        {
          type: 'warning',
          confirmButtonText: '放弃修改并切换',
          cancelButtonText: '继续编辑',
          distinguishCancelAndClose: true
        }
      )

      if (pendingPatientToSelect) queue.commitSelectedPatient(pendingPatientToSelect)
    } catch {
      // 取消或关闭弹窗时保留当前患者和草稿。
    } finally {
      patientSwitchPrompting = false
      pendingPatientToSelect = null
    }
  }

  async function loadSelectedPatientData(encounterId: string) {
    const clinicalDataRequests = [
      medicalRecord.loadCurrentMedicalRecord(encounterId),
      diagnosis.loadCurrentDiagnoses(encounterId),
      adviceOrders.loadCurrentAdviceOrders(encounterId)
    ]
    await nextTick()
    await Promise.all([
      ...clinicalDataRequests,
      tcmPrescription.reload()
    ])
  }

  /**
   * 待诊患者由按钮触发接诊；在诊和完诊患者复用同一入口自动加载上下文。
   */
  async function handleAcceptPatient() {
    const encounterId = queue.selectedPatientId.value
    if (!encounterId || accepting.value || context.contextLoading.value) return

    const encounterIdText = String(encounterId)
    const isWaitingPatient = selectedVisitStatus.value === 'arrived'
    if (isWaitingPatient) accepting.value = true

    try {
      const result = await context.acceptOrLoadPatient(encounterIdText)
      if (!result || String(queue.selectedPatientId.value) !== encounterIdText) return

      await loadSelectedPatientData(encounterIdText)
      if (!isWaitingPatient || String(queue.selectedPatientId.value) !== encounterIdText) return

      if (queue.selectedQueueStatus.value === 'arrived') {
        queue.setQueueStatus('in-progress')
      }
      await queue.loadPatients()
      ElMessage.success('接诊成功')
    } catch (error) {
      if (isWaitingPatient) {
        const message = error instanceof Error ? error.message : '接诊失败'
        ElMessage.error(message)
      }
    } finally {
      if (isWaitingPatient) accepting.value = false
    }
  }

  function focusFirstUnsavedSection() {
    if (medicalRecord.medicalRecordDirty.value) activeTab.value = 'record'
    else if (diagnosis.diagnosesDirty.value) activeTab.value = 'diagnosis'
    else if (adviceOrders.ordersDirty.value) activeTab.value = 'orders'
    else if (tcmPrescription.dirty.value) activeTab.value = 'tcmPrescription'
  }

  /**
   * 办理住院只允许使用已保存的诊疗数据。
   *
   * 按钮不因“有草稿”而直接隐藏，让医生点击后能得到明确提示并自动定位
   * 到需要处理的页签。
   */
  function handleOpenInpatientAdmission() {
    if (
      !queue.selectedPatient.value ||
      !context.patientContext.value ||
      !context.patientAccepted.value ||
      context.patientReadonly.value
    ) {
      ElMessage.warning('请先接诊患者再办理住院')
      return
    }

    if (unsavedSectionNames.value.length > 0) {
      focusFirstUnsavedSection()
      ElMessage.warning(
        '请先处理当前患者的' + unsavedSectionNames.value.join('、') + '待提交内容，再办理住院'
      )
      return
    }

    if (
      !diagnosis.diagnosisList.value ||
      diagnosis.diagnosisList.value.rows.length === 0 ||
      !diagnosis.diagnosesValid.value
    ) {
      activeTab.value = 'diagnosis'
      ElMessage.warning('请先保存诊断，并且只能设置一个主诊断')
      return
    }

    admissionDialogVisible.value = true
  }

  /**
   * 退费按患者病历号查询收费系统，不要求患者处于在诊状态。
   * 该流程不会修改临床草稿，因此也不阻止医生在有未保存内容时查看清单。
   */
  function handleOpenRefundApplication() {
    if (!queue.selectedPatient.value) {
      ElMessage.warning('请先选择患者')
      return
    }
    if (!String(queue.selectedPatient.value.patientNo ?? '').trim()) {
      ElMessage.warning('当前患者缺少病历号，无法办理退费')
      return
    }
    refundDialogVisible.value = true
  }

  /** 处方单是已签发药品医嘱的只读视图，不要求患者处于在诊状态。 */
  function handleOpenPrescriptionSheet() {
    if (!queue.selectedPatient.value) {
      ElMessage.warning('请先选择患者')
      return
    }
    if (!String(queue.selectedPatient.value.patientNo ?? '').trim()) {
      ElMessage.warning('当前患者缺少病历号，无法查看处方单')
      return
    }
    prescriptionDialogVisible.value = true
  }

  async function handleLeavePatient() {
    const encounterId = queue.selectedPatientId.value
    if (
      !encounterId ||
      !context.patientAccepted.value ||
      patientTransitioning.value ||
      stationSaving.value
    ) {
      return
    }

    const encounterIdText = String(encounterId)
    leaving.value = true

    try {
      if (unsavedSectionNames.value.length > 0) {
        try {
          await ElMessageBox.confirm(
            '当前患者的' +
              unsavedSectionNames.value.join('、') +
              '尚未保存。暂离会放弃这些修改，是否继续？',
            '存在未保存的诊疗数据',
            {
              type: 'warning',
              confirmButtonText: '放弃修改并暂离',
              cancelButtonText: '继续编辑'
            }
          )
        } catch {
          return
        }
      }

      const result = await leavePatient(encounterIdText)
      if (String(queue.selectedPatientId.value) !== encounterIdText) return

      if (result.visitStatus === 'arrived') {
        context.resetPatientContext()
        medicalRecord.resetMedicalRecord()
        diagnosis.resetDiagnoses()
        adviceOrders.resetAdviceOrders()
      } else {
        context.patientContext.value = result
      }
      if (queue.selectedQueueStatus.value === 'in-progress') queue.setQueueStatus('arrived')

      await queue.loadPatients()
      ElMessage.success('患者已暂离并返回候诊队列')
    } catch (error) {
      const message = error instanceof Error ? error.message : '暂离失败'
      ElMessage.error(message)
    } finally {
      leaving.value = false
    }
  }

  async function handleCompletePatient() {
    const encounterId = queue.selectedPatientId.value
    if (
      !encounterId ||
      !context.patientAccepted.value ||
      patientTransitioning.value ||
      stationSaving.value
    ) {
      return
    }

    completing.value = true
    try {
      if (unsavedSectionNames.value.length > 0) {
        focusFirstUnsavedSection()
        ElMessage.warning(
          '请先处理当前患者的' + unsavedSectionNames.value.join('、') + '待提交内容，再执行完诊'
        )
        return
      }

      if (
        !medicalRecord.medicalRecord.value ||
        !medicalRecord.medicalRecord.value.id ||
        !medicalRecord.hasMeaningfulMedicalRecordContent()
      ) {
        activeTab.value = 'record'
        ElMessage.warning('请先填写并保存门诊病历')
        return
      }

      if (!diagnosis.diagnosisList.value || !diagnosis.diagnosesValid.value) {
        activeTab.value = 'diagnosis'
        ElMessage.warning('请先保存诊断，并且只能设置一个主诊断')
        return
      }

      try {
        await ElMessageBox.confirm(
          '完诊后病历、诊断、医嘱和中医处方将转为只读，确认完成当前患者的本次就诊吗？',
          '确认完诊',
          {
            type: 'warning',
            confirmButtonText: '确认完诊',
            cancelButtonText: '取消'
          }
        )
      } catch {
        return
      }

      const encounterIdText = String(encounterId)
      const result = await completePatient(encounterIdText)
      if (String(queue.selectedPatientId.value) !== encounterIdText) return

      context.patientContext.value = result
      if (queue.selectedQueueStatus.value === 'in-progress') queue.setQueueStatus('finished')

      await queue.loadPatients()
      ElMessage.success('患者已完诊，诊疗内容已转为只读')
    } catch (error) {
      const message = error instanceof Error ? error.message : '完诊失败'
      ElMessage.error(message)
    } finally {
      completing.value = false
    }
  }

  async function handleSaveMedicalRecord() {
    if (
      !queue.selectedPatientId.value ||
      !context.patientAccepted.value ||
      patientTransitioning.value ||
      medicalRecord.medicalRecordSaving.value ||
      stationDataLoading.value
    ) {
      return
    }

    if (!medicalRecord.medicalRecordDirty.value) {
      ElMessage.info('当前病历已保存')
      return
    }

    const saved = await medicalRecord.saveCurrentMedicalRecord(false)
    if (saved) ElMessage.success('门诊病历保存成功')
  }

  function handleHeaderTabChange(tab: CommonComponentKey) {
    activeTab.value = tab as DoctorStationTab
  }

  function handleHeaderAction(action: ActionToolbarItem) {
    const actionKey = action.key as DoctorStationAction
    if (actionKey === 'accept') void handleAcceptPatient()
    else if (actionKey === 'leave') void handleLeavePatient()
    else if (actionKey === 'admission') handleOpenInpatientAdmission()
    else if (actionKey === 'complete') void handleCompletePatient()
    else if (actionKey === 'prescription') handleOpenPrescriptionSheet()
    else if (actionKey === 'refund') handleOpenRefundApplication()
  }

  function handleBeforeUnload(event: BeforeUnloadEvent) {
    if (unsavedSectionNames.value.length === 0) return
    event.preventDefault()
    event.returnValue = ''
  }

  // 切换患者时先清理旧数据；待诊患者必须点击接诊，在诊和完诊患者自动加载。
  watch(queue.selectedPatientId, (encounterId) => {
    admissionDialogVisible.value = false
    refundDialogVisible.value = false
    prescriptionDialogVisible.value = false

    context.resetPatientContext()
    medicalRecord.resetMedicalRecord()
    diagnosis.resetDiagnoses()
    adviceOrders.resetAdviceOrders()

    if (
      encounterId !== null &&
      (selectedVisitStatus.value === 'in-progress' || selectedVisitStatus.value === 'finished')
    ) {
      void handleAcceptPatient()
    }
  })

  onMounted(() => {
    window.addEventListener('beforeunload', handleBeforeUnload)
    void queue.loadPatients()
  })

  onBeforeUnmount(() => {
    window.removeEventListener('beforeunload', handleBeforeUnload)
  })

  return {
    accepting,
    leaving,
    completing,
    admissionDialogVisible,
    refundDialogVisible,
    prescriptionDialogVisible,
    unsavedSectionNames,
    stationSaving,
    stationDataLoading,
    patientTransitioning,
    stationHeaderActions,
    selectPatient,
    handleAcceptPatient,
    handleOpenInpatientAdmission,
    handleOpenRefundApplication,
    handleOpenPrescriptionSheet,
    handleSaveMedicalRecord,
    handleHeaderTabChange,
    handleHeaderAction
  }
}
