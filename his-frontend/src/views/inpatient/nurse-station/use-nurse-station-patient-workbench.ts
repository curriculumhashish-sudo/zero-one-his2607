import { computed, onBeforeUnmount, ref, watch } from 'vue'

import {
  getNurseStationPatientStatistics,
  loadNurseStationWardTree,
  pageNurseStationPatientQueue
} from '@/apis/inpatient/nurse-station'

import {
  buildPatientFilterQuery,
  mapPatientQueueItemToSummary,
  mapWardTreeToOptions
} from './adapter'
import type {
  NurseStationHouseOption,
  NurseStationPatientStatistics,
  NurseStationPatientSummary,
  NurseStationQueueScope,
  NurseStationSelectionPolicy,
  NurseStationTaskFilter,
  NurseStationWardOption
} from './type'
import { normalizeSelectedPatientIds } from './selection'

const PATIENT_QUEUE_PAGE_SIZE = 50
const KEYWORD_DEBOUNCE_MS = 300

export function useNurseStationPatientWorkbench(
  getSelectionPolicy: () => NurseStationSelectionPolicy
) {
  const wardOptions = ref<NurseStationWardOption[]>([])
  const houseOptions = ref<NurseStationHouseOption[]>([])
  const selectedWardIds = ref<string[]>([])
  const selectedHouseIds = ref<string[]>([])
  const patients = ref<NurseStationPatientSummary[]>([])
  const selectedPatientIds = ref<string[]>([])
  const statistics = ref<NurseStationPatientStatistics>({
    totalCount: 0,
    pendingAuditCount: 0,
    pendingExecuteCount: 0
  })
  const keyword = ref('')
  const queueScope = ref<NurseStationQueueScope>('in-hospital')
  const taskFilter = ref<NurseStationTaskFilter>('all')
  const loading = ref(false)
  const errorMessage = ref('')
  const bootstrapped = ref(false)

  const availableHouseOptions = computed(() => {
    const wardIdSet = new Set(selectedWardIds.value)
    return houseOptions.value.filter((house) => wardIdSet.size === 0 || wardIdSet.has(house.wardId))
  })

  let requestVersion = 0
  let keywordTimer: ReturnType<typeof setTimeout> | undefined

  function buildCurrentFilterQuery() {
    return buildPatientFilterQuery({
      keyword: keyword.value,
      queueScope: queueScope.value,
      taskFilter: taskFilter.value,
      selectedWardIds: selectedWardIds.value,
      selectedHouseIds: selectedHouseIds.value
    })
  }

  async function fetchAllPatientRows() {
    const filter = buildCurrentFilterQuery()
    const rows: NurseStationPatientSummary[] = []
    let pageIndex = 1
    let total = 0
    do {
      const page = await pageNurseStationPatientQueue({
        ...filter,
        pageIndex,
        pageSize: PATIENT_QUEUE_PAGE_SIZE
      })
      const pageRows = page.rows ?? []
      rows.push(...pageRows.map(mapPatientQueueItemToSummary))
      total = page.total
      pageIndex += 1
      if (pageRows.length === 0) break
    } while (rows.length < total)
    return rows
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
      const [nextPatients, nextStatistics] = await Promise.all([
        fetchAllPatientRows(),
        getNurseStationPatientStatistics(filter)
      ])
      if (currentVersion !== requestVersion) return
      patients.value = nextPatients
      statistics.value = nextStatistics
      normalizeSelection()
    } catch (error) {
      if (currentVersion !== requestVersion) return
      patients.value = []
      statistics.value = { totalCount: 0, pendingAuditCount: 0, pendingExecuteCount: 0 }
      errorMessage.value = error instanceof Error ? error.message : '加载患者队列失败，请稍后重试'
    } finally {
      if (currentVersion === requestVersion) loading.value = false
    }
  }

  async function bootstrapPatientWorkbench() {
    loading.value = true
    errorMessage.value = ''
    try {
      const tree = await loadNurseStationWardTree()
      const options = mapWardTreeToOptions(tree)
      wardOptions.value = options.wardOptions
      houseOptions.value = options.houseOptions
      selectedWardIds.value = wardOptions.value.map((ward) => ward.id)
      selectedHouseIds.value = []
      bootstrapped.value = true
      await refreshPatients()
    } catch (error) {
      errorMessage.value = error instanceof Error ? error.message : '加载病区病房数据失败'
      loading.value = false
    }
  }

  function syncHouseSelection() {
    const availableIds = new Set(availableHouseOptions.value.map((house) => house.id))
    const nextIds = selectedHouseIds.value.filter((id) => availableIds.has(id))
    if (
      nextIds.length === selectedHouseIds.value.length &&
      nextIds.every((id, index) => id === selectedHouseIds.value[index])
    ) {
      return false
    }
    selectedHouseIds.value = nextIds
    return true
  }

  watch(
    selectedWardIds,
    () => {
      const houseSelectionChanged = syncHouseSelection()
      if (bootstrapped.value && !houseSelectionChanged) void refreshPatients()
    },
    { flush: 'sync' }
  )
  watch(
    [selectedHouseIds, queueScope, taskFilter],
    () => {
      if (queueScope.value === 'settled-discharged') selectedPatientIds.value = []
      if (bootstrapped.value) void refreshPatients()
    },
    { flush: 'sync' }
  )
  watch(keyword, () => {
    if (!bootstrapped.value) return
    if (keywordTimer) clearTimeout(keywordTimer)
    keywordTimer = setTimeout(() => void refreshPatients(), KEYWORD_DEBOUNCE_MS)
  })

  onBeforeUnmount(() => {
    if (keywordTimer) clearTimeout(keywordTimer)
  })

  return {
    availableHouseOptions,
    bootstrapPatientWorkbench,
    errorMessage,
    houseOptions,
    keyword,
    loading,
    normalizeSelection,
    patients,
    queueScope,
    refreshPatients,
    selectedHouseIds,
    selectedPatientIds,
    selectedWardIds,
    statistics,
    taskFilter,
    wardOptions
  }
}
