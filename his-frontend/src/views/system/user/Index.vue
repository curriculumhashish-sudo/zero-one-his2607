<template>
  <section class="user-page">
    <SearchPanel
      v-model:keyword="query.keyword"
      v-model:dropdown-value="query.enabled"
      keyword-placeholder="请输入用户名、姓名或手机号"
      dropdown-label="账号状态"
      dropdown-placeholder="全部状态"
      :dropdown-options="USER_ENABLED_OPTIONS"
      :field-count="1"
      :loading="loading"
      @search="handleSearch"
      @reset="handleReset"
    >
      <template #header>
        <div class="query-shell__field">
          <label class="query-shell__label">锁定状态</label>
          <el-select v-model="query.locked" clearable placeholder="全部" style="width: 100%">
            <el-option
              v-for="option in USER_LOCKED_OPTIONS"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </div>
      </template>
    </SearchPanel>

    <ActionToolbar
      title="用户列表"
      description="维护登录账号、角色和账号状态"
      :actions="USER_TOOLBAR_ACTIONS"
      :loading="loading"
      @action="handleToolbarAction"
    />

    <el-alert v-if="loadError" type="error" :title="loadError" show-icon :closable="false">
      <template #default>
        <el-button type="primary" link @click="loadUsers">重新加载</el-button>
      </template>
    </el-alert>

    <DataTablePanel
      :data="rows"
      :loading="loading"
      :total="total"
      :page="query.page"
      :page-size="query.pageSize"
      show-index
      min-height="360px"
      @update:page="handlePageChange"
      @update:page-size="handlePageSizeChange"
    >
      <el-table-column prop="username" label="用户名" min-width="120" fixed="left" />
      <el-table-column prop="realName" label="姓名" width="100" />
      <el-table-column label="角色" min-width="160" show-overflow-tooltip>
        <template #default="{ row }">{{ row.roleNames.join('、') || '-' }}</template>
      </el-table-column>
      <el-table-column prop="phone" label="手机号" width="130" />
      <el-table-column prop="email" label="邮箱" min-width="190" show-overflow-tooltip />
      <el-table-column label="账号状态" width="100" align="center">
        <template #default="{ row }">
          <StatusDisplay :value="row.enabled ? 'enabled' : 'disabled'" :options="USER_STATUS_MAP" />
        </template>
      </el-table-column>
      <el-table-column label="锁定状态" width="100" align="center">
        <template #default="{ row }">
          <StatusDisplay :value="row.locked ? 'locked' : 'unlocked'" :options="USER_LOCK_MAP" />
        </template>
      </el-table-column>
      <el-table-column prop="accessFailedCount" label="失败次数" width="90" align="center" />
      <el-table-column prop="lockoutEnd" label="锁定结束时间" width="170">
        <template #default="{ row }">{{ row.lockoutEnd || '-' }}</template>
      </el-table-column>
      <el-table-column prop="updatedAt" label="更新时间" width="170" />
      <el-table-column label="操作" width="250" fixed="right">
        <template #default="{ row }">
          <RowActionGroup
            :row="row"
            :actions="actionsFor(row)"
            :disabled="operationLoadingId === row.id"
            @action="handleRowAction"
          />
        </template>
      </el-table-column>
    </DataTablePanel>

    <UserEditDialog ref="editDialogRef" @saved="loadUsers" />
  </section>
</template>

<script setup lang="ts">
import { onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import SearchPanel from '@/components/common/SearchPanel.vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import type { ActionToolbarItem, RowActionItem } from '@/components/common/type'
import {
  changeUserPassword,
  pageUsers,
  removeUser,
  setUserEnabled,
  setUserLocked,
  unlockUser
} from '@/apis/system/user'
import type { UserRecord } from '@/apis/system/user'
import UserEditDialog from './components/UserEditDialog.vue'
import {
  USER_ENABLED_OPTIONS,
  USER_LOCKED_OPTIONS,
  USER_LOCK_MAP,
  USER_STATUS_MAP,
  USER_TOOLBAR_ACTIONS
} from './constants'
import type { UserEnabledFilter, UserLockedFilter } from './type'

const query = reactive({
  keyword: '',
  enabled: '' as UserEnabledFilter,
  locked: '' as UserLockedFilter,
  page: 1,
  pageSize: 10
})
const rows = ref<UserRecord[]>([])
const total = ref(0)
const loading = ref(false)
const loadError = ref('')
const operationLoadingId = ref('')
const editDialogRef = ref<InstanceType<typeof UserEditDialog> | null>(null)
let requestSequence = 0

async function loadUsers() {
  const currentSequence = ++requestSequence
  loading.value = true
  loadError.value = ''
  try {
    const result = await pageUsers({
      keyword: query.keyword,
      enabled: query.enabled === '' ? null : query.enabled === 'enabled',
      locked: query.locked === '' ? null : query.locked === 'locked',
      page: query.page,
      pageSize: query.pageSize
    })
    // 快速切换查询条件时，过期请求不能覆盖最新结果。
    if (currentSequence !== requestSequence) return
    rows.value = result.rows
    total.value = result.total
    if (query.page > 1 && rows.value.length === 0 && total.value > 0) {
      query.page -= 1
      await loadUsers()
    }
  } catch (error) {
    if (currentSequence !== requestSequence) return
    rows.value = []
    total.value = 0
    loadError.value = error instanceof Error ? error.message : '用户列表加载失败'
  } finally {
    if (currentSequence === requestSequence) loading.value = false
  }
}

function handleSearch() {
  query.page = 1
  void loadUsers()
}

function handleReset() {
  query.keyword = ''
  query.enabled = ''
  query.locked = ''
  query.page = 1
  void loadUsers()
}

function handlePageChange(page: number) {
  query.page = page
  void loadUsers()
}

function handlePageSizeChange(pageSize: number) {
  query.pageSize = pageSize
  query.page = 1
  void loadUsers()
}

function handleToolbarAction(action: ActionToolbarItem) {
  if (action.key === 'create') editDialogRef.value?.open()
  else if (action.key === 'refresh') void loadUsers()
}

function actionsFor(source: unknown): RowActionItem[] {
  const row = source as UserRecord
  const isAdmin = row.username.toLowerCase() === 'admin'
  const actions: RowActionItem[] = [
    { key: 'edit', label: '编辑', type: 'primary' },
    { key: 'change-password', label: '修改密码', type: 'warning' }
  ]
  if (row.locked) {
    actions.push({ key: 'unlock', label: '解锁', type: 'success', confirmText: '确认解锁该用户？' })
  } else {
    actions.push({
      key: 'lock',
      label: '锁定',
      type: 'warning',
      disabled: isAdmin,
      confirmText: '确认锁定该用户？锁定后将不能登录。'
    })
  }
  actions.push(
    row.enabled
      ? {
          key: 'disable',
          label: '停用',
          type: 'warning',
          disabled: isAdmin,
          confirmText: '确认停用该用户？停用后将不能登录。'
        }
      : { key: 'enable', label: '启用', type: 'success', confirmText: '确认启用该用户？' }
  )
  actions.push({
    key: 'delete',
    label: '删除',
    type: 'danger',
    disabled: isAdmin,
    confirmText: '确认删除该用户？删除后无法恢复。'
  })
  return actions
}

async function handleRowAction(action: RowActionItem, source: unknown) {
  const row = source as UserRecord
  if (!row?.id || operationLoadingId.value) return
  if (action.key === 'edit') {
    await editDialogRef.value?.open(row.id)
    return
  }

  if (action.key === 'change-password') {
    try {
      const { value } = await ElMessageBox.prompt('请输入新密码（至少 6 位）', '修改密码', {
        inputType: 'password',
        inputPattern: /^.{6,}$/,
        inputErrorMessage: '密码长度不能少于 6 位',
        confirmButtonText: '保存',
        cancelButtonText: '取消'
      })
      operationLoadingId.value = row.id
      await changeUserPassword(row.id, value)
      ElMessage.success('密码修改成功')
    } catch (error) {
      if (error !== 'cancel' && error !== 'close') {
        ElMessage.error(error instanceof Error ? error.message : '密码修改失败')
      }
    } finally {
      operationLoadingId.value = ''
    }
    return
  }

  operationLoadingId.value = row.id
  try {
    if (action.key === 'unlock') {
      await unlockUser(row.id)
      ElMessage.success('用户已解锁')
    } else if (action.key === 'enable' || action.key === 'disable') {
      const enabled = action.key === 'enable'
      await setUserEnabled(row.id, enabled)
      ElMessage.success(enabled ? '用户已启用' : '用户已停用')
    } else if (action.key === 'lock') {
      await setUserLocked(row.id, true)
      ElMessage.success('用户已锁定')
    } else if (action.key === 'delete') {
      await removeUser(row.id)
      ElMessage.success('用户已删除')
    }
    await loadUsers()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '用户操作失败')
  } finally {
    operationLoadingId.value = ''
  }
}

onMounted(() => void loadUsers())
</script>

<style scoped>
.user-page {
  display: flex;
  flex-direction: column;
  min-height: 100%;
  padding: 16px;
  gap: 14px;
  box-sizing: border-box;
}
</style>
