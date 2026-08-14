import { computed, ref } from 'vue'

import { acceptPatient, type DoctorPatientContextVO } from '@/apis/outpatient/doctor-station'

/** 管理当前患者的完整就诊上下文，以及由状态码派生出的编辑权限。 */
export function useDoctorPatientContext() {
  const patientContext = ref<DoctorPatientContextVO | null>(null)
  const contextLoading = ref(false)
  const contextError = ref('')

  const patientAccepted = computed(() => patientContext.value?.visitStatus === 'in-progress')
  const patientReadonly = computed(() => patientContext.value?.visitStatus === 'finished')

  let latestContextRequestId = 0

  /** 接诊待诊患者，或加载在诊、完诊患者的已有上下文。 */
  async function acceptOrLoadPatient(
    encounterId: string | number
  ): Promise<DoctorPatientContextVO | null> {
    const requestId = ++latestContextRequestId
    contextLoading.value = true
    contextError.value = ''
    patientContext.value = null

    try {
      const result = await acceptPatient(String(encounterId))
      if (requestId !== latestContextRequestId) return null
      patientContext.value = result
      return result
    } catch (error) {
      if (requestId !== latestContextRequestId) return null
      contextError.value = error instanceof Error ? error.message : '患者就诊信息加载失败'
      throw error
    } finally {
      if (requestId === latestContextRequestId) contextLoading.value = false
    }
  }

  /** 清空选择时使在途请求失效，旧患者响应不能重新写回页面。 */
  function resetPatientContext() {
    latestContextRequestId++
    patientContext.value = null
    contextError.value = ''
    contextLoading.value = false
  }

  return {
    patientContext,
    contextLoading,
    contextError,
    patientAccepted,
    patientReadonly,
    acceptOrLoadPatient,
    resetPatientContext
  }
}
