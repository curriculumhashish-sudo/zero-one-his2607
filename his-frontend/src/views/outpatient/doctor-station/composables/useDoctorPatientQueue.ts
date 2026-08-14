import { computed, reactive, ref } from 'vue'
import dayjs from 'dayjs'
import { ElMessage } from 'element-plus'

import {
  listDoctorPatientQueue,
  type DoctorPatientQueueQuery
} from '@/apis/outpatient/doctor-station'
import type { PatientSummary } from '@/components/patient/type'

import { toPatientSummary } from '../adapter'
import type { DoctorQueueStatusFilter } from '../type'

/**
 * 管理左侧患者队列。
 *
 * 查询条件、加载状态和选中 ID 都只属于医生站页面，因此使用 composable，
 * 不放进全局 Pinia。离开页面后这些状态会随组件实例一起释放。
 */
export function useDoctorPatientQueue() {
  const patients = ref<PatientSummary[]>([])
  const selectedPatientId = ref<string | number | null>(null)
  /**
   * 当前患者的摘要快照不能只依赖“当前页患者列表”。
   * 医生翻到下一页时，正在诊疗的患者可能不在新页中，但中间工作区仍应保留。
   */
  const selectedPatientSnapshot = ref<PatientSummary | null>(null)
  const selectedPatient = computed(() => {
    const patientOnCurrentPage = patients.value.find(
      (patient) => patient.id === selectedPatientId.value
    )

    if (patientOnCurrentPage) return patientOnCurrentPage
    if (selectedPatientSnapshot.value?.id === selectedPatientId.value) {
      return selectedPatientSnapshot.value
    }
    return null
  })

  /** 患者队列默认只查看当天，避免首次进入页面加载医生的全部历史患者。 */
  const today = dayjs().format('YYYY-MM-DD')
  const queueDate = ref(today)

  const queueQuery = reactive<DoctorPatientQueueQuery>({
    patientName: undefined,
    visitDate: today
  })
  const queueLoading = ref(false)
  const selectedQueueStatus = ref<DoctorQueueStatusFilter>('ALL')

  let latestQueueRequestId = 0

  /** 只允许最后一次队列请求更新页面，防止旧响应覆盖新查询。 */
  async function loadPatients() {
    const requestId = ++latestQueueRequestId
    queueLoading.value = true

    try {
      const result = await listDoctorPatientQueue({ ...queueQuery })
      if (requestId !== latestQueueRequestId) return

      patients.value = result.map(toPatientSummary)

      // 如果当前患者仍在本页，用后端最新摘要刷新快照；不在本页则继续保留原快照。
      const refreshedSelectedPatient = patients.value.find(
        (patient) => patient.id === selectedPatientId.value
      )
      if (refreshedSelectedPatient) {
        selectedPatientSnapshot.value = refreshedSelectedPatient
      }
    } catch (error) {
      if (requestId !== latestQueueRequestId) return

      patients.value = []
      const message = error instanceof Error ? error.message : '患者队列加载失败'
      ElMessage.error(`${message}，请点击刷新重试`)
    } finally {
      if (requestId === latestQueueRequestId) queueLoading.value = false
    }
  }

  /**
   * 重置会连续修改关键词和日期，使用微任务把同一轮变化合并成一次请求。
   */
  let queueReloadScheduled = false
  function scheduleLoadPatients() {
    if (queueReloadScheduled) return

    queueReloadScheduled = true
    queueMicrotask(() => {
      queueReloadScheduled = false
      void loadPatients()
    })
  }

  function searchPatients(keyword: string) {
    queueQuery.patientName = keyword || undefined
    scheduleLoadPatients()
  }

  function changeQueueDate(date: string) {
    // 日期不可为空；清空或重置时恢复今天。
    const nextDate = date || today
    queueDate.value = nextDate
    queueQuery.visitDate = nextDate
    scheduleLoadPatients()
  }

  /** 只修改筛选状态，不立即请求；用于接诊、暂离和完诊后的精确刷新。 */
  function setQueueStatus(status: DoctorQueueStatusFilter) {
    selectedQueueStatus.value = status
    queueQuery.visitStatus = status === 'ALL' ? undefined : status
  }

  function changeQueueStatus(status: DoctorQueueStatusFilter) {
    setQueueStatus(status)
    scheduleLoadPatients()
  }

  /**
   * 只有医生主动选择患者时才更新当前患者。
   * 翻页和查询不会调用此函数，因此不会绕过工作流中的未保存提示。
   */
  function commitSelectedPatient(patient: PatientSummary) {
    selectedPatientSnapshot.value = patient
    selectedPatientId.value = patient.id
  }

  return {
    patients,
    selectedPatientId,
    selectedPatient,
    queueDate,
    queueQuery,
    queueLoading,
    selectedQueueStatus,
    loadPatients,
    searchPatients,
    changeQueueDate,
    changeQueueStatus,
    setQueueStatus,
    commitSelectedPatient
  }
}
