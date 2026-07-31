<template>
  <MyDialog
    ref="dialogRef"
    :data="form"
    :title="form.id ? '编辑频次' : '新增频次'"
    width="720px"
    draggable
    :close-on-click-modal="false"
  >
    <template #header>
      <strong>{{ form.id ? '编辑频次' : '新增频次' }}</strong>
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
            <el-form-item label="频次编码" prop="code">
              <el-input v-model="form.code" maxlength="20" show-word-limit placeholder="如 QD, BID, TID" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="频次名称" prop="name">
              <el-input v-model="form.name" maxlength="40" show-word-limit placeholder="如 每日一次" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="频次类型" prop="type">
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
            <el-form-item label="每日次数" prop="timesPerDay">
              <el-input-number v-model="form.timesPerDay" :min="0" controls-position="right" style="width: 100%" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="间隔(小时)" prop="intervalHours">
              <el-input-number v-model="form.intervalHours" :min="0" controls-position="right" style="width: 100%" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="排序号" prop="sort">
              <el-input-number v-model="form.sort" :min="0" controls-position="right" style="width: 100%" />
            </el-form-item>
          </el-col>
          <el-col :span="24">
            <el-form-item label="执行时间点" prop="timePoints">
              <el-input v-model="form.timePoints" maxlength="100" placeholder="如 08:00,12:00,20:00（多个用逗号分隔）" />
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
  createFrequency,
  getFrequencyDetail,
  loadFrequencyFormOptions,
  updateFrequency
} from '@/apis/basicdata/frequency'
import type { FrequencyFormOptions } from '@/apis/basicdata/frequency'
import { createEmptyFrequencyForm, toFrequencyForm } from '../adapter'
import type { FrequencyDialogOpenOptions, FrequencyFormModel } from '../type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

const emit = defineEmits<{ saved: [] }>()
const dialogRef = ref<DialogExpose | null>(null)
const formRef = ref<FormInstance>()
const form = reactive<FrequencyFormModel>(createEmptyFrequencyForm())
const options = reactive<FrequencyFormOptions>({
  typeOptions: []
})
const loading = ref(false)
const saving = ref(false)

const rules: FormRules<FrequencyFormModel> = {
  code: [{ required: true, message: '请输入频次编码', trigger: 'blur' }],
  name: [{ required: true, message: '请输入频次名称', trigger: 'blur' }],
  type: [{ required: true, message: '请选择频次类型', trigger: 'change' }]
}

async function open(openOptions: FrequencyDialogOpenOptions = {}) {
  Object.assign(form, createEmptyFrequencyForm())
  dialogRef.value?.openDialog()
  await nextTick()
  formRef.value?.clearValidate()

  loading.value = true
  try {
    const [formOptions, detail] = await Promise.all([
      loadFrequencyFormOptions(),
      openOptions.id ? getFrequencyDetail(openOptions.id) : Promise.resolve(null)
    ])
    Object.assign(options, formOptions)
    if (detail) Object.assign(form, toFrequencyForm(detail))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '频次表单加载失败')
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
    if (form.id) await updateFrequency(form.id, payload)
    else await createFrequency(payload)
    ElMessage.success(form.id ? '频次更新成功' : '频次创建成功')
    dialogRef.value?.closeDialog()
    emit('saved')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '频次保存失败')
  } finally {
    saving.value = false
  }
}

defineExpose({ open })
</script>
