import { computed, ref } from 'vue'

import {
  getInpatientMedicalRecord,
  listInpatientMedicalRecordDocumentTypes,
  saveInpatientMedicalRecord,
  signInpatientMedicalRecord,
  type InpatientDoctorPatientQueueItemVO,
  type InpatientMedicalRecordDocumentTypeCode,
  type InpatientMedicalRecordDocumentTypeOptionVO,
  type InpatientMedicalRecordVO
} from '@/apis/inpatient/doctor-station'

type EditableDocumentTypeCode = Exclude<InpatientMedicalRecordDocumentTypeCode, 'PROGRESS_NOTE'>

function serializeMedicalRecord(
  documentTypeCode: InpatientMedicalRecordDocumentTypeCode | '',
  title: string,
  content: string
) {
  return JSON.stringify({ documentTypeCode, title, content })
}

export function useInpatientMedicalRecord() {
  const documentTypes = ref<InpatientMedicalRecordDocumentTypeOptionVO[]>([])
  const record = ref<InpatientMedicalRecordVO | null>(null)
  const documentTypeCode = ref<InpatientMedicalRecordDocumentTypeCode | ''>('')
  const title = ref('')
  const content = ref('')
  const savedSnapshot = ref('')
  const initialized = ref(false)
  const loading = ref(false)
  const saving = ref(false)
  const signing = ref(false)
  const error = ref('')

  let latestRecordRequestId = 0

  const dirty = computed(
    () =>
      initialized.value &&
      !loading.value &&
      (record.value === null ||
        serializeMedicalRecord(documentTypeCode.value, title.value, content.value) !==
          savedSnapshot.value)
  )
  const readonly = computed(() => Boolean(record.value?.readonly))
  const canSign = computed(
    () => Boolean(record.value?.id) && record.value?.status === 'DRAFT' && !dirty.value
  )

  function hydrateRecord(nextRecord: InpatientMedicalRecordVO) {
    record.value = nextRecord
    documentTypeCode.value = nextRecord.documentTypeCode
    title.value = nextRecord.title
    content.value = nextRecord.content
    savedSnapshot.value = serializeMedicalRecord(
      nextRecord.documentTypeCode,
      nextRecord.title,
      nextRecord.content
    )
    initialized.value = true
    error.value = ''
  }

  function resetMedicalRecord() {
    latestRecordRequestId += 1
    record.value = null
    documentTypeCode.value = ''
    title.value = ''
    content.value = ''
    savedSnapshot.value = ''
    initialized.value = false
    loading.value = false
    saving.value = false
    signing.value = false
    error.value = ''
  }

  async function loadDocumentTypes() {
    if (documentTypes.value.length > 0) return
    documentTypes.value = await listInpatientMedicalRecordDocumentTypes()
  }

  function startNewMedicalRecord(typeCode?: EditableDocumentTypeCode) {
    latestRecordRequestId += 1
    const selectedType =
      documentTypes.value.find((item) => item.value === typeCode) ?? documentTypes.value[0]
    record.value = null
    documentTypeCode.value = selectedType?.value ?? 'ADMISSION_RECORD'
    title.value = selectedType?.defaultTitle ?? '住院病历'
    content.value = selectedType?.defaultContent ?? ''
    savedSnapshot.value = ''
    initialized.value = true
    loading.value = false
    error.value = ''
  }

  function applyDocumentType(typeCode: EditableDocumentTypeCode) {
    documentTypeCode.value = typeCode
    if (record.value) return
    const selectedType = documentTypes.value.find((item) => item.value === typeCode)
    if (!selectedType) return
    title.value = selectedType.defaultTitle
    content.value = selectedType.defaultContent
  }

  async function loadMedicalRecord(recordId: string) {
    const requestId = ++latestRecordRequestId
    loading.value = true
    error.value = ''
    try {
      const result = await getInpatientMedicalRecord(recordId)
      if (requestId !== latestRecordRequestId) return
      hydrateRecord(result)
    } catch (caught) {
      if (requestId !== latestRecordRequestId) return
      record.value = null
      initialized.value = false
      error.value = caught instanceof Error ? caught.message : '住院病历加载失败'
    } finally {
      if (requestId === latestRecordRequestId) loading.value = false
    }
  }

  async function saveMedicalRecordDraft(patient: InpatientDoctorPatientQueueItemVO) {
    if (!initialized.value) throw new Error('请先新建或选择一份病历')
    if (readonly.value) throw new Error('已签名或归档病历不能修改')
    if (saving.value) return null
    if (
      !documentTypeCode.value ||
      documentTypeCode.value === 'PROGRESS_NOTE' ||
      !title.value.trim()
    ) {
      throw new Error('请完善病历文书类型和标题')
    }

    const requestId = latestRecordRequestId
    saving.value = true
    try {
      const result = await saveInpatientMedicalRecord({
        id: record.value?.id,
        encounterId: patient.encounterId,
        patientId: patient.patientId,
        documentTypeCode: documentTypeCode.value,
        title: title.value,
        content: content.value,
        version: record.value?.version ?? 0
      })
      if (requestId !== latestRecordRequestId) return null
      hydrateRecord(result)
      return result
    } finally {
      saving.value = false
    }
  }

  async function signMedicalRecordDraft() {
    const currentRecord = record.value
    if (!currentRecord?.id) throw new Error('请先保存病历草稿')
    if (dirty.value) throw new Error('病历存在未保存修改，请先保存')
    if (currentRecord.status !== 'DRAFT') throw new Error('只有草稿病历可以签名')
    if (signing.value) return null

    const requestId = latestRecordRequestId
    signing.value = true
    try {
      const result = await signInpatientMedicalRecord({
        id: currentRecord.id,
        version: currentRecord.version
      })
      if (requestId !== latestRecordRequestId) return null
      hydrateRecord(result)
      return result
    } finally {
      signing.value = false
    }
  }

  return {
    applyDocumentType,
    canSign,
    content,
    dirty,
    documentTypeCode,
    documentTypes,
    error,
    initialized,
    loadDocumentTypes,
    loadMedicalRecord,
    loading,
    readonly,
    record,
    resetMedicalRecord,
    saveMedicalRecordDraft,
    saving,
    signMedicalRecordDraft,
    signing,
    startNewMedicalRecord,
    title
  }
}
