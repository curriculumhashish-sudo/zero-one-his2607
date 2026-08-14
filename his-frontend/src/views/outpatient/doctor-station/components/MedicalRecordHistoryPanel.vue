<script setup lang="ts">
import { computed } from 'vue'

import type { DoctorMedicalRecordHistoryItemVO } from '@/apis/outpatient/doctor-station'
import EmptyState from '@/components/common/EmptyState.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import RecordSidebar from '@/components/common/RecordSidebar.vue'
import type { RecordSidebarItem } from '@/components/common/type'

defineOptions({
  name: 'MedicalRecordHistoryPanel'
})

const props = withDefaults(
  defineProps<{
    patientSelected?: boolean
    rows?: DoctorMedicalRecordHistoryItemVO[]
    keyword?: string
    page?: number
    pageSize?: number
    total?: number
    loading?: boolean
    error?: string
  }>(),
  {
    patientSelected: false,
    rows: () => [],
    keyword: '',
    page: 1,
    pageSize: 10,
    total: 0,
    loading: false,
    error: ''
  }
)

const emit = defineEmits<{
  'update:keyword': [keyword: string]
  search: [keyword: string]
  pageChange: [payload: { page: number; pageSize: number }]
  select: [item: DoctorMedicalRecordHistoryItemVO]
  retry: []
}>()

// Map the business summary to the generic sidebar item shape.
const sidebarItems = computed<RecordSidebarItem[]>(() =>
  props.rows.map((item) => ({
    key: item.encounterId,
    title: item.visitDate,
    subtitle: [item.deptName, item.doctorName].filter(Boolean).join(' · '),
    description: item.diagnosis ? `主诊断：${item.diagnosis}` : '暂无主诊断'
  }))
)

function selectRecord(sidebarItem: RecordSidebarItem) {
  const historyItem = props.rows.find((item) => item.encounterId === sidebarItem.key)
  if (historyItem) emit('select', historyItem)
}
</script>

<template>
  <section class="medical-record-history-panel">
    <EmptyState
      v-if="!patientSelected"
      compact
      title="请先选择患者"
      description="选择患者后可查看历史门诊病历"
    />

    <!-- 使用骨架屏替代半透明 loading 遮罩，Chrome 中的文字不会被整体淡化。 -->
    <div v-else-if="loading" class="medical-record-history-panel__skeleton">
      <el-skeleton :rows="8" animated />
    </div>

    <EmptyState
      v-else-if="error"
      compact
      title="历史病历加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <RecordSidebar
      v-else
      :items="sidebarItems"
      :keyword="keyword"
      title="历史病历"
      width="100%"
      empty-description="暂无符合条件的历史病历"
      search-placeholder="科室/医生/主诊断"
      @update:keyword="emit('update:keyword', $event)"
      @search="emit('search', $event)"
      @select="selectRecord"
    >
      <template #footer>
        <PaginationBar
          :page="page"
          :page-size="pageSize"
          :total="total"
          :layout="['total', 'prev', 'pager', 'next']"
          :pager-count="5"
          small
          @change="emit('pageChange', $event)"
        />
      </template>
    </RecordSidebar>
  </section>
</template>

<style scoped>
.medical-record-history-panel {
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  color: #303133;
  background: var(--el-bg-color);
}

.medical-record-history-panel__skeleton {
  box-sizing: border-box;
  height: 100%;
  padding: 16px 12px;
  background: #ffffff;
}

.medical-record-history-panel :deep(.side-panel-shell) {
  width: 100% !important;
  border: 0;
}

.medical-record-history-panel :deep(.record-sidebar__item-title) {
  color: #303133;
}

.medical-record-history-panel :deep(.record-sidebar__item-subtitle) {
  color: #606266;
}

.medical-record-history-panel :deep(.record-sidebar__item-description) {
  color: #606266;
}

.medical-record-history-panel :deep(.side-panel-shell__footer) {
  padding: 8px;
}
</style>
