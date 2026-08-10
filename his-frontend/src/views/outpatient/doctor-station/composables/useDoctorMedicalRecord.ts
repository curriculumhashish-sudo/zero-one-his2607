import { computed, ref, type ComputedRef, type Ref } from 'vue'
import { ElMessage } from 'element-plus'

import {
  getMedicalRecord,
  saveMedicalRecord,
  type DoctorMedicalRecordVO
} from '@/apis/outpatient/doctor-station'

interface DoctorMedicalRecordOptions {
  selectedPatientId: Ref<string | number | null>
  patientAccepted: ComputedRef<boolean>
}

/** 管理门诊病历正文、服务端快照和保存流程。 */
export function useDoctorMedicalRecord(options: DoctorMedicalRecordOptions) {
  const { selectedPatientId, patientAccepted } = options

  const medicalRecord = ref<DoctorMedicalRecordVO | null>(null)
  const medicalRecordContent = ref('')
  const savedMedicalRecordContent = ref('')
  const medicalRecordLoading = ref(false)
  const medicalRecordSaving = ref(false)
  const medicalRecordError = ref('')
  const medicalRecordDirty = computed(
    () => medicalRecordContent.value !== savedMedicalRecordContent.value
  )

  let latestMedicalRecordRequestId = 0

  async function loadCurrentMedicalRecord(encounterId: string | number) {
    const requestId = ++latestMedicalRecordRequestId
    medicalRecordLoading.value = true
    medicalRecordError.value = ''
    medicalRecord.value = null
    medicalRecordContent.value = ''
    savedMedicalRecordContent.value = ''

    try {
      const result = await getMedicalRecord(String(encounterId))
      if (requestId !== latestMedicalRecordRequestId) return

      medicalRecord.value = result
      medicalRecordContent.value = result.content
      savedMedicalRecordContent.value = result.content
    } catch (error) {
      if (requestId !== latestMedicalRecordRequestId) return
      medicalRecordError.value = error instanceof Error ? error.message : '门诊病历加载失败'
    } finally {
      if (requestId === latestMedicalRecordRequestId) medicalRecordLoading.value = false
    }
  }

  function resetMedicalRecord() {
    latestMedicalRecordRequestId++
    medicalRecord.value = null
    medicalRecordContent.value = ''
    savedMedicalRecordContent.value = ''
    medicalRecordError.value = ''
    medicalRecordLoading.value = false
  }

  async function saveCurrentMedicalRecord(notifySuccess = true): Promise<boolean> {
    const encounterId = selectedPatientId.value
    if (
      !encounterId ||
      !patientAccepted.value ||
      !medicalRecord.value ||
      medicalRecordSaving.value
    ) {
      return false
    }

    const encounterIdText = String(encounterId)
    const contentToSave = medicalRecordContent.value
    medicalRecordSaving.value = true

    try {
      const result = await saveMedicalRecord(encounterIdText, {
        content: contentToSave
      })
      if (String(selectedPatientId.value) !== encounterIdText) return false

      medicalRecord.value = result
      savedMedicalRecordContent.value = result.content
      if (notifySuccess) ElMessage.success('门诊病历保存成功')
      return true
    } catch (error) {
      const message = error instanceof Error ? error.message : '门诊病历保存失败'
      ElMessage.error(message)
      return false
    } finally {
      medicalRecordSaving.value = false
    }
  }

  /** 富文本只有空标签或空格时，不能作为已完成病历。 */
  function hasMeaningfulMedicalRecordContent() {
    return Boolean(
      savedMedicalRecordContent.value
        .replace(/<[^>]*>/g, '')
        .replace(/&nbsp;|&#160;/gi, '')
        .trim()
    )
  }

  return {
    medicalRecord,
    medicalRecordContent,
    savedMedicalRecordContent,
    medicalRecordLoading,
    medicalRecordSaving,
    medicalRecordError,
    medicalRecordDirty,
    loadCurrentMedicalRecord,
    resetMedicalRecord,
    saveCurrentMedicalRecord,
    hasMeaningfulMedicalRecordContent
  }
}
