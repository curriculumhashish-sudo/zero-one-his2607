import { computed, ref } from 'vue'

import {
  createInpatientDiagnosis,
  deleteInpatientDiagnosis,
  listInpatientDiagnosisBrief,
  pageInpatientDiagnosisRecords,
  updateInpatientDiagnosis,
  type InpatientDiagnosisBriefVO,
  type InpatientDiagnosisRecordVO
} from '@/apis/inpatient/doctor-station'
import type { InpatientDiagnosisItem } from '../type'

let nextDraftKey = 1

function toDiagnosisItem(record: InpatientDiagnosisRecordVO): InpatientDiagnosisItem {
  return {
    key: record.id,
    id: record.id,
    conditionId: record.conditionId,
    definitionCode: record.definitionCode,
    description: record.description,
    kindName: record.kindName,
    diagnosisTypeCode: record.diagnosisTypeCode,
    diagnosisTypeName: record.diagnosisTypeName,
    primaryFlag: record.primaryFlag,
    rank: record.rank,
    tcmFlag: record.tcmFlag
  }
}

function normalizeRanks(items: InpatientDiagnosisItem[]): InpatientDiagnosisItem[] {
  return items.map((item, index) => ({ ...item, rank: index + 1 }))
}

function serializeDiagnoses(items: InpatientDiagnosisItem[]) {
  return JSON.stringify(
    normalizeRanks(items).map((item) => ({
      id: item.id,
      conditionId: item.conditionId,
      definitionCode: item.definitionCode,
      description: item.description.trim(),
      kindName: item.kindName,
      diagnosisTypeCode: item.diagnosisTypeCode,
      diagnosisTypeName: item.diagnosisTypeName,
      primaryFlag: item.primaryFlag,
      rank: item.rank,
      tcmFlag: item.tcmFlag
    }))
  )
}

function diagnosisChanged(saved: InpatientDiagnosisRecordVO, current: InpatientDiagnosisItem) {
  return saved.diagnosisTypeCode !== current.diagnosisTypeCode
}

export function useInpatientDiagnosis() {
  const diagnosisRecords = ref<InpatientDiagnosisRecordVO[]>([])
  const diagnoses = ref<InpatientDiagnosisItem[]>([])
  const savedDiagnosisSnapshot = ref('[]')
  const loadedEncounterId = ref('')
  const loadedPatientId = ref('')
  const diagnosesLoading = ref(false)
  const diagnosesSaving = ref(false)
  const diagnosesError = ref('')

  const diagnosisBriefs = ref<InpatientDiagnosisBriefVO[]>([])
  const diagnosisBriefsLoading = ref(false)
  const diagnosisBriefsError = ref('')

  const diagnosesDirty = computed(
    () =>
      Boolean(loadedEncounterId.value) &&
      !diagnosesLoading.value &&
      serializeDiagnoses(diagnoses.value) !== savedDiagnosisSnapshot.value
  )

  let latestDiagnosisRequestId = 0
  let latestBriefRequestId = 0

  function resetDiagnoses() {
    latestDiagnosisRequestId += 1
    loadedEncounterId.value = ''
    loadedPatientId.value = ''
    diagnosisRecords.value = []
    diagnoses.value = []
    savedDiagnosisSnapshot.value = '[]'
    diagnosesLoading.value = false
    diagnosesSaving.value = false
    diagnosesError.value = ''
  }

  async function loadDiagnoses(encounterId: string, patientId: string) {
    const requestId = ++latestDiagnosisRequestId
    loadedEncounterId.value = encounterId
    loadedPatientId.value = patientId
    diagnosisRecords.value = []
    diagnoses.value = []
    savedDiagnosisSnapshot.value = '[]'
    diagnosesLoading.value = true
    diagnosesError.value = ''

    try {
      const result = await pageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 100,
        encounterId,
        patientId
      })
      if (requestId !== latestDiagnosisRequestId || loadedEncounterId.value !== encounterId) return

      const records = (result.rows ?? []).slice().sort((left, right) => left.rank - right.rank)
      const items = normalizeRanks(records.map(toDiagnosisItem))
      diagnosisRecords.value = records
      diagnoses.value = items
      savedDiagnosisSnapshot.value = serializeDiagnoses(items)
    } catch (error) {
      if (requestId !== latestDiagnosisRequestId || loadedEncounterId.value !== encounterId) return
      diagnosesError.value = error instanceof Error ? error.message : '住院诊断加载失败'
    } finally {
      if (requestId === latestDiagnosisRequestId) diagnosesLoading.value = false
    }
  }

  async function loadDiagnosisBriefs() {
    const requestId = ++latestBriefRequestId
    diagnosisBriefsLoading.value = true
    diagnosisBriefsError.value = ''
    try {
      const rows = await listInpatientDiagnosisBrief()
      if (requestId !== latestBriefRequestId) return
      diagnosisBriefs.value = rows
    } catch (error) {
      if (requestId !== latestBriefRequestId) return
      diagnosisBriefs.value = []
      diagnosisBriefsError.value = error instanceof Error ? error.message : '历史诊断加载失败'
    } finally {
      if (requestId === latestBriefRequestId) diagnosisBriefsLoading.value = false
    }
  }

  function addDiagnosisFromBrief(brief: InpatientDiagnosisBriefVO) {
    const description = brief.definitionName.trim()
    if (!description) return
    if (diagnoses.value.some((item) => item.description.trim() === description)) return

    diagnoses.value = normalizeRanks([
      ...diagnoses.value,
      {
        key: `draft-${nextDraftKey++}`,
        definitionCode: '',
        description,
        kindName: '西医',
        diagnosisTypeCode: '入院初步诊断',
        diagnosisTypeName: '入院初步诊断',
        primaryFlag: diagnoses.value.length === 0 ? 1 : 0,
        rank: diagnoses.value.length + 1,
        tcmFlag: 0
      }
    ])
  }

  async function saveDiagnoses(encounterId: string, patientId: string) {
    if (
      loadedEncounterId.value !== encounterId ||
      loadedPatientId.value !== patientId ||
      diagnosesSaving.value
    ) {
      return false
    }

    const normalized = normalizeRanks(diagnoses.value)
    const descriptions = normalized.map((item) => item.description.trim())
    if (normalized.length === 0) throw new Error('请至少添加一条诊断')
    if (descriptions.some((description) => !description)) throw new Error('诊断描述不能为空')
    if (new Set(descriptions).size !== descriptions.length) throw new Error('诊断描述不能重复')
    if (normalized.filter((item) => item.primaryFlag === 1).length !== 1) {
      throw new Error('只能存在一个主诊断')
    }

    diagnoses.value = normalized
    const savedById = new Map(diagnosisRecords.value.map((record) => [record.id, record]))
    const currentIds = new Set(normalized.flatMap((item) => (item.id ? [item.id] : [])))
    const deletedRecords = diagnosisRecords.value.filter((record) => !currentIds.has(record.id))
    let changedCount = 0
    diagnosesSaving.value = true

    try {
      for (const record of deletedRecords) {
        await deleteInpatientDiagnosis({ id: record.id, conditionId: record.conditionId })
        changedCount += 1
      }

      for (const item of normalized) {
        if (!item.id) {
          await createInpatientDiagnosis({
            encounterId,
            patientId,
            definitionCode: item.definitionCode,
            description: item.description.trim(),
            diagnosisTypeCode: item.diagnosisTypeCode,
            primaryFlag: item.primaryFlag,
            rank: item.rank,
            tcmFlag: item.tcmFlag
          })
          changedCount += 1
          continue
        }

        const saved = savedById.get(item.id)
        if (!saved) throw new Error('诊断原始记录不存在，请刷新后重试')
        if (!diagnosisChanged(saved, item)) continue
        await updateInpatientDiagnosis({
          id: item.id,
          diagnosisTypeCode: item.diagnosisTypeCode
        })
        changedCount += 1
      }

      if (loadedEncounterId.value === encounterId && loadedPatientId.value === patientId) {
        await Promise.all([loadDiagnoses(encounterId, patientId), loadDiagnosisBriefs()])
      }
      return changedCount
    } catch (error) {
      if (loadedEncounterId.value === encounterId && loadedPatientId.value === patientId) {
        await loadDiagnoses(encounterId, patientId)
      }
      throw error
    } finally {
      diagnosesSaving.value = false
    }
  }

  return {
    addDiagnosisFromBrief,
    diagnoses,
    diagnosesDirty,
    diagnosesError,
    diagnosesLoading,
    diagnosesSaving,
    diagnosisBriefs,
    diagnosisBriefsError,
    diagnosisBriefsLoading,
    loadDiagnoses,
    loadDiagnosisBriefs,
    resetDiagnoses,
    saveDiagnoses
  }
}
