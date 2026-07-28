import { reactive, ref } from 'vue'

import {
  getInpatientMedicalRecordHistoryDetail,
  pageInpatientMedicalRecordHistory,
  type InpatientMedicalRecordHistoryDetailVO,
  type InpatientMedicalRecordHistoryItemVO,
  type InpatientMedicalRecordHistoryQuery
} from '@/apis/inpatient/doctor-station'

export function useInpatientMedicalRecordHistory() {
  const historyQuery = reactive<InpatientMedicalRecordHistoryQuery>({
    pageIndex: 1,
    pageSize: 10,
    patientId: '',
    currentEncounterId: undefined,
    keyword: undefined
  })
  const historyRows = ref<InpatientMedicalRecordHistoryItemVO[]>([])
  const historyTotal = ref(0)
  const historyLoading = ref(false)
  const historyError = ref('')
  const selectedRecordId = ref<string | null>(null)
  const selectedRecordDetail = ref<InpatientMedicalRecordHistoryDetailVO | null>(null)
  const detailLoading = ref(false)
  const detailError = ref('')

  let latestHistoryRequestId = 0
  let latestDetailRequestId = 0

  function clearDetail() {
    selectedRecordDetail.value = null
    detailError.value = ''
    detailLoading.value = false
  }

  function clearHistorySelection() {
    latestDetailRequestId += 1
    selectedRecordId.value = null
    clearDetail()
  }

  function resetHistory() {
    latestHistoryRequestId += 1
    latestDetailRequestId += 1
    historyQuery.patientId = ''
    historyQuery.currentEncounterId = undefined
    historyQuery.keyword = undefined
    historyQuery.pageIndex = 1
    historyRows.value = []
    historyTotal.value = 0
    historyLoading.value = false
    historyError.value = ''
    selectedRecordId.value = null
    clearDetail()
  }

  async function loadHistoryDetail(recordId: string) {
    const requestId = ++latestDetailRequestId
    selectedRecordId.value = recordId
    selectedRecordDetail.value = null
    detailLoading.value = true
    detailError.value = ''

    try {
      const detail = await getInpatientMedicalRecordHistoryDetail(recordId)
      if (requestId !== latestDetailRequestId || selectedRecordId.value !== recordId) return
      selectedRecordDetail.value = detail
    } catch (error) {
      if (requestId !== latestDetailRequestId || selectedRecordId.value !== recordId) return
      detailError.value = error instanceof Error ? error.message : '历史病历详情加载失败'
    } finally {
      if (requestId === latestDetailRequestId) {
        detailLoading.value = false
      }
    }
  }

  async function loadHistory(patientId: string, currentEncounterId?: string) {
    const requestId = ++latestHistoryRequestId
    latestDetailRequestId += 1
    const patientChanged = historyQuery.patientId !== patientId
    if (patientChanged) {
      historyQuery.keyword = undefined
      historyQuery.documentTypeCode = undefined
      historyQuery.pageIndex = 1
      selectedRecordId.value = null
    }
    historyQuery.patientId = patientId
    historyQuery.currentEncounterId = currentEncounterId
    historyLoading.value = true
    historyError.value = ''
    clearDetail()

    try {
      const result = await pageInpatientMedicalRecordHistory({ ...historyQuery })
      if (requestId !== latestHistoryRequestId) return

      historyRows.value = result.rows ?? []
      historyTotal.value = result.total

      const selectedStillExists = historyRows.value.some(
        (record) => record.id === selectedRecordId.value
      )
      selectedRecordId.value = selectedStillExists
        ? selectedRecordId.value
        : (historyRows.value[0]?.id ?? null)

      if (selectedRecordId.value) {
        await loadHistoryDetail(selectedRecordId.value)
      }
    } catch (error) {
      if (requestId !== latestHistoryRequestId) return
      historyRows.value = []
      historyTotal.value = 0
      selectedRecordId.value = null
      historyError.value = error instanceof Error ? error.message : '历史病历列表加载失败'
    } finally {
      if (requestId === latestHistoryRequestId) {
        historyLoading.value = false
      }
    }
  }

  async function reloadHistory() {
    if (!historyQuery.patientId) return
    await loadHistory(historyQuery.patientId, historyQuery.currentEncounterId)
  }

  async function searchHistory(keyword: string) {
    historyQuery.keyword = keyword.trim() || undefined
    historyQuery.pageIndex = 1
    await reloadHistory()
  }

  async function changeHistoryPage(pageIndex: number) {
    historyQuery.pageIndex = Math.max(1, pageIndex)
    await reloadHistory()
  }

  async function selectHistoryRecord(record: InpatientMedicalRecordHistoryItemVO) {
    if (record.id === selectedRecordId.value && selectedRecordDetail.value) return
    await loadHistoryDetail(record.id)
  }

  return {
    changeHistoryPage,
    clearHistorySelection,
    detailError,
    detailLoading,
    historyError,
    historyLoading,
    historyQuery,
    historyRows,
    historyTotal,
    loadHistory,
    loadHistoryDetail,
    reloadHistory,
    resetHistory,
    searchHistory,
    selectHistoryRecord,
    selectedRecordDetail,
    selectedRecordId
  }
}
