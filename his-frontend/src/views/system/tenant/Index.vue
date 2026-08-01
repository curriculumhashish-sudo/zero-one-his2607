<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ElMessage, ElMessageBox, ElTag } from 'element-plus'
import type { ActionToolbarItem, RowActionItem } from '@/components/common/type'
import SearchPanel from '@/components/common/SearchPanel.vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import EditDialog from './components/edit-dialog.vue'
import UserSelectDialog from './components/user-select-dialog.vue'
import {
  addTenantUsers,
  pageTenantUsers,
  pageTenants,
  removeTenant,
  removeTenantUsers,
  type TenantRecord,
  type TenantUserRecord
} from '@/apis/system/tenant'

defineOptions({ name: 'TenantManagement' })

const TOOLBAR_ACTIONS: ActionToolbarItem[] = [{ key: 'create', label: '新增租户', type: 'primary' }]

const keyword = ref('')
const code = ref('')
const isActive = ref('')
const STATUS_OPTIONS = [
  { label: '启用', value: 'true' },
  { label: '禁用', value: 'false' }
]

const page = ref(1)
const pageSize = ref(10)
const rows = ref<TenantRecord[]>([])
const total = ref(0)
const loading = ref(false)

async function loadTenants() {
  loading.value = true
  try {
    const res = await pageTenants({
      page: page.value,
      pageSize: pageSize.value,
      code: code.value || undefined,
      keyword: keyword.value || undefined,
      isActive: isActive.value || undefined
    })
    rows.value = res.list
    total.value = res.total
  } catch {
    rows.value = []
    total.value = 0
  } finally {
    loading.value = false
  }
}

function handleSearch() {
  page.value = 1
  loadTenants()
}

function handleReset() {
  keyword.value = ''
  code.value = ''
  isActive.value = ''
  page.value = 1
  loadTenants()
}

function handleToolbarAction(action: ActionToolbarItem) {
  if (action.key === 'create') {
    openCreateDialog()
  }
}

const dialogVisible = ref(false)
const dialogFormData = ref<Partial<TenantRecord>>({})

function openCreateDialog() {
  dialogFormData.value = {}
  dialogVisible.value = true
}

function openEditDialog(row: TenantRecord) {
  dialogFormData.value = { ...row }
  dialogVisible.value = true
}

async function handleRowAction(action: RowActionItem, row: unknown) {
  const r = row as TenantRecord
  if (action.key === 'edit') {
    openEditDialog(r)
  } else if (action.key === 'delete') {
    try {
      await ElMessageBox.confirm(`确定删除租户「${r.name}」？`, '确认', { type: 'error' })
      await removeTenant(r.id)
      ElMessage.success('删除成功')
      loadTenants()
      if (selectedTenant.value?.id === r.id) {
        selectedTenant.value = null
        userList.value = []
      }
    } catch { /* cancelled */ }
  }
}

function buildRowActions(): RowActionItem[] {
  return [
    { key: 'edit', label: '编辑', type: 'primary' },
    { key: 'delete', label: '删除', type: 'danger' }
  ]
}

const selectedTenant = ref<TenantRecord | null>(null)
const userList = ref<TenantUserRecord[]>([])
const userTotal = ref(0)
const userLoading = ref(false)
const userPage = ref(1)
const userPageSize = ref(10)
const selectedUserIds = ref<string[]>([])

async function loadUserList() {
  if (!selectedTenant.value?.id) {
    userList.value = []
    userTotal.value = 0
    return
  }
  userLoading.value = true
  try {
    const res = await pageTenantUsers(selectedTenant.value.id, {
      page: userPage.value,
      pageSize: userPageSize.value
    })
    userList.value = res.list
    userTotal.value = res.total
  } catch {
    userList.value = []
    userTotal.value = 0
  } finally {
    userLoading.value = false
  }
}

function handleSelectTenant(row: unknown) {
  const r = row as TenantRecord
  selectedTenant.value = r
  selectedUserIds.value = []
  userPage.value = 1
  loadUserList()
}

const userSelectDialogVisible = ref(false)
const userSelectSelectedIds = ref<string[]>([])

function openAddUserDialog() {
  if (!selectedTenant.value) {
    ElMessage.warning('请先选择租户')
    return
  }
  userSelectSelectedIds.value = userList.value.map((u) => u.id)
  userSelectDialogVisible.value = true
}

async function handleAddUsers(users: TenantUserRecord[]) {
  if (!selectedTenant.value || users.length === 0) return
  try {
    await addTenantUsers({
      tenantId: selectedTenant.value.id,
      userIds: users.map((u) => u.id)
    })
    ElMessage.success(`成功添加 ${users.length} 个用户`)
    loadUserList()
  } catch (e: any) {
    ElMessage.error(e.message || '添加失败')
  }
}

async function handleRemoveUsers() {
  if (!selectedTenant.value || selectedUserIds.value.length === 0) {
    ElMessage.warning('请先选择要移除的用户')
    return
  }
  try {
    await ElMessageBox.confirm(`确定移除选中的 ${selectedUserIds.value.length} 个用户？`, '确认', { type: 'warning' })
    await removeTenantUsers({
      tenantId: selectedTenant.value.id,
      userIds: selectedUserIds.value
    })
    ElMessage.success('移除成功')
    selectedUserIds.value = []
    loadUserList()
  } catch { /* cancelled */ }
}

async function handleRemoveSingleUser(user: TenantUserRecord) {
  if (!selectedTenant.value) return
  try {
    await ElMessageBox.confirm(`确定移除用户「${user.name}」？`, '确认', { type: 'warning' })
    await removeTenantUsers({
      tenantId: selectedTenant.value.id,
      userIds: [user.id]
    })
    ElMessage.success('移除成功')
    loadUserList()
  } catch { /* cancelled */ }
}

function handleUserSelectionChange(selection: unknown[]) {
  selectedUserIds.value = (selection as TenantUserRecord[]).map((u) => u.id)
}

onMounted(() => {
  loadTenants()
})
</script>

<template>
  <div class="tenant-page">
    <div class="tenant-page__left">
      <SearchPanel
        v-model:keyword="keyword"
        :keyword-label="'租户名称/描述'"
        :keyword-placeholder="'请输入关键词'"
        v-model:dropdown-value="isActive"
        :dropdown-label="'状态'"
        :dropdown-options="STATUS_OPTIONS"
        :dropdown-placeholder="'全部'"
        :loading="loading"
        @search="handleSearch"
        @reset="handleReset"
      >
        <template #header>
          <div class="query-shell__field">
            <label class="query-shell__label">租户编码</label>
            <el-input
              v-model="code"
              placeholder="请输入编码"
              clearable
              size="default"
              @keyup.enter="handleSearch"
            />
          </div>
        </template>
      </SearchPanel>

      <ActionToolbar
        :actions="TOOLBAR_ACTIONS"
        @action="handleToolbarAction"
      />

      <DataTablePanel
        :data="rows"
        :loading="loading"
        :total="total"
        v-model:page="page"
        v-model:page-size="pageSize"
        show-index
        style="cursor: pointer"
        @row-click="handleSelectTenant"
      >
        <el-table-column prop="code" label="租户编码" width="120" />
        <el-table-column prop="name" label="租户名称" width="160" />
        <el-table-column prop="description" label="描述" min-width="160" show-overflow-tooltip />
        <el-table-column prop="isActive" label="状态" width="80" align="center">
          <template #default="{ row }">
            <el-tag :type="row.isActive === 'true' ? 'success' : 'info'">
              {{ row.isActive === 'true' ? '启用' : '禁用' }}
            </el-tag>
          </template>
        </el-table-column>
        <el-table-column prop="createdBy" label="创建人" width="100" />
        <el-table-column prop="createdAt" label="创建时间" width="160" />
        <el-table-column prop="updatedAt" label="更新时间" width="160" />
        <el-table-column label="操作" width="120" fixed="right" align="center">
          <template #default="{ row }">
            <RowActionGroup :actions="buildRowActions()" :row="row" @action="handleRowAction" />
          </template>
        </el-table-column>
      </DataTablePanel>
    </div>

    <div class="tenant-page__right">
      <div class="user-panel">
        <div class="user-panel__header">
          <span class="user-panel__title">
            {{ selectedTenant ? `${selectedTenant.name} - 用户列表` : '用户列表（请先选择租户）' }}
          </span>
        </div>
        <div v-if="selectedTenant" class="user-panel__body">
          <div class="user-panel__toolbar">
            <el-button type="success" size="small" @click="openAddUserDialog">+ 添加用户</el-button>
            <el-button type="danger" size="small" :disabled="selectedUserIds.length === 0" @click="handleRemoveUsers">移除选中</el-button>
          </div>
          <DataTablePanel
            :data="userList"
            :loading="userLoading"
            :total="userTotal"
            v-model:page="userPage"
            v-model:page-size="userPageSize"
            selectable
            show-index
            @selectable="handleUserSelectionChange"
          >
            <el-table-column label="当前租户" width="120">
              <template #default>{{ selectedTenant.name }}</template>
            </el-table-column>
            <el-table-column prop="userName" label="账号" width="120" />
            <el-table-column prop="name" label="姓名" width="100" />
            <el-table-column prop="email" label="邮箱" min-width="160" show-overflow-tooltip />
            <el-table-column prop="roleNames" label="角色" width="140">
              <template #default="{ row }">
                <el-tag v-for="r in row.roleNames" :key="r" size="small" style="margin-right: 4px">
                  {{ r }}
                </el-tag>
              </template>
            </el-table-column>
            <el-table-column label="操作" width="80" fixed="right" align="center">
              <template #default="{ row }">
                <el-button type="danger" link size="small" @click="handleRemoveSingleUser(row as TenantUserRecord)">移除</el-button>
              </template>
            </el-table-column>
          </DataTablePanel>
        </div>
        <el-empty v-else description="请在左侧选择一个租户查看其用户" :image-size="80" />
      </div>
    </div>

    <EditDialog
      v-model:visible="dialogVisible"
      :form-data="dialogFormData"
      @success="loadTenants"
    />

    <UserSelectDialog
      v-model:visible="userSelectDialogVisible"
      v-model:selected-ids="userSelectSelectedIds"
      :multiple="true"
      :show-input="false"
      placeholder="请选择要添加的用户"
      dialog-title="选择用户"
      @change="handleAddUsers"
    />
  </div>
</template>

<style scoped>
.tenant-page {
  display: flex;
  gap: 16px;
  height: calc(100vh - 120px);
  padding: 16px;
}

.tenant-page__left {
  flex: 2;
  display: flex;
  flex-direction: column;
  gap: 12px;
  min-width: 0;
}

.tenant-page__right {
  flex: 1;
  min-width: 380px;
  display: flex;
}

.user-panel {
  width: 100%;
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.user-panel__header {
  padding: 14px 16px;
  border-bottom: 1px solid #ebeef5;
}

.user-panel__title {
  font-size: 15px;
  font-weight: 600;
  color: var(--el-text-color-primary);
}

.user-panel__toolbar {
  display: flex;
  gap: 8px;
  padding: 12px 16px 0;
}

.user-panel__body {
  flex: 1;
  overflow: auto;
  padding: 0 16px 16px;
}

.tenant-page :deep(.table-panel) {
  padding: 12px;
}
</style>
