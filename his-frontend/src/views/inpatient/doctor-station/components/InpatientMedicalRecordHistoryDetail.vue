<script setup lang="ts">
import { computed } from 'vue'

import type { InpatientMedicalRecordHistoryDetailVO } from '@/apis/inpatient/doctor-station'
import EmptyState from '@/components/common/EmptyState.vue'

defineOptions({
  name: 'InpatientMedicalRecordHistoryDetail'
})

const props = withDefaults(
  defineProps<{
    patientSelected?: boolean
    detail?: InpatientMedicalRecordHistoryDetailVO | null
    loading?: boolean
    error?: string
    historyEmpty?: boolean
  }>(),
  {
    patientSelected: false,
    detail: null,
    loading: false,
    error: '',
    historyEmpty: false
  }
)

const emit = defineEmits<{
  retry: []
}>()

const previewDocument = computed(() => {
  const content = props.detail?.content ?? ''
  return `<!doctype html>
<html lang="zh-CN">
  <head>
    <meta charset="UTF-8" />
    <style>
      body {
        box-sizing: border-box;
        margin: 0;
        padding: 16px;
        color: #303133;
        font: 14px/1.8 Arial, "Microsoft YaHei", sans-serif;
      }
      h1, h2, h3, p { margin-top: 0; }
      table { width: 100%; border-collapse: collapse; }
      th, td { padding: 6px 8px; border: 1px solid #dcdfe6; }
    </style>
  </head>
  <body>${content}</body>
</html>`
})
</script>

<template>
  <section class="medical-record-history-detail">
    <EmptyState
      v-if="!patientSelected"
      title="请先选择患者"
      description="从左侧在院患者列表选择患者后查看历史病历"
    />

    <div v-else-if="loading" class="medical-record-history-detail__skeleton">
      <el-skeleton :rows="10" animated />
    </div>

    <EmptyState
      v-else-if="error"
      title="历史病历详情加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <EmptyState
      v-else-if="historyEmpty"
      title="暂无历史病历"
      description="当前患者暂时没有可查看的历史住院文书"
    />

    <EmptyState
      v-else-if="!detail"
      title="请选择历史病历"
      description="从右侧历史病历列表选择一份文书查看详情"
    />

    <div v-else class="medical-record-history-detail__content">
      <header class="medical-record-history-detail__header">
        <div>
          <strong>{{ detail.title }}</strong>
          <el-tag size="small" effect="plain">{{ detail.documentTypeName }}</el-tag>
          <el-tag size="small" type="info" effect="plain">{{ detail.statusName || detail.status }}</el-tag>
        </div>
      </header>

      <iframe
        class="medical-record-history-detail__preview"
        :srcdoc="previewDocument"
        sandbox=""
        title="历史病历正文"
      />
    </div>
  </section>
</template>

<style scoped>
.medical-record-history-detail {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  min-height: 100%;
  align-items: stretch;
  justify-content: center;
}

.medical-record-history-detail__skeleton,
.medical-record-history-detail__content {
  box-sizing: border-box;
  width: 100%;
  padding: 20px;
}

.medical-record-history-detail__header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  margin-bottom: 8px;
}

.medical-record-history-detail__header > div {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 8px;
}

.medical-record-history-detail__header > span,
.medical-record-history-detail__meta {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.medical-record-history-detail__meta {
  display: flex;
  flex-wrap: wrap;
  gap: 8px 20px;
  margin-bottom: 14px;
  padding-bottom: 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.medical-record-history-detail__preview {
  box-sizing: border-box;
  width: 100%;
  min-height: 460px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}
</style>
