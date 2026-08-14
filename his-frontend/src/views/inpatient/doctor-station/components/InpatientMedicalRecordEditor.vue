<script setup lang="ts">
import { computed } from 'vue'

import type {
  InpatientMedicalRecordDocumentTypeCode,
  InpatientMedicalRecordDocumentTypeOptionVO,
  InpatientMedicalRecordVO
} from '@/apis/inpatient/doctor-station'
import { ClinicalMedicalRecordEditor } from '@/components/clinical/medical-record'

defineOptions({
  name: 'InpatientMedicalRecordEditor'
})

const props = withDefaults(
  defineProps<{
    patientSelected?: boolean
    initialized?: boolean
    record?: InpatientMedicalRecordVO | null
    documentTypes?: InpatientMedicalRecordDocumentTypeOptionVO[]
    documentTypeCode?: InpatientMedicalRecordDocumentTypeCode | ''
    title?: string
    content?: string
    loading?: boolean
    error?: string
    dirty?: boolean
    readonly?: boolean
  }>(),
  {
    patientSelected: false,
    initialized: false,
    record: null,
    documentTypes: () => [],
    documentTypeCode: '',
    title: '',
    content: '',
    loading: false,
    error: '',
    dirty: false,
    readonly: false
  }
)

const emit = defineEmits<{
  'update:documentTypeCode': [value: InpatientMedicalRecordDocumentTypeCode]
  'update:title': [value: string]
  'update:content': [value: string]
  documentTypeChange: [value: Exclude<InpatientMedicalRecordDocumentTypeCode, 'PROGRESS_NOTE'>]
  retry: []
}>()

const recordStateText = computed(() => {
  if (!props.record) return '新建文书，尚未保存'
  if (props.dirty) return '草稿有未保存修改'
  return '已保存，只读'
})

const recordStateType = computed(() => {
  if (!props.record) return 'info'
  return props.dirty ? 'warning' : 'primary'
})

function handleDocumentTypeChange(value: string) {
  if (value === 'PROGRESS_NOTE') return
  const documentTypeCode = value as Exclude<InpatientMedicalRecordDocumentTypeCode, 'PROGRESS_NOTE'>
  emit('update:documentTypeCode', documentTypeCode)
  emit('documentTypeChange', documentTypeCode)
}
</script>

<template>
  <ClinicalMedicalRecordEditor
    :patient-selected="patientSelected"
    :initialized="initialized"
    :document-types="documentTypes"
    :document-type-code="documentTypeCode"
    :document-type-name="record?.documentTypeName"
    :document-type-locked="Boolean(record)"
    :title="title"
    :content="content"
    :loading="loading"
    :error="error"
    :dirty="dirty"
    :readonly="readonly"
    heading="住院病历"
    :status-text="recordStateText"
    :status-type="recordStateType"
    show-document-meta
    patient-empty-description="从左侧在院患者列表选择患者后书写住院病历"
    inactive-title="请选择或新建病历"
    inactive-description="从右侧历史病历中选择文书查看，或点击顶部“新建病历”开始书写"
    error-title="住院病历加载失败"
    placeholder="请填写住院病历正文"
    min-height="420px"
    @document-type-change="handleDocumentTypeChange"
    @update:title="emit('update:title', $event)"
    @update:content="emit('update:content', $event)"
    @retry="emit('retry')"
  />
</template>
