<script setup lang="ts">
import { ref, watch } from 'vue'

import type { DoctorPatientDiagnosisHistoryItemVO } from '@/apis/outpatient/doctor-station'
import EmptyState from '@/components/common/EmptyState.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'

defineOptions({ name: 'DiagnosisHistoryDrawer' })

const props = withDefaults(
  defineProps<{
    modelValue?: boolean
    rows?: DoctorPatientDiagnosisHistoryItemVO[]
    total?: number
    loading?: boolean
    error?: string
    page?: number
    pageSize?: number
    keyword?: string
    dateRange?: [string, string] | []
  }>(),
  {
    modelValue: false,
    rows: () => [],
    total: 0,
    loading: false,
    error: '',
    page: 1,
    pageSize: 10,
    keyword: '',
    dateRange: () => []
  }
)

const emit = defineEmits<{
  'update:modelValue': [visible: boolean]
  queryChange: [
    patch: Partial<{
      pageIndex: number
      pageSize: number
      keyword: string
      dateRange: [string, string] | []
    }>,
    resetPage?: boolean
  ]
  retry: []
}>()

const keywordInput = ref(props.keyword)

watch(
  () => props.keyword,
  (value) => {
    keywordInput.value = value
  }
)

function submitSearch() {
  emit('queryChange', { keyword: keywordInput.value }, true)
}

function resetFilters() {
  keywordInput.value = ''
  emit('queryChange', { keyword: '', dateRange: [] }, true)
}
</script>

<template>
  <el-drawer
    :model-value="modelValue"
    title="患者历史诊断"
    size="82%"
    destroy-on-close
    @update:model-value="emit('update:modelValue', $event)"
  >
    <section class="diagnosis-history">
      <div class="diagnosis-history__filters">
        <el-input
          v-model="keywordInput"
          clearable
          placeholder="就诊号、科室、医生、诊断编码或诊断名称"
          @keyup.enter="submitSearch"
        />
        <el-date-picker
          :model-value="dateRange"
          type="daterange"
          value-format="YYYY-MM-DD"
          range-separator="至"
          start-placeholder="开始日期"
          end-placeholder="结束日期"
          @update:model-value="emit('queryChange', { dateRange: $event || [] }, true)"
        />
        <el-button type="primary" @click="submitSearch">查询</el-button>
        <el-button @click="resetFilters">重置</el-button>
      </div>

      <div v-if="loading" class="diagnosis-history__skeleton">
        <el-skeleton :rows="10" animated />
      </div>

      <EmptyState
        v-else-if="error"
        title="历史诊断加载失败"
        :description="error"
        action-text="重试"
        @action="emit('retry')"
      />

      <template v-else>
        <el-table :data="rows" row-key="id" border stripe class="diagnosis-history__table">
          <el-table-column prop="visitedAt" label="就诊时间" width="156" />
          <el-table-column prop="visitNo" label="就诊号" min-width="150" />
          <el-table-column prop="departmentName" label="科室" min-width="120" />
          <el-table-column prop="attendingDoctorName" label="接诊医生" width="110" />
          <el-table-column label="主次" width="92" align="center">
            <template #default="{ row }">
              <el-tag :type="row.primary ? 'danger' : 'info'" effect="light">
                {{ row.primary ? '主诊断' : '次诊断' }}
              </el-tag>
            </template>
          </el-table-column>
          <el-table-column prop="code" label="诊断编码" width="120" />
          <el-table-column prop="name" label="诊断名称" min-width="190" />
          <el-table-column prop="typeName" label="诊断分类" width="110" />
          <template #empty>
            <el-empty description="当前条件下暂无历史诊断" :image-size="88" />
          </template>
        </el-table>

        <PaginationBar
          :page="page"
          :page-size="pageSize"
          :total="total"
          :page-sizes="[10, 20, 50]"
          :layout="['total', 'sizes', 'prev', 'pager', 'next']"
          @change="
            emit('queryChange', { pageIndex: $event.page, pageSize: $event.pageSize }, false)
          "
        />
      </template>
    </section>
  </el-drawer>
</template>

<style scoped>
.diagnosis-history {
  display: flex;
  min-width: 0;
  height: calc(100vh - 120px);
  flex-direction: column;
  gap: 14px;
  color: #303133;
}

.diagnosis-history__filters {
  display: flex;
  align-items: center;
  flex-wrap: wrap;
  gap: 8px;
}

.diagnosis-history__filters > :deep(.el-input) {
  width: 300px;
}

.diagnosis-history__filters > :deep(.el-select) {
  width: 138px;
}

.diagnosis-history__filters > :deep(.el-date-editor) {
  width: 250px;
}

.diagnosis-history__skeleton {
  padding: 16px;
}

.diagnosis-history__table {
  --el-table-text-color: #303133;
  --el-table-header-text-color: #606266;
  --el-table-header-bg-color: #f5f7fa;
  width: 100%;
}

.diagnosis-history__table :deep(.el-table__cell),
.diagnosis-history__table :deep(.cell) {
  color: #303133;
}
</style>
