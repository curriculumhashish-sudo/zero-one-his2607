<script setup lang="ts">
import EmptyState from '@/components/common/EmptyState.vue'
import RichTextEditorField from '@/components/common/RichTextEditorField.vue'
import type {
  ClinicalMedicalRecordDocumentTypeOption,
  ClinicalMedicalRecordStatusType
} from './type'

defineOptions({
  name: 'ClinicalMedicalRecordEditor'
})

withDefaults(
  defineProps<{
    /** 是否已选择患者。 */
    patientSelected?: boolean
    /** 是否已有可展示的病历上下文。 */
    initialized?: boolean
    /** 是否只读。 */
    readonly?: boolean
    /** 是否正在加载。 */
    loading?: boolean
    /** 加载错误。 */
    error?: string
    /** 病历是否有未保存修改。 */
    dirty?: boolean
    /** 编辑器标题。 */
    heading?: string
    /** 当前状态说明。 */
    statusText?: string
    /** 当前状态标签颜色。 */
    statusType?: ClinicalMedicalRecordStatusType
    /** 最近保存时间。 */
    updatedAt?: string
    /** 数据版本号。 */
    version?: number
    /** 是否显示文书类型和标题字段。 */
    showDocumentMeta?: boolean
    /** 文书类型是否已经锁定。 */
    documentTypeLocked?: boolean
    /** 文书类型选项。 */
    documentTypes?: ClinicalMedicalRecordDocumentTypeOption[]
    /** 当前文书类型编码。 */
    documentTypeCode?: string
    /** 已保存文书的类型名称。 */
    documentTypeName?: string
    /** 文书标题。 */
    title?: string
    /** 富文本正文。 */
    content?: string
    /** 富文本占位文字。 */
    placeholder?: string
    /** 富文本编辑区最小高度。 */
    minHeight?: string
    /** 未选择患者时的说明。 */
    patientEmptyDescription?: string
    /** 尚无可展示病历时的标题。 */
    inactiveTitle?: string
    /** 尚无可展示病历时的说明。 */
    inactiveDescription?: string
    /** 错误状态标题。 */
    errorTitle?: string
  }>(),
  {
    patientSelected: false,
    initialized: false,
    readonly: false,
    loading: false,
    error: '',
    dirty: false,
    heading: '病历',
    statusText: '',
    statusType: 'info',
    updatedAt: '',
    version: undefined,
    showDocumentMeta: false,
    documentTypeLocked: false,
    documentTypes: () => [],
    documentTypeCode: '',
    documentTypeName: '',
    title: '',
    content: '',
    placeholder: '请填写病历正文',
    minHeight: '360px',
    patientEmptyDescription: '从左侧患者队列选择患者后查看病历',
    inactiveTitle: '暂无可编辑病历',
    inactiveDescription: '请先完成当前诊疗环节',
    errorTitle: '病历加载失败'
  }
)

const emit = defineEmits<{
  'update:documentTypeCode': [value: string]
  'update:title': [value: string]
  'update:content': [value: string]
  documentTypeChange: [value: string]
  change: [value: string]
  retry: []
}>()

function handleDocumentTypeChange(value: string) {
  emit('update:documentTypeCode', value)
  emit('documentTypeChange', value)
}
</script>

<template>
  <section class="clinical-medical-record-editor">
    <EmptyState v-if="!patientSelected" title="请选择患者" :description="patientEmptyDescription" />

    <div v-else-if="loading" class="clinical-medical-record-editor__skeleton">
      <el-skeleton :rows="10" animated />
    </div>

    <EmptyState
      v-else-if="error"
      :title="errorTitle"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <EmptyState
      v-else-if="!initialized"
      :title="inactiveTitle"
      :description="inactiveDescription"
    />

    <div v-else class="clinical-medical-record-editor__body">
      <header class="clinical-medical-record-editor__header">
        <strong>{{ heading }}</strong>
        <div>
          <el-tag v-if="statusText" :type="statusType" effect="plain">{{ statusText }}</el-tag>
          <span v-if="updatedAt">最近保存：{{ updatedAt }}</span>
          <span v-if="version !== undefined">V{{ version }}</span>
        </div>
      </header>

      <div v-if="showDocumentMeta" class="clinical-medical-record-editor__meta">
        <label>
          <span>文书类型</span>
          <el-input
            v-if="documentTypeLocked"
            :model-value="documentTypeName"
            disabled
            aria-label="文书类型"
          />
          <el-select
            v-else
            :model-value="documentTypeCode"
            placeholder="请选择文书类型"
            aria-label="文书类型"
            @update:model-value="handleDocumentTypeChange"
          >
            <el-option
              v-for="item in documentTypes"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
        </label>

        <label class="clinical-medical-record-editor__title">
          <span>文书标题</span>
          <el-input
            :model-value="title"
            maxlength="80"
            show-word-limit
            :readonly="readonly"
            placeholder="请输入文书标题"
            @update:model-value="emit('update:title', $event)"
          />
        </label>
      </div>

      <RichTextEditorField
        :model-value="content"
        :readonly="readonly"
        :min-height="minHeight"
        :placeholder="placeholder"
        @update:model-value="emit('update:content', $event)"
        @change="emit('change', $event)"
      />
    </div>
  </section>
</template>

<style scoped>
.clinical-medical-record-editor {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  min-height: 100%;
  align-items: stretch;
  justify-content: center;
  background: var(--el-bg-color);
}

.clinical-medical-record-editor__skeleton,
.clinical-medical-record-editor__body {
  box-sizing: border-box;
  width: 100%;
  padding: 16px;
}

.clinical-medical-record-editor__header {
  display: flex;
  min-height: 30px;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  margin-bottom: 12px;
  color: var(--el-text-color-primary);
}

.clinical-medical-record-editor__header > div {
  display: flex;
  align-items: center;
  gap: 10px;
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.clinical-medical-record-editor__meta {
  display: flex;
  gap: 12px;
  margin-bottom: 12px;
}

.clinical-medical-record-editor__meta label {
  display: flex;
  width: 190px;
  flex-direction: column;
  gap: 6px;
  color: var(--el-text-color-regular);
  font-size: 13px;
}

.clinical-medical-record-editor__meta .clinical-medical-record-editor__title {
  min-width: 260px;
  flex: 1;
}

@media (max-width: 1100px) {
  .clinical-medical-record-editor__header,
  .clinical-medical-record-editor__meta {
    align-items: stretch;
    flex-direction: column;
  }

  .clinical-medical-record-editor__meta label {
    width: 100%;
  }
}
</style>
