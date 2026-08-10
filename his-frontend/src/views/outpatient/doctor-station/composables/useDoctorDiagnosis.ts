import { computed, onBeforeUnmount, ref, type ComputedRef, type Ref } from 'vue'
import { ElMessage } from 'element-plus'

import {
  createDiagnosis,
  deleteDiagnosis,
  getDiagnoses,
  pageDiagnosisDictionary,
  type DoctorDiagnosisListVO
} from '@/apis/outpatient/doctor-station'
import {
  hasValidPrimaryDiagnosis,
  type ClinicalDiagnosisDictionaryLoader,
  type ClinicalDiagnosisItem
} from '@/components/clinical/diagnosis'

import {
  toOpdoctorCreateDiagnosis,
  toClinicalDiagnosis,
  toClinicalDiagnosisCandidate,
  toSaveDoctorDiagnosisItem
} from '../adapter'

interface DoctorDiagnosisOptions {
  selectedPatientId: Ref<string | number | null>
  patientAccepted: ComputedRef<boolean>
}

/** 生成稳定业务快照；临时行 key 不参与“是否修改”的判断。 */
function serializeDiagnoses(items: ClinicalDiagnosisItem[]) {
  return JSON.stringify(
    items.map((item) => ({
      recordId: item.recordId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      typeName: item.typeName,
      primary: item.primary
    }))
  )
}

/** 管理门诊诊断加载、字典查询、脏状态和保存。 */
export function useDoctorDiagnosis(options: DoctorDiagnosisOptions) {
  const { selectedPatientId, patientAccepted } = options

  const diagnosisList = ref<DoctorDiagnosisListVO | null>(null)
  const diagnoses = ref<ClinicalDiagnosisItem[]>([])
  const savedDiagnosisSnapshot = ref('[]')
  const diagnosesLoading = ref(false)
  const diagnosesSaving = ref(false)
  const diagnosesError = ref('')

  const diagnosesDirty = computed(
    () =>
      diagnosisList.value !== null &&
      !diagnosesLoading.value &&
      serializeDiagnoses(diagnoses.value) !== savedDiagnosisSnapshot.value
  )
  const diagnosesValid = computed(() => hasValidPrimaryDiagnosis(diagnoses.value))

  let latestDiagnosesRequestId = 0
  let autoSaveTimer: ReturnType<typeof setTimeout> | undefined

  function cancelDiagnosisAutoSave() {
    if (autoSaveTimer) clearTimeout(autoSaveTimer)
    autoSaveTimer = undefined
  }

  async function loadCurrentDiagnoses(encounterId: string | number) {
    cancelDiagnosisAutoSave()
    const requestId = ++latestDiagnosesRequestId
    diagnosesLoading.value = true
    diagnosesError.value = ''
    diagnosisList.value = null
    diagnoses.value = []
    savedDiagnosisSnapshot.value = serializeDiagnoses([])

    try {
      const result = await getDiagnoses(String(encounterId))
      if (requestId !== latestDiagnosesRequestId) return

      const clinicalRows = result.rows
        .slice()
        .sort((left, right) => left.orderNo - right.orderNo)
        .map(toClinicalDiagnosis)
      diagnosisList.value = result
      diagnoses.value = clinicalRows
      savedDiagnosisSnapshot.value = serializeDiagnoses(clinicalRows)
    } catch (error) {
      if (requestId !== latestDiagnosesRequestId) return
      diagnosesError.value = error instanceof Error ? error.message : '门诊诊断加载失败'
    } finally {
      if (requestId === latestDiagnosesRequestId) diagnosesLoading.value = false
    }
  }

  function resetDiagnoses() {
    cancelDiagnosisAutoSave()
    latestDiagnosesRequestId++
    diagnosisList.value = null
    diagnoses.value = []
    savedDiagnosisSnapshot.value = serializeDiagnoses([])
    diagnosesError.value = ''
    diagnosesLoading.value = false
  }

  const loadDiagnosisDictionary: ClinicalDiagnosisDictionaryLoader = async (query, context) => {
    if (context.signal.aborted) return { rows: [], total: 0 }

    const result = await pageDiagnosisDictionary({
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      keyword: query.keyword || undefined
    })
    if (context.signal.aborted) return { rows: [], total: 0 }

    return {
      rows: (result.rows ?? []).map(toClinicalDiagnosisCandidate),
      total: result.total
    }
  }

  async function saveCurrentDiagnoses(notifySuccess = true): Promise<boolean> {
    const encounterId = selectedPatientId.value
    if (!encounterId || !patientAccepted.value || !diagnosisList.value || diagnosesSaving.value) {
      return false
    }

    if (!diagnosesValid.value) {
      ElMessage.warning('请至少添加一条诊断，并且只能设置一个主诊断')
      return false
    }

    const encounterIdText = String(encounterId)
    diagnosesSaving.value = true

    try {
      // 增量提交：对比已保存基线，移除的记录逐个删除，无记录 ID 的新行逐个新增。
      const savedRows = diagnosisList.value.rows
      const currentRecordIds = new Set(
        diagnoses.value
          .map((item) => item.recordId)
          .filter((id): id is string => Boolean(id))
      )
      const deletedIds = savedRows
        .map((row) => row.id)
        .filter((id) => !currentRecordIds.has(id))

      for (const id of deletedIds) {
        await deleteDiagnosis(id)
        if (String(selectedPatientId.value) !== encounterIdText) return false
      }

      for (const [index, diagnosis] of diagnoses.value.entries()) {
        if (diagnosis.recordId) continue
        await createDiagnosis(
          toOpdoctorCreateDiagnosis(encounterIdText, toSaveDoctorDiagnosisItem(diagnosis, index))
        )
        if (String(selectedPatientId.value) !== encounterIdText) return false
      }

      // 保存成功后重新拉取列表，同步后端生成的记录 ID、主诊断和顺序。
      const result = await getDiagnoses(encounterIdText)
      if (String(selectedPatientId.value) !== encounterIdText) return false

      const clinicalRows = result.rows
        .slice()
        .sort((left, right) => left.orderNo - right.orderNo)
        .map(toClinicalDiagnosis)
      diagnosisList.value = result
      diagnoses.value = clinicalRows
      savedDiagnosisSnapshot.value = serializeDiagnoses(clinicalRows)
      if (notifySuccess) ElMessage.success('门诊诊断保存成功')
      return true
    } catch (error) {
      const message = error instanceof Error ? error.message : '门诊诊断保存失败'
      ElMessage.error(message)
      // 失败后重新加载列表，保持本地与后端一致。
      await loadCurrentDiagnoses(encounterIdText)
      return false
    } finally {
      diagnosesSaving.value = false
    }
  }

  /**
   * 诊断没有独立保存按钮；编辑停止片刻且满足唯一主诊断规则后自动提交。
   * 捕获就诊 ID，避免患者切换后旧定时器误保存新患者数据。
   */
  function scheduleDiagnosisAutoSave(delay = 600) {
    cancelDiagnosisAutoSave()
    const encounterId = selectedPatientId.value
    if (!encounterId || !patientAccepted.value) return
    const encounterIdText = String(encounterId)
    autoSaveTimer = setTimeout(() => {
      autoSaveTimer = undefined
      if (
        String(selectedPatientId.value) !== encounterIdText ||
        !diagnosesDirty.value ||
        !diagnosesValid.value
      ) {
        return
      }
      void saveCurrentDiagnoses(false)
    }, delay)
  }

  onBeforeUnmount(cancelDiagnosisAutoSave)

  return {
    diagnosisList,
    diagnoses,
    diagnosesLoading,
    diagnosesSaving,
    diagnosesError,
    diagnosesDirty,
    diagnosesValid,
    loadCurrentDiagnoses,
    resetDiagnoses,
    loadDiagnosisDictionary,
    saveCurrentDiagnoses,
    scheduleDiagnosisAutoSave,
    cancelDiagnosisAutoSave
  }
}
