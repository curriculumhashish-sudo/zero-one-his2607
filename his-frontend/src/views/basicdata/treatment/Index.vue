<template>
  <section class="treatment-page">
    <SearchPanel
      v-model:keyword="query.keyword"
      v-model:dropdown-value="query.category"
      keyword-label="关键词"
      keyword-placeholder="请输入诊疗项目名称或编码"
      dropdown-label="诊疗类别"
      dropdown-placeholder="全部分类"
      :dropdown-options="TREATMENT_CATEGORY_FILTER"
      :field-count="1"
      :loading="loading"
      @search="loadData"
      @reset="handleReset"
    >
      <template #header>
        <div class="query-shell__field">
          <label class="query-shell__label">启用状态</label>
          <el-select v-model="query.enabled" clearable placeholder="全部状态" style="width: 100%">
            <el-option
              v-for="option in TREATMENT_ENABLED_OPTIONS"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </div>
      </template>
    </SearchPanel>

    <ActionToolbar
      title="诊疗管理"
      description="维护诊疗项目基础信息、计价单位与价格"
      :actions="TREATMENT_TOOLBAR_ACTIONS"
      :selection-count="selectedRows.length"
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
      selectable
      show-index
      row-key="id"
      min-height="420px"
      @update:page="handlePageChange"
      @update:page-size="handlePageSizeChange"
      @selectable="handleSelectionChange"
    >
      <el-table-column prop="code" label="诊疗编码" width="120" />
      <el-table-column prop="name" label="诊疗项目名称" min-width="180" />
      <el-table-column prop="pyCode" label="拼音码" width="110" align="center" />
      <el-table-column prop="categoryName" label="诊疗类别" width="100" align="center" />
      <el-table-column prop="unit" label="计价单位" width="100" align="center" />
      <el-table-column label="价格" width="110" align="right">
        <template #default="{ row }">
          <span class="treatment-price">¥{{ formatPrice(row.price) }}</span>
        </template>
      </el-table-column>
      <el-table-column label="状态" width="80" align="center">
        <template #default="{ row }">
          <StatusDisplay
            :value="row.enabled"
            :options="TREATMENT_STATUS_MAP"
          />
        </template>
      </el-table-column>
      <el-table-column prop="sort" label="排序号" width="90" align="center" />
      <el-table-column prop="updatedAt" label="更新时间" width="170" />
      <el-table-column label="操作" width="220">
        <template #default="{ row }">
          <RowActionGroup
            :row="row"
            :actions="actionsFor(row)"
            :disabled="operationLoadingId === row.id"
            :max-visible="4"
            @action="handleRowAction"
          />
        </template>
      </el-table-column>
    </DataTablePanel>

    <TreatmentEditDialog ref="editDialogRef" @saved="loadData" />
  </section>
</template>

<script setup lang="ts">
import { onMounted, reactive, ref } from 'vue'
import { ElMessage } from 'element-plus'
import SearchPanel from '@/components/common/SearchPanel.vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import type { ActionToolbarItem, RowActionItem } from '@/components/common/type'
import {
  listTreatment,
  removeTreatment,
  setTreatmentEnabled
} from '@/apis/basicdata/treatment'
import type { TreatmentRecord } from '@/apis/basicdata/treatment'
import TreatmentEditDialog from './components/TreatmentEditDialog.vue'
import {
  TREATMENT_CATEGORY_FILTER,
  TREATMENT_ENABLED_OPTIONS,
  TREATMENT_STATUS_MAP,
  TREATMENT_TOOLBAR_ACTIONS
} from './constants'
import type { TreatmentEnabledFilter } from './type'

const query = reactive({
  keyword: '',
  category: '',
  enabled: '' as TreatmentEnabledFilter,
  pageIndex: 1,
  pageSize: 10
})
const rows = ref<TreatmentRecord[]>([])
const total = ref(0)
const loading = ref(false)
const loadError = ref('')
const operationLoadingId = ref('')
const selectedRows = ref<TreatmentRecord[]>([])
const editDialogRef = ref<InstanceType<typeof TreatmentEditDialog> | null>(null)
let requestSequence = 0

function formatPrice(price: number): string {
  return price.toFixed(2)
}

async function loadData() {
  const currentSequence = ++requestSequence
  loading.value = true
  loadError.value = ''
  try {
    const result = await listTreatment({
      keyword: query.keyword,
      category: query.category,
      enabled: query.enabled === '' ? null : query.enabled === 'enabled',
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
    loadError.value = error instanceof Error ? error.message : '诊疗项目加载失败'
  } finally {
    if (currentSequence === requestSequence) loading.value = false
  }
}

function handleReset() {
  query.keyword = ''
  query.category = ''
  query.enabled = ''
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

function handleSelectionChange(selection: TreatmentRecord[]) {
  selectedRows.value = selection
}

function handleToolbarAction(action: ActionToolbarItem) {
  if (action.key === 'create') {
    editDialogRef.value?.open()
  } else if (action.key === 'refresh') {
    void loadData()
  } else if (action.key === 'batchEnable' || action.key === 'batchDisable' || action.key === 'batchDelete') {
    void handleBatchAction(action.key)
  } else if (action.key === 'import' || action.key === 'export') {
    ElMessage.info(`${action.label}功能开发中`)
  }
}

async function handleBatchAction(key: string) {
  if (!selectedRows.value.length) {
    ElMessage.warning('请先选择需要操作的诊疗项目')
    return
  }
  const ids = selectedRows.value.map((row) => row.id)
  loading.value = true
  try {
    if (key === 'batchDelete') {
      await Promise.all(ids.map((id) => removeTreatment(id)))
      ElMessage.success(`已删除 ${ids.length} 条诊疗项目记录`)
    } else {
      const enabled = key === 'batchEnable'
      await Promise.all(ids.map((id) => setTreatmentEnabled(id, enabled)))
      ElMessage.success(`已${enabled ? '启用' : '停用'} ${ids.length} 条诊疗项目记录`)
    }
    selectedRows.value = []
    await loadData()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '批量操作失败')
  } finally {
    loading.value = false
  }
}

function actionsFor(source: unknown): RowActionItem[] {
  const row = source as TreatmentRecord
  return [
    { key: 'edit', label: '编辑', type: 'primary' },
    row.enabled
      ? { key: 'disable', label: '停用', type: 'warning', confirmText: '确认停用该诊疗项目？' }
      : { key: 'enable', label: '启用', type: 'success', confirmText: '确认启用该诊疗项目？' },
    {
      key: 'delete',
      label: '删除',
      type: 'danger',
      confirmText: '确认删除该诊疗项目？删除后无法恢复。'
    }
  ]
}

async function handleRowAction(action: RowActionItem, source: unknown) {
  const row = source as TreatmentRecord
  if (!row?.id || operationLoadingId.value) return
  if (action.key === 'edit') {
    await editDialogRef.value?.open({ id: row.id })
    return
  }

  operationLoadingId.value = row.id
  try {
    if (action.key === 'delete') {
      await removeTreatment(row.id)
      ElMessage.success('诊疗项目已删除')
    } else if (action.key === 'enable' || action.key === 'disable') {
      const enabled = action.key === 'enable'
      await setTreatmentEnabled(row.id, enabled)
      ElMessage.success(enabled ? '诊疗项目已启用' : '诊疗项目已停用')
    }
    await loadData()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '诊疗项目操作失败')
  } finally {
    operationLoadingId.value = ''
  }
}

onMounted(() => void loadData())
</script>

<style scoped>
.treatment-page {
  display: flex;
  flex-direction: column;
  min-height: 100%;
  padding: 16px;
  gap: 14px;
  box-sizing: border-box;
}

.treatment-price {
  font-weight: 600;
  color: var(--el-color-primary);
}
</style>
