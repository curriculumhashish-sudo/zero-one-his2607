import { onBeforeUnmount, ref, watch } from 'vue'

import {
  getNurseStationPatientStatistics,
  pageNurseStationPatientQueue
} from '@/apis/inpatient/nurse-station'

import { buildPatientFilterQuery, mapPatientQueueItemToSummary } from './adapter'
import type {
  NurseStationPatientStatistics,
  NurseStationPatientSummary,
  NurseStationSelectionPolicy,
  NurseStationTaskFilter
} from './type'
import { normalizeSelectedPatientIds } from './selection'

const PATIENT_QUEUE_PAGE_SIZE = 20
const KEYWORD_DEBOUNCE_MS = 300

export function useNurseStationPatientWorkbench(
  getSelectionPolicy: () => NurseStationSelectionPolicy
) {
  const patients = ref<NurseStationPatientSummary[]>([])
  const selectedPatientIds = ref<string[]>([])
  const statistics = ref<NurseStationPatientStatistics>({
    totalCount: 0,
    pendingAuditCount: 0,
    pendingExecuteCount: 0
  })
  const keyword = ref('')
  const taskFilter = ref<NurseStationTaskFilter>('all')
  const pageIndex = ref(1)
  const pageSize = PATIENT_QUEUE_PAGE_SIZE
  const total = ref(0)
  const loading = ref(false)
  const errorMessage = ref('')
  const bootstrapped = ref(false)

  let requestVersion = 0
  let keywordTimer: ReturnType<typeof setTimeout> | undefined

  function buildCurrentFilterQuery() {
    return buildPatientFilterQuery({
      keyword: keyword.value,
      taskFilter: taskFilter.value
    })
  }

  async function fetchPatientPage() {
    const filter = buildCurrentFilterQuery()
    return pageNurseStationPatientQueue({
      ...filter,
      pageIndex: pageIndex.value,
      pageSize
    })
  }

  function normalizeSelection() {
    selectedPatientIds.value = normalizeSelectedPatientIds(
      selectedPatientIds.value,
      patients.value.map((patient) => patient.encounterId),
      getSelectionPolicy()
    )
  }

  async function refreshPatients() {
    const currentVersion = ++requestVersion
    loading.value = true
    errorMessage.value = ''
    try {
      const filter = buildCurrentFilterQuery()
      const [page, nextStatistics] = await Promise.all([
        fetchPatientPage(),
        getNurseStationPatientStatistics(filter)
      ])
      if (currentVersion !== requestVersion) return
      patients.value = (page.rows ?? []).map(mapPatientQueueItemToSummary)
      total.value = page.total
      statistics.value = nextStatistics
      normalizeSelection()
    } catch (error) {
      if (currentVersion !== requestVersion) return
      patients.value = []
      total.value = 0
      statistics.value = { totalCount: 0, pendingAuditCount: 0, pendingExecuteCount: 0 }
      errorMessage.value = error instanceof Error ? error.message : '加载患者队列失败，请稍后重试'
    } finally {
      if (currentVersion === requestVersion) loading.value = false
    }
  }

  async function bootstrapPatientWorkbench() {
    bootstrapped.value = true
    await refreshPatients()
  }

  function resetPageAndRefresh() {
    if (pageIndex.value !== 1) pageIndex.value = 1
    else void refreshPatients()
  }

  watch(taskFilter, () => {
    if (bootstrapped.value) resetPageAndRefresh()
  })
  watch(pageIndex, () => {
    if (bootstrapped.value) void refreshPatients()
  })
  watch(keyword, () => {
    if (!bootstrapped.value) return
    if (keywordTimer) clearTimeout(keywordTimer)
    keywordTimer = setTimeout(resetPageAndRefresh, KEYWORD_DEBOUNCE_MS)
  })

  onBeforeUnmount(() => {
    if (keywordTimer) clearTimeout(keywordTimer)
  })

  return {
    bootstrapPatientWorkbench,
    errorMessage,
    keyword,
    loading,
    normalizeSelection,
    pageIndex,
    pageSize,
    patients,
    refreshPatients,
    selectedPatientIds,
    statistics,
    taskFilter,
    total
  }
}
