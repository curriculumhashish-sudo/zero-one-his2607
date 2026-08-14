<script setup lang="ts">
import { ref } from 'vue'
import { ElMessage, type FormInstance, type FormRules } from 'element-plus'
import { changePassword, type PasswordChangePayload } from '@/apis/account/profile'

const formRef = ref<FormInstance>()
const submitting = ref(false)
const formData = ref<PasswordChangePayload>({
  oldPassword: '',
  newPassword: '',
  confirmPassword: ''
})

const rules: FormRules = {
  oldPassword: [{ required: true, message: '请输入原密码', trigger: 'blur' }],
  newPassword: [
    { required: true, message: '请输入新密码', trigger: 'blur' },
    { min: 6, message: '密码至少6位', trigger: 'blur' }
  ],
  confirmPassword: [
    { required: true, message: '请再次输入新密码', trigger: 'blur' },
    {
      validator: (_rule, value, callback) => {
        if (value !== formData.value.newPassword) {
          callback(new Error('两次密码不一致'))
        } else {
          callback()
        }
      },
      trigger: 'blur'
    }
  ]
}

async function handleSubmit() {
  if (!formRef.value) return
  try {
    await formRef.value.validate()
  } catch {
    return
  }
  submitting.value = true
  try {
    await changePassword(formData.value)
    ElMessage.success('密码修改成功')
    formData.value = { oldPassword: '', newPassword: '', confirmPassword: '' }
    formRef.value?.resetFields()
  } catch (e: any) {
    ElMessage.error(e.message || '修改失败')
  } finally {
    submitting.value = false
  }
}
</script>

<template>
  <div class="password-setting">
    <el-form
      ref="formRef"
      :model="formData"
      :rules="rules"
      label-width="120px"
      style="max-width: 500px"
    >
      <el-form-item label="原密码" prop="oldPassword">
        <el-input v-model="formData.oldPassword" type="password" show-password placeholder="请输入原密码" />
      </el-form-item>
      <el-form-item label="新密码" prop="newPassword">
        <el-input v-model="formData.newPassword" type="password" show-password placeholder="请输入新密码（至少6位）" />
      </el-form-item>
      <el-form-item label="确认密码" prop="confirmPassword">
        <el-input v-model="formData.confirmPassword" type="password" show-password placeholder="请再次输入新密码" />
      </el-form-item>
      <el-form-item>
        <el-button type="primary" :loading="submitting" @click="handleSubmit">确认修改</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>
