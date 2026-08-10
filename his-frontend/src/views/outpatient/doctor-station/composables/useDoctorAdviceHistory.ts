import { reactive, ref, watch, type Ref } from 'vue'

import {
  pagePatientAdviceOrderHistory,
  pagePatientAdviceOrderHistoryDetails,
  type DoctorAdviceOrderHistoryEncounterVO,
  type DoctorAdviceOrderVO,
  type DoctorPatientContextVO
} from '@/apis/outpatient/doctor-station'

interface DoctorAdviceHistoryOptions {
  patientContext: Ref<DoctorPatientContextVO | null>
}

/** 管理历史医嘱抽屉的两级分页，状态与本次医嘱编辑完全隔离。 */
export function useDoctorAdviceHistory(options: DoctorAdviceHistoryOptions) {
  const { patientContext } = options
  const adviceHistoryVisible = ref(false)

  const historyEncounters = ref<DoctorAdviceOrderHistoryEncounterVO[]>([])
  const historyEncounterTotal = ref(0)
  const historyEncounterLoading = ref(false)
  const historyEncounterError = ref('')
  const historyEncounterQuery = reactive({
    pageIndex: 1,
    pageSize: 10,
    keyword: ''
  })

  const selectedHistoryEncounter = ref<DoctorAdviceOrderHistoryEncounterVO | null>(null)
  const historyAdviceOrders = ref<DoctorAdviceOrderVO[]>([])
  const historyAdviceTotal = ref(0)
  const historyAdviceLoading = ref(false)
  const historyAdviceError = ref('')
  const historyAdviceQuery = reactive({
    pageIndex: 1,
    pageSize: 10,
    keyword: ''
  })

  let latestEncounterRequestId = 0
  let latestAdviceRequestId = 0

  function resetDetails() {
    latestAdviceRequestId++
    selectedHistoryEncounter.value = null
    historyAdviceOrders.value = []
    historyAdviceTotal.value = 0
    historyAdviceLoading.value = false
    historyAdviceError.value = ''
    historyAdviceQuery.pageIndex = 1
    historyAdviceQuery.pageSize = 10
    historyAdviceQuery.keyword = ''
  }

  function resetHistory() {
    latestEncounterRequestId++
    historyEncounters.value = []
    historyEncounterTotal.value = 0
    historyEncounterLoading.value = false
    historyEncounterError.value = ''
    historyEncounterQuery.pageIndex = 1
    historyEncounterQuery.pageSize = 10
    historyEncounterQuery.keyword = ''
    resetDetails()
  }

  async function loadHistoryEncounters() {
    const context = patientContext.value
    if (!context) {
      resetHistory()
      return
    }
    const requestId = ++latestEncounterRequestId
    const patientId = context.patientId
    historyEncounterLoading.value = true
    historyEncounterError.value = ''
    try {
      const result = await pagePatientAdviceOrderHistory({
        patientId,
        keyword: historyEncounterQuery.keyword.trim() || undefined,
        pageIndex: historyEncounterQuery.pageIndex,
        pageSize: historyEncounterQuery.pageSize
      })
      if (requestId !== latestEncounterRequestId || patientContext.value?.patientId !== patientId) {
        return
      }
      historyEncounters.value = result.rows ?? []
      historyEncounterTotal.value = result.total
      if (
        selectedHistoryEncounter.value &&
        !historyEncounters.value.some(
          (item) => item.encounterId === selectedHistoryEncounter.value?.encounterId
        )
      ) {
        resetDetails()
      }
    } catch (error) {
      if (requestId !== latestEncounterRequestId) return
      historyEncounters.value = []
      historyEncounterTotal.value = 0
      historyEncounterError.value = error instanceof Error ? error.message : '历史就诊加载失败'
    } finally {
      if (requestId === latestEncounterRequestId) historyEncounterLoading.value = false
    }
  }

  async function loadHistoryAdviceOrders() {
    const context = patientContext.value
    const encounter = selectedHistoryEncounter.value
    if (!context || !encounter) {
      historyAdviceOrders.value = []
      historyAdviceTotal.value = 0
      return
    }
    const requestId = ++latestAdviceRequestId
    const encounterId = encounter.encounterId
    historyAdviceLoading.value = true
    historyAdviceError.value = ''
    try {
      const result = await pagePatientAdviceOrderHistoryDetails({
        patientId: context.patientId,
        encounterId,
        keyword: historyAdviceQuery.keyword.trim() || undefined,
        pageIndex: historyAdviceQuery.pageIndex,
        pageSize: historyAdviceQuery.pageSize
      })
      if (
        requestId !== latestAdviceRequestId ||
        selectedHistoryEncounter.value?.encounterId !== encounterId
      ) {
        return
      }
      historyAdviceOrders.value = result.rows ?? []
      historyAdviceTotal.value = result.total
    } catch (error) {
      if (requestId !== latestAdviceRequestId) return
      historyAdviceOrders.value = []
      historyAdviceTotal.value = 0
      historyAdviceError.value = error instanceof Error ? error.message : '历史医嘱加载失败'
    } finally {
      if (requestId === latestAdviceRequestId) historyAdviceLoading.value = false
    }
  }

  function openAdviceHistory() {
    if (!patientContext.value) return
    adviceHistoryVisible.value = true
    if (historyEncounters.value.length === 0 && !historyEncounterLoading.value) {
      void loadHistoryEncounters()
    }
  }

  function closeAdviceHistory() {
    adviceHistoryVisible.value = false
  }

  function selectHistoryEncounter(item: DoctorAdviceOrderHistoryEncounterVO) {
    selectedHistoryEncounter.value = item
    historyAdviceQuery.pageIndex = 1
    historyAdviceQuery.keyword = ''
    void loadHistoryAdviceOrders()
  }

  function updateHistoryEncounterQuery(
    patch: Partial<typeof historyEncounterQuery>,
    resetPage = false
  ) {
    Object.assign(historyEncounterQuery, patch)
    if (resetPage) historyEncounterQuery.pageIndex = 1
    void loadHistoryEncounters()
  }

  function updateHistoryAdviceQuery(patch: Partial<typeof historyAdviceQuery>, resetPage = false) {
    Object.assign(historyAdviceQuery, patch)
    if (resetPage) historyAdviceQuery.pageIndex = 1
    void loadHistoryAdviceOrders()
  }

  watch(
    () => `${patientContext.value?.patientId ?? ''}|${patientContext.value?.encounterId ?? ''}`,
    () => {
      adviceHistoryVisible.value = false
      resetHistory()
    },
    { flush: 'sync' }
  )

  return {
    adviceHistoryVisible,
    historyEncounters,
    historyEncounterTotal,
    historyEncounterLoading,
    historyEncounterError,
    historyEncounterQuery,
    selectedHistoryEncounter,
    historyAdviceOrders,
    historyAdviceTotal,
    historyAdviceLoading,
    historyAdviceError,
    historyAdviceQuery,
    openAdviceHistory,
    closeAdviceHistory,
    loadHistoryEncounters,
    selectHistoryEncounter,
    updateHistoryEncounterQuery,
    loadHistoryAdviceOrders,
    updateHistoryAdviceQuery
  }
}
