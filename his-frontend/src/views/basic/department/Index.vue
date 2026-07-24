<template>
  <section class="department-page">
    <SearchPanel
      v-model:keyword="query.keyword"
      v-model:dropdown-value="query.type"
      keyword-placeholder="请输入科室名称或编码"
      dropdown-label="科室类型"
      dropdown-placeholder="全部类型"
      :dropdown-options="DEPARTMENT_TYPE_OPTIONS"
      :field-count="2"
      :loading="loading"
      @search="loadDepartments"
      @reset="handleReset"
    >
      <template #header>
        <div class="query-shell__field">
          <label class="query-shell__label">科室类别</label>
          <el-select v-model="query.category" clearable placeholder="全部类别" style="width: 100%">
            <el-option
              v-for="option in DEPARTMENT_CATEGORY_OPTIONS"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">科室状态</label>
          <el-select v-model="query.enabled" clearable placeholder="全部状态" style="width: 100%">
            <el-option
              v-for="option in DEPARTMENT_ENABLED_OPTIONS"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </div>
      </template>
    </SearchPanel>

    <ActionToolbar
      title="科室组织树"
      description="按层级维护医院、临床、医技和行政科室"
      :actions="DEPARTMENT_TOOLBAR_ACTIONS"
      :loading="loading"
      @action="handleToolbarAction"
    />

    <el-alert v-if="loadError" type="error" :title="loadError" show-icon :closable="false">
      <template #default>
        <el-button type="primary" link @click="loadDepartments">重新加载</el-button>
      </template>
    </el-alert>

    <DataTablePanel
      :data="rows"
      :loading="loading"
      :total="0"
      row-key="id"
      default-expand-all
      :tree-props="{ children: 'children' }"
      min-height="360px"
    >
      <el-table-column prop="name" label="科室名称" min-width="220" fixed="left" />
      <el-table-column prop="code" label="科室编码" width="110" />
      <el-table-column prop="type" label="类型" width="110" />
      <el-table-column prop="category" label="类别" width="90" />
      <el-table-column prop="leaderName" label="负责人" width="110">
        <template #default="{ row }">{{ row.leaderName || '-' }}</template>
      </el-table-column>
      <el-table-column prop="sort" label="排序" width="80" align="center" />
      <el-table-column label="状态" width="90" align="center">
        <template #default="{ row }">
          <StatusDisplay
            :value="row.enabled ? 'enabled' : 'disabled'"
            :options="DEPARTMENT_STATUS_MAP"
          />
        </template>
      </el-table-column>
      <el-table-column prop="updatedAt" label="更新时间" width="170" />
      <el-table-column label="操作" width="300" fixed="right">
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

    <DepartmentEditDialog ref="editDialogRef" @saved="loadDepartments" />
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
import { listDepartments, removeDepartment, setDepartmentEnabled } from '@/apis/basic/department'
import type { DepartmentRecord } from '@/apis/basic/department'
import DepartmentEditDialog from './components/DepartmentEditDialog.vue'
import {
  DEPARTMENT_CATEGORY_OPTIONS,
  DEPARTMENT_ENABLED_OPTIONS,
  DEPARTMENT_STATUS_MAP,
  DEPARTMENT_TOOLBAR_ACTIONS,
  DEPARTMENT_TYPE_OPTIONS
} from './constants'
import type { DepartmentEnabledFilter } from './type'

const query = reactive({
  keyword: '',
  type: '',
  category: '',
  enabled: '' as DepartmentEnabledFilter
})
const rows = ref<DepartmentRecord[]>([])
const loading = ref(false)
const loadError = ref('')
const operationLoadingId = ref('')
const editDialogRef = ref<InstanceType<typeof DepartmentEditDialog> | null>(null)
let requestSequence = 0

async function loadDepartments() {
  const currentSequence = ++requestSequence
  loading.value = true
  loadError.value = ''
  try {
    const result = await listDepartments({
      keyword: query.keyword,
      type: query.type,
      category: query.category,
      enabled: query.enabled === '' ? null : query.enabled === 'enabled'
    })
    // 树形筛选也要防止慢请求覆盖用户刚刚提交的新查询。
    if (currentSequence !== requestSequence) return
    rows.value = result
  } catch (error) {
    if (currentSequence !== requestSequence) return
    rows.value = []
    loadError.value = error instanceof Error ? error.message : '科室列表加载失败'
  } finally {
    if (currentSequence === requestSequence) loading.value = false
  }
}

function handleReset() {
  query.keyword = ''
  query.type = ''
  query.category = ''
  query.enabled = ''
  void loadDepartments()
}

function handleToolbarAction(action: ActionToolbarItem) {
  if (action.key === 'create') editDialogRef.value?.open()
  else if (action.key === 'refresh') void loadDepartments()
}

function actionsFor(source: unknown): RowActionItem[] {
  const row = source as DepartmentRecord
  return [
    { key: 'createChild', label: '新增下级', type: 'primary' },
    { key: 'edit', label: '编辑', type: 'primary' },
    row.enabled
      ? { key: 'disable', label: '停用', type: 'warning', confirmText: '确认停用该科室？' }
      : { key: 'enable', label: '启用', type: 'success', confirmText: '确认启用该科室？' },
    {
      key: 'delete',
      label: '删除',
      type: 'danger',
      disabled: Boolean(row.children?.length),
      confirmText: '确认删除该科室？删除后无法恢复。'
    }
  ]
}

async function handleRowAction(action: RowActionItem, source: unknown) {
  const row = source as DepartmentRecord
  if (!row?.id || operationLoadingId.value) return
  if (action.key === 'createChild') {
    await editDialogRef.value?.open({ parentId: row.id })
    return
  }
  if (action.key === 'edit') {
    await editDialogRef.value?.open({ id: row.id })
    return
  }

  operationLoadingId.value = row.id
  try {
    if (action.key === 'delete') {
      await removeDepartment(row.id)
      ElMessage.success('科室已删除')
    } else if (action.key === 'enable' || action.key === 'disable') {
      const enabled = action.key === 'enable'
      await setDepartmentEnabled(row.id, enabled)
      ElMessage.success(enabled ? '科室已启用' : '科室已停用')
    }
    await loadDepartments()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '科室操作失败')
  } finally {
    operationLoadingId.value = ''
  }
}

onMounted(() => void loadDepartments())
</script>

<style scoped>
.department-page {
  display: flex;
  flex-direction: column;
  min-height: 100%;
  padding: 16px;
  gap: 14px;
  box-sizing: border-box;
}
</style>
