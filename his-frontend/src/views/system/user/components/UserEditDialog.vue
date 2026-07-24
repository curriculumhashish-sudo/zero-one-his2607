<template>
  <MyDialog
    ref="dialogRef"
    :data="form"
    :title="form.id ? '编辑用户' : '新增用户'"
    width="620px"
    draggable
    :close-on-click-modal="false"
  >
    <template #header>
      <strong>{{ form.id ? '编辑用户' : '新增用户' }}</strong>
    </template>

    <template #default>
      <el-form
        ref="formRef"
        v-loading="loading"
        :model="form"
        :rules="rules"
        label-width="92px"
        status-icon
      >
        <el-row :gutter="18">
          <el-col :span="12">
            <el-form-item label="用户名" prop="username">
              <el-input
                v-model="form.username"
                :disabled="Boolean(form.id)"
                maxlength="32"
                show-word-limit
                placeholder="请输入登录用户名"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="姓名" prop="realName">
              <el-input v-model="form.realName" maxlength="30" placeholder="请输入姓名" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="手机号" prop="phone">
              <el-input v-model="form.phone" maxlength="11" placeholder="请输入手机号" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="邮箱" prop="email">
              <el-input v-model="form.email" maxlength="80" placeholder="请输入邮箱" />
            </el-form-item>
          </el-col>
          <el-col :span="24">
            <el-form-item label="角色" prop="roleIds">
              <RemoteSelectField
                v-model="form.roleIds"
                multiple
                :load-options="loadRoles"
                :initial-options="initialRoleOptions"
                value-key="id"
                label-key="name"
                placeholder="输入角色名称查询，可多选"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="账号状态" prop="enabled">
              <el-switch
                v-model="form.enabled"
                inline-prompt
                active-text="启用"
                inactive-text="停用"
              />
            </el-form-item>
          </el-col>
        </el-row>
      </el-form>
    </template>

    <template #footer>
      <el-button :disabled="saving" @click="close">取消</el-button>
      <el-button type="primary" :loading="saving" :disabled="loading" @click="submit">
        保存
      </el-button>
    </template>
  </MyDialog>
</template>

<script setup lang="ts">
import { nextTick, reactive, ref } from 'vue'
import type { FormInstance, FormRules } from 'element-plus'
import { ElMessage } from 'element-plus'
import MyDialog from '@/components/mydialog/MyDialog.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import { createUser, getUserDetail, loadRoleOptions, updateUser } from '@/apis/system/user'
import type { RoleOption } from '@/apis/system/user'
import { createEmptyUserForm, toUserForm } from '../adapter'
import type { UserFormModel } from '../type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

const emit = defineEmits<{ saved: [] }>()
const dialogRef = ref<DialogExpose | null>(null)
const formRef = ref<FormInstance>()
const form = reactive<UserFormModel>(createEmptyUserForm())
const initialRoleOptions = ref<RoleOption[]>([])
const loading = ref(false)
const saving = ref(false)

const rules: FormRules<UserFormModel> = {
  username: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    {
      pattern: /^[a-zA-Z][a-zA-Z0-9_-]{2,31}$/,
      message: '用户名须以字母开头，长度3至32位',
      trigger: 'blur'
    }
  ],
  realName: [{ required: true, message: '请输入姓名', trigger: 'blur' }],
  phone: [
    { required: true, message: '请输入手机号', trigger: 'blur' },
    { pattern: /^1\d{10}$/, message: '请输入11位手机号', trigger: 'blur' }
  ],
  email: [{ type: 'email', message: '邮箱格式不正确', trigger: 'blur' }],
  roleIds: [
    { type: 'array', required: true, min: 1, message: '请至少选择一个角色', trigger: 'change' }
  ]
}

async function loadRoles(keyword: string, context: { signal: AbortSignal }) {
  return loadRoleOptions(keyword, context.signal)
}

async function open(userId?: string) {
  Object.assign(form, createEmptyUserForm())
  initialRoleOptions.value = []
  dialogRef.value?.openDialog()
  await nextTick()
  formRef.value?.clearValidate()
  if (!userId) return

  loading.value = true
  try {
    const user = await getUserDetail(userId)
    Object.assign(form, toUserForm(user))
    initialRoleOptions.value = user.roleIds.map((id, index) => ({
      id,
      name: user.roleNames[index] ?? id
    }))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '用户详情加载失败')
    close()
  } finally {
    loading.value = false
  }
}

function close() {
  if (!saving.value) dialogRef.value?.closeDialog()
}

async function submit() {
  if (saving.value || loading.value) return
  const valid = await formRef.value?.validate().catch(() => false)
  if (!valid) return

  saving.value = true
  try {
    const payload = {
      username: form.username,
      realName: form.realName,
      phone: form.phone,
      email: form.email,
      roleIds: [...form.roleIds],
      enabled: form.enabled
    }
    if (form.id) await updateUser(form.id, payload)
    else await createUser(payload)
    ElMessage.success(form.id ? '用户更新成功' : '用户创建成功')
    dialogRef.value?.closeDialog()
    emit('saved')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '用户保存失败')
  } finally {
    saving.value = false
  }
}

defineExpose({ open })
</script>
