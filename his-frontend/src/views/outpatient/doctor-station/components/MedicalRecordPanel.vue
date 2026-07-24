<script setup lang="ts">
import { computed } from 'vue'

import EmptyState from '@/components/common/EmptyState.vue'
import RichTextEditorField from '@/components/common/RichTextEditorField.vue'

defineOptions({
  name: 'MedicalRecordPanel'
})

const props = withDefaults(
  defineProps<{
    /** 富文本病历内容，由父页面保存 */
    modelValue?: string
    /** 是否已选择患者 */
    patientSelected?: boolean
    /** 患者是否已接诊，只有在诊患者可以编辑 */
    accepted?: boolean
    /** 是否只读，例如患者已经完诊 */
    readonly?: boolean
    /** 病历是否正在加载 */
    loading?: boolean
    /** 病历加载失败信息 */
    error?: string
    /** 页面内容是否与最近一次保存结果不同 */
    dirty?: boolean
    /** 最近一次保存时间 */
    updatedAt?: string
  }>(),
  {
    modelValue: '',
    patientSelected: false,
    accepted: false,
    readonly: false,
    loading: false,
    error: '',
    dirty: false,
    updatedAt: ''
  }
)

const emit = defineEmits<{
  'update:modelValue': [value: string]
  change: [value: string]
  retry: []
}>()

// 在诊和完诊患者都能查看病历，区别是完诊患者只能阅读。
const canDisplayRecord = computed(() => props.accepted || props.readonly)

function updateContent(value: string) {
  emit('update:modelValue', value)
}

function changeContent(value: string) {
  emit('change', value)
}
</script>

<template>
  <section class="medical-record-panel">
    <EmptyState
      v-if="!patientSelected"
      title="请选择患者"
      description="从左侧患者队列选择患者后，可查看本次门诊病历"
    />

    <!-- 骨架屏不会像遮罩层一样让编辑器文字变浅。 -->
    <div v-else-if="loading" class="medical-record-panel__skeleton">
      <el-skeleton :rows="10" animated />
    </div>

    <EmptyState
      v-else-if="error"
      title="病历加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <EmptyState
      v-else-if="!canDisplayRecord"
      title="请先接诊患者"
      description="接诊后才能编辑并保存本次门诊病历"
    />

    <div v-else class="medical-record-panel__editor">
      <div class="medical-record-panel__status">
        <div>
          <strong>门诊病历</strong>
          <span v-if="readonly" class="medical-record-panel__readonly">已完诊，只读</span>
          <span v-else-if="dirty" class="medical-record-panel__dirty">有未保存修改</span>
          <span v-else class="medical-record-panel__saved">内容已保存</span>
        </div>
        <span v-if="updatedAt" class="medical-record-panel__time"> 最近保存：{{ updatedAt }} </span>
      </div>

      <!--
        v-model 被拆成 model-value 和 update:model-value，便于新手看清数据流：
        父页面把内容传进来，编辑器变化后再通过事件把新内容传回父页面。
      -->
      <RichTextEditorField
        :model-value="modelValue"
        :readonly="readonly"
        min-height="360px"
        placeholder="请填写主诉、现病史、查体和处理意见，也可以从右侧套用病历模板"
        @update:model-value="updateContent"
        @change="changeContent"
      />
    </div>
  </section>
</template>

<style scoped>
.medical-record-panel {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  min-height: 100%;
  align-items: stretch;
  justify-content: center;
  background: var(--el-bg-color);
}

.medical-record-panel__skeleton {
  width: 100%;
  padding: 24px;
}

.medical-record-panel__editor {
  box-sizing: border-box;
  width: 100%;
  padding: 16px;
}

.medical-record-panel__status {
  display: flex;
  min-height: 28px;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  margin-bottom: 10px;
  color: var(--el-text-color-primary);
  font-size: 14px;
}

.medical-record-panel__readonly,
.medical-record-panel__dirty,
.medical-record-panel__saved {
  margin-left: 12px;
  font-size: 13px;
  font-weight: 400;
}

.medical-record-panel__readonly {
  color: var(--el-text-color-secondary);
}

.medical-record-panel__dirty {
  color: var(--el-color-warning);
}

.medical-record-panel__saved {
  color: var(--el-color-success);
}

.medical-record-panel__time {
  flex-shrink: 0;
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

@media (max-width: 1100px) {
  .medical-record-panel__status {
    align-items: flex-start;
    flex-direction: column;
    gap: 4px;
  }
}
</style>
