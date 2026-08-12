import { computed, reactive, ref } from 'vue'
import { ElMessage } from 'element-plus'

import {
  pageInpatientDoctorPatientQueue,
  type InpatientDoctorPatientQueueItemVO,
  type InpatientDoctorPatientQueueQuery
} from '@/apis/inpatient/doctor-station'
import type { PatientSummary } from '@/components/patient/type'
import { toInpatientPatientSummary } from '../adapter'

export type InpatientPatientQueueLoadStatus = 'success' | 'failed' | 'stale'
export type InpatientDischargeReconcileStatus = 'removed' | 'still-present' | 'refresh-failed'

export function useInpatientPatientQueue() {
  const queueQuery = reactive<InpatientDoctorPatientQueueQuery>({
    pageIndex: 1,
    pageSize: 50,
    keyword: undefined
  })
  const queueItems = ref<InpatientDoctorPatientQueueItemVO[]>([])
  const patients = ref<PatientSummary[]>([])
  const queueTotal = ref(0)
  const queueLoading = ref(false)
  const selectedPatientId = ref<string | number | null>(null)

  const selectedPatient = computed(
    () => patients.value.find((patient) => patient.id === selectedPatientId.value) ?? null
  )
  const selectedQueueItem = computed(
    () =>
      queueItems.value.find((patient) => patient.encounterId === selectedPatientId.value) ?? null
  )

  // Only the latest queue request may update state.
  let latestRequestId = 0

  async function loadPatients(
    options: { notifyError?: boolean } = {}
  ): Promise<InpatientPatientQueueLoadStatus> {
    const requestId = ++latestRequestId
    queueLoading.value = true

    try {
      const result = await pageInpatientDoctorPatientQueue({ ...queueQuery })
      if (requestId !== latestRequestId) return 'stale' as const

      const pageCount =
        result.pages ??
        Math.ceil(result.total / Math.max(result.pageSize || queueQuery.pageSize, 1))
      if (result.total === 0 && queueQuery.pageIndex > 1) {
        queueQuery.pageIndex = 1
      } else if (result.total > 0 && queueQuery.pageIndex > Math.max(pageCount, 1)) {
        queueQuery.pageIndex = Math.max(pageCount, 1)
        return loadPatients(options)
      }

      queueItems.value = result.rows ?? []
      patients.value = queueItems.value.map(toInpatientPatientSummary)
      queueTotal.value = result.total

      const selectedStillExists = patients.value.some(
        (patient) => patient.id === selectedPatientId.value
      )
      if (!selectedStillExists) {
        selectedPatientId.value = patients.value[0]?.id ?? null
      }
      return 'success' as const
    } catch (error) {
      if (requestId !== latestRequestId) return 'stale' as const

      queueItems.value = []
      patients.value = []
      queueTotal.value = 0
      selectedPatientId.value = null
      if (options.notifyError !== false) {
        const message = error instanceof Error ? error.message : '在院患者队列加载失败'
        ElMessage.error(`${message}，请点击刷新重试`)
      }
      return 'failed' as const
    } finally {
      if (requestId === latestRequestId) {
        queueLoading.value = false
      }
    }
  }

  async function searchPatients(keyword: string) {
    queueQuery.keyword = keyword.trim() || undefined
    queueQuery.pageIndex = 1
    return loadPatients()
  }

  async function changePatientPage(pageIndex: number) {
    queueQuery.pageIndex = pageIndex
    return loadPatients()
  }

  async function reconcileDischarge(
    encounterId: string
  ): Promise<InpatientDischargeReconcileStatus> {
    const loadStatus: InpatientPatientQueueLoadStatus = await loadPatients({ notifyError: false })
    if (loadStatus !== 'success') return 'refresh-failed'
    return queueItems.value.some((patient) => patient.encounterId === encounterId)
      ? 'still-present'
      : 'removed'
  }

  function selectPatient(patient: PatientSummary) {
    selectedPatientId.value = patient.id
  }

  return {
    changePatientPage,
    loadPatients,
    patients,
    queueItems,
    queueLoading,
    queueQuery,
    queueTotal,
    reconcileDischarge,
    searchPatients,
    selectPatient,
    selectedPatient,
    selectedPatientId,
    selectedQueueItem
  }
}
