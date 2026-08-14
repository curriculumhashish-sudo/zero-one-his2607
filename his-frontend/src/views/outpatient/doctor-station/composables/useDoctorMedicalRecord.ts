import { computed, ref, type ComputedRef, type Ref } from 'vue'
import { ElMessage } from 'element-plus'

import {
  getMedicalRecord,
  saveMedicalRecord,
  type DoctorMedicalRecordVO
} from '@/apis/outpatient/doctor-station'
import { isMedicalRecordNotFoundError } from '@/apis/outpatient/doctor-station/medical-record-real'

interface DoctorMedicalRecordOptions {
  selectedPatientId: Ref<string | number | null>
  patientAccepted: ComputedRef<boolean>
}

function hasMeaningfulMedicalRecordContent(content: string): boolean {
  return Boolean(
    content
      .replace(/<[^>]*>/g, '')
      .replace(/&nbsp;|&#160;/gi, '')
      .trim()
  )
}

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
      medicalRecordContent.value = result.content ?? ''
      savedMedicalRecordContent.value = result.content ?? ''
    } catch (error) {
      if (requestId !== latestMedicalRecordRequestId) return
      const message = error instanceof Error ? error.message : '门诊病历加载失败'
      if (isMedicalRecordNotFoundError(error)) {
        medicalRecord.value = { encounterId: String(encounterId), content: '' }
        medicalRecordContent.value = ''
        savedMedicalRecordContent.value = ''
      } else {
        medicalRecordError.value = message
      }
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
    if (!encounterId || !patientAccepted.value || medicalRecordSaving.value) return false

    const encounterIdText = String(encounterId)
    const contentToSave = medicalRecordContent.value
    medicalRecordSaving.value = true

    try {
      const result = await saveMedicalRecord(encounterIdText, { content: contentToSave })
      if (String(selectedPatientId.value) !== encounterIdText) return false

      medicalRecord.value = result
      savedMedicalRecordContent.value = result.content ?? contentToSave
      medicalRecordContent.value = savedMedicalRecordContent.value
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
    hasMeaningfulMedicalRecordContent: () =>
      hasMeaningfulMedicalRecordContent(savedMedicalRecordContent.value)
  }
}
