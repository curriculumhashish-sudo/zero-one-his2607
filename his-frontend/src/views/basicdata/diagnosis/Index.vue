<template>
  <section class="diagnosis-page">
    <SearchPanel
      v-model:keyword="query.keyword"
      keyword-label="关键词"
      keyword-placeholder="请输入诊断名称或编码"
      :loading="loading"
      @search="handleSearch"
      @reset="handleReset"
    />

    <ActionToolbar
      title="诊断字典"
      description="查询诊断编码、名称和类型"
      :actions="DIAGNOSIS_TOOLBAR_ACTIONS"
      :loading="loading"
      @action="handleToolbarAction"
    />

    <el-alert v-if="loadError" type="error" :title="loadError" show-icon :closable="false">
      <template #default>
        <el-button type="primary" link @click="loadData">重新加载</el-button>
      </template>
    </el-alert>

    <DataTablePanel
      :data="rows"
      :loading="loading"
      :total="total"
      :page="query.pageIndex"
      :page-size="query.pageSize"
      show-index
      row-key="id"
      min-height="420px"
      @update:page="handlePageChange"
      @update:page-size="handlePageSizeChange"
    >
      <el-table-column prop="code" label="诊断编码" width="160" />
      <el-table-column prop="name" label="诊断名称" min-width="260" />
      <el-table-column prop="typeName" label="诊断类型" min-width="160" />
    </DataTablePanel>
  </section>
</template>

<script setup lang="ts">
import { onMounted, reactive, ref } from 'vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import type { ActionToolbarItem } from '@/components/common/type'
import { listDiagnosis } from '@/apis/basicdata/diagnosis'
import type { DiagnosisRecord } from '@/apis/basicdata/diagnosis'
import { DIAGNOSIS_TOOLBAR_ACTIONS } from './constants'

const query = reactive({
  keyword: '',
  pageIndex: 1,
  pageSize: 10
})
const rows = ref<DiagnosisRecord[]>([])
const total = ref(0)
const loading = ref(false)
const loadError = ref('')
let requestSequence = 0

async function loadData() {
  const currentSequence = ++requestSequence
  loading.value = true
  loadError.value = ''
  try {
    const result = await listDiagnosis({
      keyword: query.keyword,
      pageIndex: query.pageIndex,
      pageSize: query.pageSize
    })
    if (currentSequence !== requestSequence) return
    rows.value = result.rows
    total.value = result.total
  } catch (error) {
    if (currentSequence !== requestSequence) return
    rows.value = []
    total.value = 0
    loadError.value = error instanceof Error ? error.message : '诊断字典加载失败'
  } finally {
    if (currentSequence === requestSequence) loading.value = false
  }
}

function handleSearch() {
  query.pageIndex = 1
  void loadData()
}

function handleReset() {
  query.keyword = ''
  query.pageIndex = 1
  void loadData()
}

function handlePageChange(page: number) {
  query.pageIndex = page
  void loadData()
}

function handlePageSizeChange(pageSize: number) {
  query.pageSize = pageSize
  query.pageIndex = 1
  void loadData()
}

function handleToolbarAction(action: ActionToolbarItem) {
  if (action.key === 'refresh') void loadData()
}

onMounted(() => void loadData())
</script>

<style scoped>
.diagnosis-page {
  display: flex;
  flex-direction: column;
  min-height: 100%;
  padding: 16px;
  gap: 14px;
  box-sizing: border-box;
}
</style>
