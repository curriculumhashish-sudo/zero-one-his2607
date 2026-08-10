import { reactive, ref, watch, type Ref } from 'vue'

import {
  getMedicalRecord,
  pagePatientMedicalRecordHistory,
  type DoctorMedicalRecordHistoryItemVO,
  type DoctorMedicalRecordHistoryQuery,
  type DoctorMedicalRecordVO,
  type DoctorPatientContextVO
} from '@/apis/outpatient/doctor-station'

interface DoctorMedicalRecordHistoryOptions {
  /** 当前患者上下文提供 patientId，并用 encounterId 排除本次就诊。 */
  patientContext: Ref<DoctorPatientContextVO | null>
}

/** 管理历史病历列表、分页和独立的只读详情，不触碰当前病历编辑状态。 */
export function useDoctorMedicalRecordHistory(options: DoctorMedicalRecordHistoryOptions) {
  const { patientContext } = options

  const historyRows = ref<DoctorMedicalRecordHistoryItemVO[]>([])
  const historyTotal = ref(0)
  const historyKeyword = ref('')
  const historyLoading = ref(false)
  const historyError = ref('')
  const historyActive = ref(false)
  const historyQuery = reactive<Pick<DoctorMedicalRecordHistoryQuery, 'pageIndex' | 'pageSize'>>({
    pageIndex: 1,
    pageSize: 10
  })

  const historyDialogVisible = ref(false)
  const selectedHistoryItem = ref<DoctorMedicalRecordHistoryItemVO | null>(null)
  const historyDetail = ref<DoctorMedicalRecordVO | null>(null)
  const historyDetailLoading = ref(false)
  const historyDetailError = ref('')

  let latestHistoryRequestId = 0
  let latestDetailRequestId = 0
  let loadedContextKey = ''

  function currentContextKey() {
    const context = patientContext.value
    return context ? `${context.patientId}|${context.encounterId}` : ''
  }

  /** 清空列表并使旧请求失效；切换患者时不会短暂展示上一位患者的数据。 */
  function resetHistoryList() {
    latestHistoryRequestId++
    historyRows.value = []
    historyTotal.value = 0
    historyKeyword.value = ''
    historyQuery.pageIndex = 1
    historyLoading.value = false
    historyError.value = ''
    loadedContextKey = ''
  }

  function closeHistoryDetail() {
    latestDetailRequestId++
    historyDialogVisible.value = false
    selectedHistoryItem.value = null
    historyDetail.value = null
    historyDetailLoading.value = false
    historyDetailError.value = ''
  }

  async function loadHistoryList() {
    const context = patientContext.value
    if (!context) {
      resetHistoryList()
      return
    }

    const requestId = ++latestHistoryRequestId
    const contextKey = currentContextKey()
    historyLoading.value = true
    historyError.value = ''

    try {
      const result = await pagePatientMedicalRecordHistory({
        patientId: context.patientId,
        keyword: historyKeyword.value.trim() || undefined,
        pageIndex: historyQuery.pageIndex,
        pageSize: historyQuery.pageSize
      })
      if (requestId !== latestHistoryRequestId || contextKey !== currentContextKey()) return

      historyRows.value = result.rows ?? []
      historyTotal.value = result.total
      loadedContextKey = contextKey
    } catch (error) {
      if (requestId !== latestHistoryRequestId || contextKey !== currentContextKey()) return

      historyRows.value = []
      historyTotal.value = 0
      historyError.value = error instanceof Error ? error.message : '历史病历加载失败'
    } finally {
      if (requestId === latestHistoryRequestId) historyLoading.value = false
    }
  }

  /** 右栏只有进入历史页签时才首次请求，避免医生不查看历史时产生额外流量。 */
  function setHistoryActive(active: boolean) {
    historyActive.value = active
    const contextKey = currentContextKey()
    if (active && contextKey && loadedContextKey !== contextKey && !historyLoading.value) {
      void loadHistoryList()
    }
  }

  function updateHistoryKeyword(keyword: string) {
    historyKeyword.value = keyword
  }

  function searchHistory(keyword: string) {
    historyKeyword.value = keyword.trim()
    historyQuery.pageIndex = 1
    void loadHistoryList()
  }

  function changeHistoryPage(payload: { page: number; pageSize: number }) {
    historyQuery.pageIndex = payload.page
    historyQuery.pageSize = payload.pageSize
    void loadHistoryList()
  }

  async function openHistoryDetail(item: DoctorMedicalRecordHistoryItemVO) {
    const requestId = ++latestDetailRequestId
    selectedHistoryItem.value = item
    historyDialogVisible.value = true
    historyDetail.value = null
    historyDetailLoading.value = true
    historyDetailError.value = ''

    try {
      const result = await getMedicalRecord(item.encounterId)
      if (
        requestId !== latestDetailRequestId ||
        selectedHistoryItem.value?.encounterId !== item.encounterId
      ) {
        return
      }
      historyDetail.value = result
    } catch (error) {
      if (requestId !== latestDetailRequestId) return
      historyDetailError.value = error instanceof Error ? error.message : '历史病历正文加载失败'
    } finally {
      if (requestId === latestDetailRequestId) historyDetailLoading.value = false
    }
  }

  function retryHistoryDetail() {
    if (selectedHistoryItem.value) void openHistoryDetail(selectedHistoryItem.value)
  }

  // patientId 或 encounterId 变化都代表查看上下文变化，需要关闭详情并清除旧列表。
  watch(
    currentContextKey,
    (contextKey) => {
      resetHistoryList()
      closeHistoryDetail()
      if (historyActive.value && contextKey) void loadHistoryList()
    },
    { flush: 'sync' }
  )

  return {
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
  }
}
