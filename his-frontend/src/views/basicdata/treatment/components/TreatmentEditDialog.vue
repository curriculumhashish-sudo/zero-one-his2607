<template>
  <MyDialog
    ref="dialogRef"
    :data="form"
    :title="form.id ? '编辑诊疗项目' : '新增诊疗项目'"
    width="720px"
    draggable
    :close-on-click-modal="false"
  >
    <template #header>
      <strong>{{ form.id ? '编辑诊疗项目' : '新增诊疗项目' }}</strong>
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
            <el-form-item label="项目编码" prop="code">
              <el-input v-model="form.code" maxlength="20" show-word-limit placeholder="如 EXAM001" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="项目名称" prop="name">
              <el-input v-model="form.name" maxlength="60" show-word-limit placeholder="如 血常规检查" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="拼音码" prop="pyCode">
              <el-input v-model="form.pyCode" maxlength="20" placeholder="如 XCGJC" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="诊疗类别" prop="category">
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
            <el-form-item label="计价单位" prop="unit">
              <el-select v-model="form.unit" filterable allow-create default-first-option placeholder="请选择或输入" style="width: 100%">
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
            <el-form-item label="价格" prop="price">
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
  createTreatment,
  getTreatmentDetail,
  loadTreatmentFormOptions,
  updateTreatment
} from '@/apis/basicdata/treatment'
import type { TreatmentFormOptions } from '@/apis/basicdata/treatment'
import { createEmptyTreatmentForm, toTreatmentForm } from '../adapter'
import type { TreatmentDialogOpenOptions, TreatmentFormModel } from '../type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

const emit = defineEmits<{ saved: [] }>()
const dialogRef = ref<DialogExpose | null>(null)
const formRef = ref<FormInstance>()
const form = reactive<TreatmentFormModel>(createEmptyTreatmentForm())
const options = reactive<TreatmentFormOptions>({
  categoryOptions: [],
  unitOptions: []
})
const loading = ref(false)
const saving = ref(false)

const rules: FormRules<TreatmentFormModel> = {
  code: [{ required: true, message: '请输入项目编码', trigger: 'blur' }],
  name: [{ required: true, message: '请输入项目名称', trigger: 'blur' }],
  category: [{ required: true, message: '请选择诊疗类别', trigger: 'change' }],
  unit: [{ required: true, message: '请选择计价单位', trigger: 'change' }],
  price: [{ required: true, message: '请输入价格', trigger: 'blur' }]
}

async function open(openOptions: TreatmentDialogOpenOptions = {}) {
  Object.assign(form, createEmptyTreatmentForm())
  dialogRef.value?.openDialog()
  await nextTick()
  formRef.value?.clearValidate()

  loading.value = true
  try {
    const [formOptions, detail] = await Promise.all([
      loadTreatmentFormOptions(),
      openOptions.id ? getTreatmentDetail(openOptions.id) : Promise.resolve(null)
    ])
    Object.assign(options, formOptions)
    if (detail) Object.assign(form, toTreatmentForm(detail))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '诊疗项目表单加载失败')
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
    if (form.id) await updateTreatment(form.id, payload)
    else await createTreatment(payload)
    ElMessage.success(form.id ? '诊疗项目更新成功' : '诊疗项目创建成功')
    dialogRef.value?.closeDialog()
    emit('saved')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '诊疗项目保存失败')
  } finally {
    saving.value = false
  }
}

defineExpose({ open })
</script>
