<template>
  <MyDialog
    ref="dialogRef"
    :data="form"
    :title="form.id ? '编辑器材' : '新增器材'"
    width="700px"
    draggable
    :close-on-click-modal="false"
  >
    <template #header>
      <strong>{{ form.id ? '编辑器材' : '新增器材' }}</strong>
    </template>

    <template #default>
      <el-form
        ref="formRef"
        v-loading="loading"
        :model="form"
        :rules="rules"
        label-width="100px"
        status-icon
      >
        <el-row :gutter="18">
          <el-col :span="12">
            <el-form-item label="器材编码" prop="code">
              <el-input v-model="form.code" maxlength="20" placeholder="如 EQ001" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="器材名称" prop="name">
              <el-input v-model="form.name" maxlength="60" show-word-limit placeholder="请输入" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="拼音码" prop="pyCode">
              <el-input v-model="form.pyCode" maxlength="20" placeholder="自动生成或手填" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="器材分类" prop="category">
              <el-select v-model="form.category" placeholder="请选择" style="width: 100%">
                <el-option
                  v-for="opt in options.categoryOptions"
                  :key="opt.value"
                  :label="opt.label"
                  :value="opt.value"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="规格" prop="specification">
              <el-input v-model="form.specification" maxlength="40" placeholder="如 5ml" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="型号" prop="model">
              <el-input v-model="form.model" maxlength="40" placeholder="如 台式" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="计量单位" prop="unit">
              <el-select v-model="form.unit" filterable clearable placeholder="请选择" style="width: 100%">
                <el-option
                  v-for="opt in options.unitOptions"
                  :key="opt.value"
                  :label="opt.label"
                  :value="opt.value"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="单价" prop="price">
              <el-input-number v-model="form.price" :min="0" :precision="2" controls-position="right" style="width: 100%" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="排序号" prop="sort">
              <el-input-number v-model="form.sort" :min="0" controls-position="right" style="width: 100%" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="启用状态" prop="enabled">
              <el-switch v-model="form.enabled" inline-prompt active-text="启用" inactive-text="停用" />
            </el-form-item>
          </el-col>
          <el-col :span="24">
            <el-form-item label="备注" prop="remark">
              <el-input v-model="form.remark" type="textarea" :rows="2" maxlength="200" show-word-limit placeholder="请输入" />
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
import {
  createEquipment,
  getEquipmentDetail,
  loadEquipmentFormOptions,
  updateEquipment
} from '@/apis/basicdata/equipment'
import type { EquipmentFormOptions } from '@/apis/basicdata/equipment'
import { createEmptyEquipmentForm, toEquipmentForm } from '../adapter'
import type { EquipmentDialogOpenOptions, EquipmentFormModel } from '../type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

const emit = defineEmits<{ saved: [] }>()
const dialogRef = ref<DialogExpose | null>(null)
const formRef = ref<FormInstance>()
const form = reactive<EquipmentFormModel>(createEmptyEquipmentForm())
const options = reactive<EquipmentFormOptions>({
  categoryOptions: [],
  unitOptions: []
})
const loading = ref(false)
const saving = ref(false)

const rules: FormRules<EquipmentFormModel> = {
  code: [{ required: true, message: '请输入器材编码', trigger: 'blur' }],
  name: [{ required: true, message: '请输入器材名称', trigger: 'blur' }],
  category: [{ required: true, message: '请选择器材分类', trigger: 'change' }],
  unit: [{ required: true, message: '请选择计量单位', trigger: 'change' }]
}

async function open(openOptions: EquipmentDialogOpenOptions = {}) {
  Object.assign(form, createEmptyEquipmentForm())
  dialogRef.value?.openDialog()
  await nextTick()
  formRef.value?.clearValidate()

  loading.value = true
  try {
    const [formOptions, detail] = await Promise.all([
      loadEquipmentFormOptions(),
      openOptions.id ? getEquipmentDetail(openOptions.id) : Promise.resolve(null)
    ])
    Object.assign(options, formOptions)
    if (detail) Object.assign(form, toEquipmentForm(detail))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '器材表单加载失败')
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
    const payload = { ...form }
    if (form.id) await updateEquipment(form.id, payload)
    else await createEquipment(payload)
    ElMessage.success(form.id ? '器材更新成功' : '器材创建成功')
    dialogRef.value?.closeDialog()
    emit('saved')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '器材保存失败')
  } finally {
    saving.value = false
  }
}

defineExpose({ open })
</script>
