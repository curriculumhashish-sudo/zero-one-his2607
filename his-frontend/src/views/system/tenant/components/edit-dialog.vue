<script setup lang="ts">
import { reactive, ref, watch } from 'vue'
import { ElMessage, type FormInstance, type FormRules } from 'element-plus'
import { createTenant, updateTenant, type TenantRecord, type TenantSavePayload } from '@/apis/system/tenant'

interface Props {
  visible?: boolean
  formData?: Partial<TenantRecord>
}

const props = withDefaults(defineProps<Props>(), {
  visible: false,
  formData: () => ({})
})

const emit = defineEmits<{
  success: []
  'update:visible': [value: boolean]
}>()

const isEdit = ref(false)
const formRef = ref<FormInstance>()
const submitting = ref(false)
const formData = reactive<TenantSavePayload>({
  code: '',
  name: '',
  description: '',
  extraProperties: '',
  isActive: 'true'
})

const rules: FormRules = {
  name: [{ required: true, message: '请输入租户名称', trigger: 'blur' }],
  code: [{ required: true, message: '请输入租户编码', trigger: 'blur' }]
}

function openDialog() {
  isEdit.value = !!props.formData?.id
  if (isEdit.value) {
    Object.assign(formData, {
      code: props.formData?.code ?? '',
      name: props.formData?.name ?? '',
      description: props.formData?.description ?? '',
      extraProperties: props.formData?.extraProperties ?? '',
      isActive: props.formData?.isActive ?? 'true'
    })
  } else {
    Object.assign(formData, {
      code: '',
      name: '',
      description: '',
      extraProperties: '',
      isActive: 'true'
    })
  }
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
    if (isEdit.value && props.formData?.id) {
      await updateTenant(props.formData.id, formData)
      ElMessage.success('更新成功')
    } else {
      await createTenant(formData)
      ElMessage.success('创建成功')
    }
    emit('success')
    emit('update:visible', false)
  } catch (e: any) {
    ElMessage.error(e.message || '操作失败')
  } finally {
    submitting.value = false
  }
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
    :title="isEdit ? '编辑租户' : '新增租户'"
    width="560px"
    :close-on-click-modal="false"
    @update:model-value="(v: boolean) => emit('update:visible', v)"
  >
    <el-form ref="formRef" :model="formData" :rules="rules" label-width="100px">
      <el-form-item label="租户编码" prop="code" v-if="!isEdit">
        <el-input v-model="formData.code" placeholder="请输入租户编码" clearable />
      </el-form-item>
      <el-form-item label="租户名称" prop="name">
        <el-input v-model="formData.name" placeholder="请输入租户名称" clearable />
      </el-form-item>
      <el-form-item label="状态" prop="isActive">
        <el-select v-model="formData.isActive" style="width: 100%">
          <el-option label="启用" value="true" />
          <el-option label="禁用" value="false" />
        </el-select>
      </el-form-item>
      <el-form-item label="描述" prop="description">
        <el-input v-model="formData.description" type="textarea" :rows="3" placeholder="请输入描述信息" />
      </el-form-item>
      <el-form-item label="扩展信息" prop="extraProperties">
        <el-input v-model="formData.extraProperties" type="textarea" :rows="2" placeholder="JSON 格式，可选" />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button @click="emit('update:visible', false)">取消</el-button>
      <el-button type="primary" :loading="submitting" @click="handleSubmit">确定</el-button>
    </template>
  </el-dialog>
</template>
