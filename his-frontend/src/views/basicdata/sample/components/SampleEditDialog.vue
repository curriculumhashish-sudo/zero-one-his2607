<template>
  <MyDialog
    ref="dialogRef"
    :data="form"
    :title="form.id ? '编辑样本' : '新增样本'"
    width="720px"
    draggable
    :close-on-click-modal="false"
  >
    <template #header>
      <strong>{{ form.id ? '编辑样本' : '新增样本' }}</strong>
    </template>

    <template #default>
      <el-form
        ref="formRef"
        v-loading="loading"
        :model="form"
        :rules="rules"
        label-width="110px"
        status-icon
      >
        <el-row :gutter="18">
          <el-col :span="12">
            <el-form-item label="样本编码" prop="code">
              <el-input v-model="form.code" maxlength="20" placeholder="如 SAM001" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="样本名称" prop="name">
              <el-input v-model="form.name" maxlength="60" show-word-limit placeholder="如 全血样本" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="拼音码" prop="pyCode">
              <el-input v-model="form.pyCode" maxlength="20" placeholder="如 QXYB" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="样本类型" prop="type">
              <el-select v-model="form.type" placeholder="请选择" style="width: 100%">
                <el-option
                  v-for="opt in options.typeOptions"
                  :key="opt.value"
                  :label="opt.label"
                  :value="opt.value"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="采集方法" prop="collectionMethod">
              <el-input v-model="form.collectionMethod" maxlength="40" placeholder="如 静脉穿刺" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="保存条件" prop="storageCondition">
              <el-input v-model="form.storageCondition" maxlength="40" placeholder="如 2-8°C冷藏" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="有效时长" prop="validHours">
              <el-input-number
                v-model="form.validHours"
                :min="0"
                :precision="0"
                controls-position="right"
                style="width: 100%"
              />
              <span class="form-suffix">小时</span>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="排序号" prop="sort">
              <el-input-number
                v-model="form.sort"
                :min="0"
                :precision="0"
                controls-position="right"
                style="width: 100%"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="启用状态" prop="enabled">
              <el-switch v-model="form.enabled" inline-prompt active-text="启用" inactive-text="停用" />
            </el-form-item>
          </el-col>
          <el-col :span="24">
            <el-form-item label="备注" prop="remark">
              <el-input
                v-model="form.remark"
                type="textarea"
                :rows="2"
                maxlength="200"
                show-word-limit
                placeholder="请输入"
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
import {
  createSample,
  getSampleDetail,
  loadSampleFormOptions,
  updateSample
} from '@/apis/basicdata/sample'
import type { SampleFormOptions } from '@/apis/basicdata/sample'
import { createEmptySampleForm, toSampleForm } from '../adapter'
import type { SampleDialogOpenOptions, SampleFormModel } from '../type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

const emit = defineEmits<{ saved: [] }>()
const dialogRef = ref<DialogExpose | null>(null)
const formRef = ref<FormInstance>()
const form = reactive<SampleFormModel>(createEmptySampleForm())
const options = reactive<SampleFormOptions>({
  typeOptions: []
})
const loading = ref(false)
const saving = ref(false)

const rules: FormRules<SampleFormModel> = {
  code: [{ required: true, message: '请输入样本编码', trigger: 'blur' }],
  name: [{ required: true, message: '请输入样本名称', trigger: 'blur' }],
  type: [{ required: true, message: '请选择样本类型', trigger: 'change' }]
}

async function open(openOptions: SampleDialogOpenOptions = {}) {
  Object.assign(form, createEmptySampleForm())
  dialogRef.value?.openDialog()
  await nextTick()
  formRef.value?.clearValidate()

  loading.value = true
  try {
    const [formOptions, detail] = await Promise.all([
      loadSampleFormOptions(),
      openOptions.id ? getSampleDetail(openOptions.id) : Promise.resolve(null)
    ])
    Object.assign(options, formOptions)
    if (detail) Object.assign(form, toSampleForm(detail))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '样本表单加载失败')
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
    if (form.id) await updateSample(form.id, payload)
    else await createSample(payload)
    ElMessage.success(form.id ? '样本更新成功' : '样本创建成功')
    dialogRef.value?.closeDialog()
    emit('saved')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '样本保存失败')
  } finally {
    saving.value = false
  }
}

defineExpose({ open })
</script>

<style scoped>
.form-suffix {
  margin-left: 8px;
  color: var(--el-text-color-secondary);
  font-size: 14px;
  white-space: nowrap;
}
</style>
