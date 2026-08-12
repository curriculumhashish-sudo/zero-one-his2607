import { reactive, ref, watch, type Ref } from 'vue'

import {
  pagePatientDiagnosisHistory,
  type DoctorPatientContextVO,
  type DoctorPatientDiagnosisHistoryItemVO
} from '@/apis/outpatient/doctor-station'

interface DoctorDiagnosisHistoryOptions {
  patientContext: Ref<DoctorPatientContextVO | null>
}

function endOfDay(value: string | undefined): string | undefined {
  return value ? `${value} 23:59:59` : undefined
}

/** 管理患者历史诊断，不触碰本次诊断草稿。 */
export function useDoctorDiagnosisHistory(options: DoctorDiagnosisHistoryOptions) {
  const { patientContext } = options

  const diagnosisHistoryVisible = ref(false)
  const patientDiagnosisHistoryRows = ref<DoctorPatientDiagnosisHistoryItemVO[]>([])
  const patientDiagnosisHistoryTotal = ref(0)
  const patientDiagnosisHistoryLoading = ref(false)
  const patientDiagnosisHistoryError = ref('')
  const patientDiagnosisHistoryQuery = reactive<{
    pageIndex: number
    pageSize: number
    keyword: string
    dateRange: [string, string] | []
  }>({
    pageIndex: 1,
    pageSize: 10,
    keyword: '',
    dateRange: []
  })

  let latestHistoryRequestId = 0

  function currentContextKey() {
    const context = patientContext.value
    return context ? `${context.patientId}|${context.encounterId}` : ''
  }

  async function loadPatientDiagnosisHistory() {
    const context = patientContext.value
    if (!context) {
      patientDiagnosisHistoryRows.value = []
      patientDiagnosisHistoryTotal.value = 0
      return
    }
    const requestId = ++latestHistoryRequestId
    const contextKey = currentContextKey()
    patientDiagnosisHistoryLoading.value = true
    patientDiagnosisHistoryError.value = ''
    try {
      const result = await pagePatientDiagnosisHistory({
        patientId: context.patientId,
        keyword: patientDiagnosisHistoryQuery.keyword.trim() || undefined,
        startDate: patientDiagnosisHistoryQuery.dateRange[0] || undefined,
        endDate: endOfDay(patientDiagnosisHistoryQuery.dateRange[1]),
        pageIndex: patientDiagnosisHistoryQuery.pageIndex,
        pageSize: patientDiagnosisHistoryQuery.pageSize
      })
      if (requestId !== latestHistoryRequestId || contextKey !== currentContextKey()) return
      patientDiagnosisHistoryRows.value = result.rows ?? []
      patientDiagnosisHistoryTotal.value = result.total
    } catch (error) {
      if (requestId !== latestHistoryRequestId) return
      patientDiagnosisHistoryRows.value = []
      patientDiagnosisHistoryTotal.value = 0
      patientDiagnosisHistoryError.value =
        error instanceof Error ? error.message : '患者历史诊断加载失败'
    } finally {
      if (requestId === latestHistoryRequestId) {
        patientDiagnosisHistoryLoading.value = false
      }
    }
  }

  function openDiagnosisHistory() {
    if (!patientContext.value) return
    diagnosisHistoryVisible.value = true
    if (patientDiagnosisHistoryRows.value.length === 0 && !patientDiagnosisHistoryLoading.value) {
      void loadPatientDiagnosisHistory()
    }
  }

  function closeDiagnosisHistory() {
    diagnosisHistoryVisible.value = false
  }

  function updatePatientDiagnosisHistoryQuery(
    patch: Partial<typeof patientDiagnosisHistoryQuery>,
    resetPage = false
  ) {
    Object.assign(patientDiagnosisHistoryQuery, patch)
    if (resetPage) patientDiagnosisHistoryQuery.pageIndex = 1
    void loadPatientDiagnosisHistory()
  }

  function resetDiagnosisHistory() {
    latestHistoryRequestId++
    diagnosisHistoryVisible.value = false
    patientDiagnosisHistoryRows.value = []
    patientDiagnosisHistoryTotal.value = 0
    patientDiagnosisHistoryLoading.value = false
    patientDiagnosisHistoryError.value = ''
    patientDiagnosisHistoryQuery.pageIndex = 1
    patientDiagnosisHistoryQuery.pageSize = 10
    patientDiagnosisHistoryQuery.keyword = ''
    patientDiagnosisHistoryQuery.dateRange = []
  }

  watch(
    currentContextKey,
    () => {
      resetDiagnosisHistory()
    },
    { flush: 'sync' }
  )

  return {
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
  }
}
