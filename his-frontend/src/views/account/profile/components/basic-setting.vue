<script setup lang="ts">
import { ref, watch } from 'vue'
import { ElMessage, type FormInstance, type FormRules } from 'element-plus'
import type { ProfileInfo, ProfileUpdatePayload } from '@/apis/account/profile'
import { updateProfile } from '@/apis/account/profile'

interface Props {
  profile: ProfileInfo | null
}

const props = defineProps<Props>()
const emit = defineEmits<{
  saved: []
}>()

const formRef = ref<FormInstance>()
const submitting = ref(false)
const formData = ref<ProfileUpdatePayload>({
  name: '',
  email: '',
  phone: '',
  gender: 'male'
})

const rules: FormRules = {
  email: [
    { type: 'email', message: '请输入正确的邮箱地址', trigger: 'blur' }
  ],
  phone: [
    { pattern: /^1[3-9]\d{9}$/, message: '请输入正确的手机号', trigger: 'blur' }
  ]
}

watch(
  () => props.profile,
  (p) => {
    if (p) {
      formData.value = {
        name: p.name,
        email: p.email,
        phone: p.phone,
        gender: p.gender
      }
    }
  },
  { immediate: true }
)

async function handleSubmit() {
  if (!formRef.value) return
  try {
    await formRef.value.validate()
  } catch {
    return
  }
  submitting.value = true
  try {
    await updateProfile(formData.value)
    ElMessage.success('保存成功')
    emit('saved')
  } catch (e: any) {
    ElMessage.error(e.message || '保存失败')
  } finally {
    submitting.value = false
  }
}
</script>

<template>
  <div class="basic-setting">
    <el-form
      ref="formRef"
      :model="formData"
      :rules="rules"
      label-width="100px"
      style="max-width: 500px"
    >
      <el-form-item label="姓名" prop="name">
        <el-input v-model="formData.name" placeholder="请输入姓名" />
      </el-form-item>
      <el-form-item label="性别" prop="gender">
        <el-select v-model="formData.gender" style="width: 100%">
          <el-option label="男" value="male" />
          <el-option label="女" value="female" />
        </el-select>
      </el-form-item>
      <el-form-item label="邮箱" prop="email">
        <el-input v-model="formData.email" placeholder="请输入邮箱" />
      </el-form-item>
      <el-form-item label="手机号" prop="phone">
        <el-input v-model="formData.phone" placeholder="请输入手机号" />
      </el-form-item>
      <el-form-item>
        <el-button type="primary" :loading="submitting" @click="handleSubmit">保存</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>
