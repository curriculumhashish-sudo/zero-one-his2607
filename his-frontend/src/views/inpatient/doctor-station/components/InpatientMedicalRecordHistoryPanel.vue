<script setup lang="ts">
import { computed } from 'vue'

import type { InpatientMedicalRecordHistoryItemVO } from '@/apis/inpatient/doctor-station'
import RecordSidebar from '@/components/common/RecordSidebar.vue'
import type { RecordSidebarItem } from '@/components/common/type'

defineOptions({
  name: 'InpatientMedicalRecordHistoryPanel'
})

const props = withDefaults(
  defineProps<{
    rows?: InpatientMedicalRecordHistoryItemVO[]
    selectedId?: string | null
    loading?: boolean
    error?: string
    keyword?: string
    pageIndex?: number
    pageSize?: number
    total?: number
  }>(),
  {
    rows: () => [],
    selectedId: null,
    loading: false,
    error: '',
    keyword: '',
    pageIndex: 1,
    pageSize: 10,
    total: 0
  }
)

const emit = defineEmits<{
  'update:keyword': [value: string]
  search: [keyword: string]
  select: [record: InpatientMedicalRecordHistoryItemVO]
  pageChange: [pageIndex: number]
  retry: []
}>()

const sidebarItems = computed<RecordSidebarItem[]>(() =>
  props.rows.map((record) => ({
    key: record.id,
    title: `${record.documentTypeName} · ${record.title}`,
    subtitle: `${record.inpatientNo} · ${record.departmentName}`,
    description: `${record.authoredByName} · ${record.authoredAt}`,
    status: record.statusName
  }))
)

function handleSelect(item: RecordSidebarItem) {
  const record = props.rows.find((row) => row.id === item.key)
  if (record) emit('select', record)
}
</script>

<template>
  <RecordSidebar
    :model-value="selectedId"
    :items="sidebarItems"
    title="历史病历"
    width="100%"
    :loading="loading"
    :empty-description="error || '暂无历史病历'"
    :keyword="keyword"
    search-placeholder="标题/类型/住院号/医生"
    @update:keyword="emit('update:keyword', $event)"
    @search="emit('search', $event)"
    @select="handleSelect"
  >
    <template #footer>
      <div v-if="error" class="medical-record-history__error">
        <span>加载失败</span>
        <el-button link type="primary" @click="emit('retry')">重试</el-button>
      </div>

      <el-pagination
        v-else-if="total > pageSize"
        small
        background
        layout="prev, pager, next"
        :current-page="pageIndex"
        :page-size="pageSize"
        :total="total"
        @update:current-page="emit('pageChange', $event)"
      />

      <span v-else class="medical-record-history__total">共 {{ total }} 份病历</span>
    </template>
  </RecordSidebar>
</template>

<style scoped>
.medical-record-history__error {
  display: flex;
  align-items: center;
  justify-content: space-between;
  color: var(--el-color-danger);
  font-size: 13px;
}

.medical-record-history__total {
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.medical-record-history__error + :deep(.el-pagination) {
  display: none;
}

:deep(.el-pagination) {
  justify-content: center;
}
</style>
