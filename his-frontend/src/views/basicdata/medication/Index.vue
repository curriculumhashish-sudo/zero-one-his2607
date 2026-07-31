<template>
  <section class="medication-page">
    <SearchPanel
      v-model:keyword="query.keyword"
      v-model:dropdown-value="query.category"
      keyword-label="关键词"
      keyword-placeholder="请输入药品名称或编码"
      dropdown-label="药品类别"
      dropdown-placeholder="全部分类"
      :dropdown-options="MEDICATION_CATEGORY_FILTER"
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
              v-for="option in MEDICATION_ENABLED_OPTIONS"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </div>
      </template>
    </SearchPanel>

    <ActionToolbar
      title="药品管理"
      description="维护药品基础信息、类别、规格、剂型与价格"
      :actions="MEDICATION_TOOLBAR_ACTIONS"
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
      <el-table-column prop="code" label="药品编码" width="120" />
      <el-table-column prop="name" label="药品名称" min-width="160" />
      <el-table-column prop="genericName" label="通用名" min-width="130" />
      <el-table-column prop="categoryName" label="类别" width="90" align="center" />
      <el-table-column prop="specification" label="规格" min-width="140" />
      <el-table-column prop="dosageFormName" label="剂型" width="90" align="center" />
      <el-table-column prop="unit" label="单位" width="70" align="center" />
      <el-table-column prop="manufacturer" label="生产厂家" min-width="180" />
      <el-table-column label="零售价" width="110" align="right">
        <template #default="{ row }">
          <span class="medication-page__price">¥{{ formatPrice(row.retailPrice) }}</span>
        </template>
      </el-table-column>
      <el-table-column prop="sort" label="排序号" width="80" align="center" />
      <el-table-column label="状态" width="80" align="center">
        <template #default="{ row }">
          <StatusDisplay
            :value="row.enabled"
            :options="MEDICATION_STATUS_MAP"
          />
        </template>
      </el-table-column>
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

    <MedicationEditDialog ref="editDialogRef" @saved="loadData" />
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
  listMedication,
  removeMedication,
  setMedicationEnabled
} from '@/apis/basicdata/medication'
import type { MedicationRecord } from '@/apis/basicdata/medication'
import MedicationEditDialog from './components/MedicationEditDialog.vue'
import {
  MEDICATION_CATEGORY_FILTER,
  MEDICATION_ENABLED_OPTIONS,
  MEDICATION_STATUS_MAP,
  MEDICATION_TOOLBAR_ACTIONS
} from './constants'
import type { MedicationEnabledFilter } from './type'

const query = reactive({
  keyword: '',
  category: '',
  enabled: '' as MedicationEnabledFilter,
  pageIndex: 1,
  pageSize: 10
})
const rows = ref<MedicationRecord[]>([])
const total = ref(0)
const loading = ref(false)
const loadError = ref('')
const operationLoadingId = ref('')
const selectedRows = ref<MedicationRecord[]>([])
const editDialogRef = ref<InstanceType<typeof MedicationEditDialog> | null>(null)
let requestSequence = 0

function formatPrice(value: number): string {
  return value.toFixed(2)
}

async function loadData() {
  const currentSequence = ++requestSequence
  loading.value = true
  loadError.value = ''
  try {
    const result = await listMedication({
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
    loadError.value = error instanceof Error ? error.message : '药品列表加载失败'
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

function handleSelectionChange(selection: MedicationRecord[]) {
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
    ElMessage.warning('请先选择需要操作的药品')
    return
  }
  const ids = selectedRows.value.map((row) => row.id)
  loading.value = true
  try {
    if (key === 'batchDelete') {
      await Promise.all(ids.map((id) => removeMedication(id)))
      ElMessage.success(`已删除 ${ids.length} 条药品记录`)
    } else {
      const enabled = key === 'batchEnable'
      await Promise.all(ids.map((id) => setMedicationEnabled(id, enabled)))
      ElMessage.success(`已${enabled ? '启用' : '停用'} ${ids.length} 条药品记录`)
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
  const row = source as MedicationRecord
  return [
    { key: 'edit', label: '编辑', type: 'primary' },
    row.enabled
      ? { key: 'disable', label: '停用', type: 'warning', confirmText: '确认停用该药品？' }
      : { key: 'enable', label: '启用', type: 'success', confirmText: '确认启用该药品？' },
    {
      key: 'delete',
      label: '删除',
      type: 'danger',
      confirmText: '确认删除该药品？删除后无法恢复。'
    }
  ]
}

async function handleRowAction(action: RowActionItem, source: unknown) {
  const row = source as MedicationRecord
  if (!row?.id || operationLoadingId.value) return
  if (action.key === 'edit') {
    await editDialogRef.value?.open({ id: row.id })
    return
  }

  operationLoadingId.value = row.id
  try {
    if (action.key === 'delete') {
      await removeMedication(row.id)
      ElMessage.success('药品已删除')
    } else if (action.key === 'enable' || action.key === 'disable') {
      const enabled = action.key === 'enable'
      await setMedicationEnabled(row.id, enabled)
      ElMessage.success(enabled ? '药品已启用' : '药品已停用')
    }
    await loadData()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '药品操作失败')
  } finally {
    operationLoadingId.value = ''
  }
}

onMounted(() => void loadData())
</script>

<style scoped>
.medication-page {
  display: flex;
  flex-direction: column;
  min-height: 100%;
  padding: 16px;
  gap: 14px;
  box-sizing: border-box;
}

.medication-page__price {
  font-variant-numeric: tabular-nums;
  font-weight: 500;
}
</style>
