import { computed, ref } from 'vue'

import {
  createInpatientDiagnosis,
  deleteInpatientDiagnosis,
  getInpatientDiagnosisUsageHistory,
  pageInpatientDiagnosisDictionary,
  pageInpatientDiagnosisRecords,
  updateInpatientDiagnosis,
  type InpatientDiagnosisRecordVO,
  type InpatientDiagnosisUsageHistoryVO
} from '@/apis/inpatient/doctor-station'
import {
  addClinicalDiagnoses,
  hasValidPrimaryDiagnosis,
  type ClinicalDiagnosisDictionaryLoader,
  type ClinicalDiagnosisItem
} from '@/components/clinical/diagnosis'
import {
  toCreateInpatientDiagnosisDTO,
  toInpatientClinicalDiagnosis,
  toInpatientDiagnosisCandidate,
  toUpdateInpatientDiagnosisDTO
} from '../adapter'

function serializeDiagnoses(items: ClinicalDiagnosisItem[]) {
  return JSON.stringify(
    items.map((item, index) => ({
      recordId: item.recordId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      typeName: item.typeName,
      primary: item.primary,
      orderNo: index + 1
    }))
  )
}

function diagnosisChanged(
  saved: InpatientDiagnosisRecordVO,
  current: ClinicalDiagnosisItem,
  index: number
) {
  return (
    saved.catalogId !== current.catalogId ||
    saved.code !== current.code ||
    saved.name !== current.name ||
    saved.typeName !== current.typeName ||
    saved.primary !== current.primary ||
    saved.orderNo !== index + 1
  )
}

export function useInpatientDiagnosis() {
  const diagnosisRecords = ref<InpatientDiagnosisRecordVO[]>([])
  const diagnoses = ref<ClinicalDiagnosisItem[]>([])
  const savedDiagnosisSnapshot = ref('[]')
  const loadedEncounterId = ref('')
  const diagnosesLoading = ref(false)
  const diagnosesSaving = ref(false)
  const diagnosesError = ref('')

  const usageHistory = ref<InpatientDiagnosisUsageHistoryVO[]>([])
  const usageHistoryLoading = ref(false)
  const usageHistoryError = ref('')
  const usageHistoryKeyword = ref('')

  const diagnosesDirty = computed(
    () =>
      Boolean(loadedEncounterId.value) &&
      !diagnosesLoading.value &&
      serializeDiagnoses(diagnoses.value) !== savedDiagnosisSnapshot.value
  )
  const diagnosisUpdatedAt = computed(
    () =>
      diagnosisRecords.value
        .map((record) => record.updatedAt)
        .sort((left, right) => right.localeCompare(left))[0] ?? ''
  )

  let latestDiagnosisRequestId = 0
  let latestUsageHistoryRequestId = 0

  function resetDiagnoses() {
    latestDiagnosisRequestId += 1
    loadedEncounterId.value = ''
    diagnosisRecords.value = []
    diagnoses.value = []
    savedDiagnosisSnapshot.value = '[]'
    diagnosesLoading.value = false
    diagnosesSaving.value = false
    diagnosesError.value = ''
  }

  async function loadDiagnoses(encounterId: string) {
    const requestId = ++latestDiagnosisRequestId
    loadedEncounterId.value = encounterId
    diagnosisRecords.value = []
    diagnoses.value = []
    savedDiagnosisSnapshot.value = '[]'
    diagnosesLoading.value = true
    diagnosesError.value = ''

    try {
      const result = await pageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 100,
        encounterId
      })
      if (requestId !== latestDiagnosisRequestId || loadedEncounterId.value !== encounterId) return

      const records = (result.rows ?? [])
        .slice()
        .sort((left, right) => left.orderNo - right.orderNo)
      const clinicalRows = records.map(toInpatientClinicalDiagnosis)
      diagnosisRecords.value = records
      diagnoses.value = clinicalRows
      savedDiagnosisSnapshot.value = serializeDiagnoses(clinicalRows)
    } catch (error) {
      if (requestId !== latestDiagnosisRequestId || loadedEncounterId.value !== encounterId) return
      diagnosesError.value = error instanceof Error ? error.message : '住院诊断加载失败'
    } finally {
      if (requestId === latestDiagnosisRequestId) {
        diagnosesLoading.value = false
      }
    }
  }

  async function loadUsageHistory(keyword = usageHistoryKeyword.value) {
    const requestId = ++latestUsageHistoryRequestId
    usageHistoryKeyword.value = keyword.trim()
    usageHistoryLoading.value = true
    usageHistoryError.value = ''

    try {
      const rows = await getInpatientDiagnosisUsageHistory({
        keyword: usageHistoryKeyword.value || undefined,
        limit: 20
      })
      if (requestId !== latestUsageHistoryRequestId) return
      usageHistory.value = rows
    } catch (error) {
      if (requestId !== latestUsageHistoryRequestId) return
      usageHistory.value = []
      usageHistoryError.value = error instanceof Error ? error.message : '诊断使用历史加载失败'
    } finally {
      if (requestId === latestUsageHistoryRequestId) {
        usageHistoryLoading.value = false
      }
    }
  }

  const loadDiagnosisDictionary: ClinicalDiagnosisDictionaryLoader = async (query, context) => {
    if (context.signal.aborted) return { rows: [], total: 0 }

    const result = await pageInpatientDiagnosisDictionary({
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      keyword: query.keyword || undefined
    })

    if (context.signal.aborted) return { rows: [], total: 0 }
    return {
      rows: (result.rows ?? []).map(toInpatientDiagnosisCandidate),
      total: result.total
    }
  }

  function addDiagnosisFromHistory(history: InpatientDiagnosisUsageHistoryVO) {
    diagnoses.value = addClinicalDiagnoses(diagnoses.value, [
      toInpatientDiagnosisCandidate(history)
    ])
  }

  async function saveDiagnoses(encounterId: string, patientId: string) {
    if (loadedEncounterId.value !== encounterId || diagnosesSaving.value) return false
    if (!hasValidPrimaryDiagnosis(diagnoses.value)) {
      throw new Error('请至少添加一条诊断，并且只能设置一个主诊断')
    }

    const savedById = new Map(diagnosisRecords.value.map((record) => [record.id, record]))
    const currentRecordIds = new Set(
      diagnoses.value
        .map((diagnosis) => diagnosis.recordId)
        .filter((id): id is string => Boolean(id))
    )
    const deletedRecords = diagnosisRecords.value.filter(
      (record) => !currentRecordIds.has(record.id)
    )
    let changedCount = 0
    diagnosesSaving.value = true

    try {
      for (const record of deletedRecords) {
        await deleteInpatientDiagnosis(record.id, record.version)
        changedCount += 1
      }

      for (const [index, diagnosis] of diagnoses.value.entries()) {
        if (!diagnosis.recordId) {
          await createInpatientDiagnosis(
            toCreateInpatientDiagnosisDTO(diagnosis, index, encounterId, patientId)
          )
          changedCount += 1
          continue
        }

        const saved = savedById.get(diagnosis.recordId)
        if (!saved) throw new Error('诊断原始记录不存在，请刷新后重试')
        if (!diagnosisChanged(saved, diagnosis, index)) continue

        await updateInpatientDiagnosis(
          diagnosis.recordId,
          toUpdateInpatientDiagnosisDTO(diagnosis, index, saved.version)
        )
        changedCount += 1
      }

      if (loadedEncounterId.value === encounterId) {
        await Promise.all([loadDiagnoses(encounterId), loadUsageHistory()])
      }
      return changedCount
    } catch (error) {
      if (loadedEncounterId.value === encounterId) {
        await loadDiagnoses(encounterId)
      }
      throw error
    } finally {
      diagnosesSaving.value = false
    }
  }

  return {
    addDiagnosisFromHistory,
    diagnoses,
    diagnosesDirty,
    diagnosesError,
    diagnosesLoading,
    diagnosesSaving,
    diagnosisUpdatedAt,
    loadDiagnoses,
    loadDiagnosisDictionary,
    loadUsageHistory,
    resetDiagnoses,
    saveDiagnoses,
    usageHistory,
    usageHistoryError,
    usageHistoryKeyword,
    usageHistoryLoading
  }
}
