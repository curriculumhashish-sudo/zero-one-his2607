<script setup lang="ts">
import { ref, watch } from 'vue'
import { searchAvailableUsers, type TenantUserRecord } from '@/apis/system/tenant'

interface Props {
  visible?: boolean
  multiple?: boolean
  showInput?: boolean
  placeholder?: string
  dialogTitle?: string
  selectedIds?: string[]
}

const props = withDefaults(defineProps<Props>(), {
  visible: false,
  multiple: true,
  showInput: false,
  placeholder: '请选择用户',
  dialogTitle: '选择用户',
  selectedIds: () => []
})

const emit = defineEmits<{
  'update:visible': [value: boolean]
  'update:selectedIds': [ids: string[]]
  change: [users: TenantUserRecord[]]
}>()

const keyword = ref('')
const loading = ref(false)
const users = ref<TenantUserRecord[]>([])
const tempSelected = ref<string[]>([])
const selectedUserIds = ref<Set<string>>(new Set())

async function loadUsers() {
  loading.value = true
  try {
    users.value = await searchAvailableUsers(keyword.value)
  } finally {
    loading.value = false
  }
}

function toggleSelect(user: TenantUserRecord) {
  if (selectedUserIds.value.has(user.id)) {
    selectedUserIds.value.delete(user.id)
  } else {
    selectedUserIds.value.add(user.id)
  }
  tempSelected.value = Array.from(selectedUserIds.value)
}

function confirm() {
  const selectedUsers = users.value.filter((u) => tempSelected.value.includes(u.id))
  emit('update:selectedIds', tempSelected.value)
  emit('change', selectedUsers)
  emit('update:visible', false)
}

function cancel() {
  emit('update:visible', false)
}

function openDialog() {
  keyword.value = ''
  selectedUserIds.value = new Set(props.selectedIds)
  tempSelected.value = [...props.selectedIds]
  loadUsers()
}

watch(
  () => props.visible,
  (v) => {
    if (v) openDialog()
  }
)
</script>

<template>
  <el-dialog
    :model-value="visible"
    :title="dialogTitle"
    width="640px"
    :close-on-click-modal="false"
    @update:model-value="(v: boolean) => emit('update:visible', v)"
  >
    <div style="margin-bottom: 12px">
      <el-input
        v-model="keyword"
        placeholder="搜索用户姓名或账号"
        clearable
        style="width: 260px"
        @keyup.enter="loadUsers"
      >
        <template #append>
          <el-button @click="loadUsers">搜索</el-button>
        </template>
      </el-input>
    </div>
    <el-table
      v-loading="loading"
      :data="users"
      max-height="400"
      highlight-current-row
      @row-click="(r: unknown) => toggleSelect(r as TenantUserRecord)"
    >
      <el-table-column width="50" align="center">
        <template #default="{ row }">
          <el-checkbox
            :model-value="tempSelected.includes(row.id)"
            @change="toggleSelect(row as TenantUserRecord)"
          />
        </template>
      </el-table-column>
      <el-table-column prop="userName" label="账号" width="140" />
      <el-table-column prop="name" label="姓名" width="120" />
      <el-table-column prop="email" label="邮箱" min-width="180" />
      <el-table-column prop="roleNames" label="角色" width="140">
        <template #default="{ row }">
          <el-tag v-for="r in row.roleNames" :key="r" size="small" style="margin-right: 4px">
            {{ r }}
          </el-tag>
        </template>
      </el-table-column>
    </el-table>
    <template #footer>
      <span style="float: left; color: #909399; font-size: 13px">
        已选 {{ tempSelected.length }} 人
      </span>
      <el-button @click="cancel">取消</el-button>
      <el-button type="primary" @click="confirm">确定</el-button>
    </template>
  </el-dialog>
</template>
