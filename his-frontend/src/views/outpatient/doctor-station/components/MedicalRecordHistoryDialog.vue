<script setup lang="ts">
import { computed } from 'vue'

import type {
  DoctorMedicalRecordHistoryItemVO,
  DoctorMedicalRecordVO
} from '@/apis/outpatient/doctor-station'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import RichTextEditorField from '@/components/common/RichTextEditorField.vue'
import type { DetailSummaryItem } from '@/components/common/type'

defineOptions({
  name: 'MedicalRecordHistoryDialog'
})

const props = withDefaults(
  defineProps<{
    modelValue?: boolean
    item?: DoctorMedicalRecordHistoryItemVO | null
    record?: DoctorMedicalRecordVO | null
    loading?: boolean
    error?: string
  }>(),
  {
    modelValue: false,
    item: null,
    record: null,
    loading: false,
    error: ''
  }
)

const emit = defineEmits<{
  'update:modelValue': [visible: boolean]
  retry: []
}>()

const summaryItems = computed<DetailSummaryItem[]>(() => {
  if (!props.item) return []

  return [
    { key: 'visitDate', label: '就诊时间', value: props.item.visitDate },
    { key: 'department', label: '就诊科室', value: props.item.deptName },
    { key: 'doctor', label: '接诊医生', value: props.item.doctorName },
    { key: 'diagnosis', label: '主诊断', value: props.item.diagnosis, span: 2 },
    { key: 'updatedAt', label: '病历更新时间', value: props.record?.updatedAt }
  ]
})
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="历史门诊病历"
    width="min(900px, 92vw)"
    destroy-on-close
    append-to-body
    class="medical-record-history-dialog"
    @update:model-value="emit('update:modelValue', $event)"
  >
    <div v-if="loading" class="medical-record-history-dialog__skeleton">
      <el-skeleton :rows="12" animated />
    </div>

    <EmptyState
      v-else-if="error"
      title="历史病历正文加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <div v-else-if="item && record" class="medical-record-history-dialog__content">
      <DetailSummary title="就诊摘要" :items="summaryItems" :column="3" label-width="88px" />

      <section class="medical-record-history-dialog__record">
        <header>病历正文</header>
        <!-- Historical records are always read-only. -->
        <RichTextEditorField
          :model-value="record.content"
          readonly
          min-height="360px"
          placeholder="暂无病历正文"
        />
      </section>
    </div>

    <EmptyState v-else compact title="暂无历史病历详情" />

    <template #footer>
      <el-button @click="emit('update:modelValue', false)">关闭</el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.medical-record-history-dialog__skeleton {
  min-height: 420px;
  padding: 8px;
}

.medical-record-history-dialog__content {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 16px;
}

.medical-record-history-dialog__record {
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: #ffffff;
}

.medical-record-history-dialog__record > header {
  padding: 12px 16px;
  border-bottom: 1px solid var(--el-border-color-light);
  color: #303133;
  font-size: 15px;
  font-weight: 600;
}

.medical-record-history-dialog__record :deep(.rich-text-field),
.medical-record-history-dialog__record :deep(.w-e-text-container),
.medical-record-history-dialog__record :deep(.w-e-text-container p) {
  color: #303133 !important;
  background: #ffffff !important;
}
</style>
