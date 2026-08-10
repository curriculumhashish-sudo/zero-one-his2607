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

const realDiagnosisCachePrefix = 'inpatient-doctor-station:diagnosis:'

function diagnosisCacheKey(record: InpatientDiagnosisRecordVO) {
  return record.backendId || record.id
}

function diagnosisBusinessKey(record: InpatientDiagnosisRecordVO) {
  return [record.catalogId, record.code, record.name].join('|')
}

function readRealDiagnosisCache(encounterId: string): InpatientDiagnosisRecordVO[] {
  try {
    const raw = window.sessionStorage.getItem(`${realDiagnosisCachePrefix}${encounterId}`)
    if (!raw) return []
    const rows = JSON.parse(raw) as InpatientDiagnosisRecordVO[]
    return Array.isArray(rows)
      ? rows.filter((row) => row?.source === 'real' && row.encounterId === encounterId)
      : []
  } catch {
    return []
  }
}

function writeRealDiagnosisCache(encounterId: string, rows: InpatientDiagnosisRecordVO[]) {
  try {
    window.sessionStorage.setItem(
      `${realDiagnosisCachePrefix}${encounterId}`,
      JSON.stringify(rows.filter((row) => row.source === 'real'))
    )
  } catch {
    // 会话缓存不可用时仍保留当前页面内的真实诊断结果。
  }
}

function mergeDiagnosisRecords(
  encounterId: string,
  rows: InpatientDiagnosisRecordVO[]
): InpatientDiagnosisRecordVO[] {
  const cachedRows = readRealDiagnosisCache(encounterId)
  const merged = new Map<string, InpatientDiagnosisRecordVO>()
  for (const row of cachedRows) merged.set(diagnosisBusinessKey(row), row)
  for (const row of rows) {
    const normalized = { ...row, source: row.source ?? 'mock' as const }
    if (!merged.has(diagnosisBusinessKey(normalized))) {
      merged.set(diagnosisBusinessKey(normalized), normalized)
    }
  }
  return [...merged.values()].sort((left, right) => left.orderNo - right.orderNo)
}

function updateRealDiagnosisCache(encounterId: string, record: InpatientDiagnosisRecordVO) {
  const rows = readRealDiagnosisCache(encounterId)
  const key = diagnosisCacheKey(record)
  const next = rows.filter((row) => diagnosisCacheKey(row) !== key)
  next.push(record)
  writeRealDiagnosisCache(encounterId, next)
}

function removeRealDiagnosisCache(encounterId: string, record: InpatientDiagnosisRecordVO) {
  const key = diagnosisCacheKey(record)
  writeRealDiagnosisCache(
    encounterId,
    readRealDiagnosisCache(encounterId).filter((row) => diagnosisCacheKey(row) !== key)
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

      const records = mergeDiagnosisRecords(encounterId, result.rows ?? [])
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
        if (record.source === 'real') {
          await deleteInpatientDiagnosis(record.id, record.version, {
            source: record.source,
            backendId: record.backendId
          })
        } else {
          await deleteInpatientDiagnosis(record.id, record.version)
        }
        if (record.source === 'real') removeRealDiagnosisCache(encounterId, record)
        changedCount += 1
      }

      for (const [index, diagnosis] of diagnoses.value.entries()) {
        if (!diagnosis.recordId) {
          const created = await createInpatientDiagnosis(
            toCreateInpatientDiagnosisDTO(diagnosis, index, encounterId, patientId)
          )
          if (created.source === 'real') updateRealDiagnosisCache(encounterId, created)
          changedCount += 1
          continue
        }

        const saved = savedById.get(diagnosis.recordId)
        if (!saved) throw new Error('诊断原始记录不存在，请刷新后重试')
        if (!diagnosisChanged(saved, diagnosis, index)) continue

        const updateData = toUpdateInpatientDiagnosisDTO(diagnosis, index, saved.version)
        if (saved.source === 'real') {
          const unsupportedChanged =
            saved.catalogId !== diagnosis.catalogId ||
            saved.code !== diagnosis.code ||
            saved.name !== diagnosis.name ||
            saved.primary !== diagnosis.primary ||
            saved.orderNo !== index + 1
          if (unsupportedChanged) {
            throw new Error('当前后端仅支持修改诊断类型，名称、编码、主诊断和顺序暂不可修改')
          }
        }

        const updated =
          saved.source === 'real'
            ? await updateInpatientDiagnosis(diagnosis.recordId, updateData, {
                source: saved.source,
                current: saved
              })
            : await updateInpatientDiagnosis(diagnosis.recordId, updateData)
        if (updated.source === 'real') updateRealDiagnosisCache(encounterId, updated)
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
