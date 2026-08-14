<template>
  <MyDialog
    ref="dialogRef"
    :data="form"
    :title="form.id ? '编辑科室' : '新增科室'"
    width="680px"
    draggable
    :close-on-click-modal="false"
  >
    <template #header>
      <strong>{{ form.id ? '编辑科室' : '新增科室' }}</strong>
    </template>

    <template #default>
      <el-form
        ref="formRef"
        v-loading="loading"
        :model="form"
        :rules="rules"
        label-width="98px"
        status-icon
      >
        <el-row :gutter="18">
          <el-col :span="12">
            <el-form-item label="科室名称" prop="name">
              <el-input v-model="form.name" maxlength="40" show-word-limit placeholder="请输入" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="科室编码" prop="code">
              <el-input v-model="form.code" maxlength="64" clearable disabled />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="上级科室" prop="parentId">
              <el-tree-select
                v-model="form.parentId"
                :data="options.departmentOptions"
                node-key="value"
                check-strictly
                clearable
                filterable
                default-expand-all
                placeholder="不选则为顶级科室"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="科室类型" prop="type">
              <el-select v-model="form.type" placeholder="请选择" style="width: 100%">
                <el-option
                  v-for="option in options.typeOptions"
                  :key="option.value"
                  :label="option.label"
                  :value="option.value"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="科室类别" prop="category">
              <el-select v-model="form.category" placeholder="请选择" style="width: 100%">
                <el-option
                  v-for="option in options.categoryOptions"
                  :key="option.value"
                  :label="option.label"
                  :value="option.value"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="医保编码" prop="ybNo">
              <el-input v-model="form.ybNo" clearable />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="医保名称" prop="ybName">
              <el-input v-model="form.ybName" clearable />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="科别" prop="caty">
              <el-input v-model="form.caty" clearable />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="负责人" prop="leaderId">
              <el-select
                v-model="form.leaderId"
                clearable
                filterable
                placeholder="请选择"
                style="width: 100%"
              >
                <el-option
                  v-for="option in options.leaderOptions"
                  :key="option.value"
                  :label="option.label"
                  :value="option.value"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="排序号" prop="sort">
              <el-input-number
                v-model="form.sort"
                :min="0"
                :max="9999"
                controls-position="right"
                style="width: 100%"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="科室状态" prop="enabled">
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
import {
  createDepartment,
  getDepartmentDetail,
  loadDepartmentFormOptions,
  updateDepartment
} from '@/apis/basic/department'
import type { DepartmentFormOptions } from '@/apis/basic/department'
import { createEmptyDepartmentForm, toDepartmentForm } from '../adapter'
import type { DepartmentDialogOpenOptions, DepartmentFormModel } from '../type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

const emit = defineEmits<{ saved: [] }>()
const dialogRef = ref<DialogExpose | null>(null)
const formRef = ref<FormInstance>()
const form = reactive<DepartmentFormModel>(createEmptyDepartmentForm())
const options = reactive<DepartmentFormOptions>({
  typeOptions: [],
  categoryOptions: [],
  leaderOptions: [],
  departmentOptions: []
})
const loading = ref(false)
const saving = ref(false)

const rules: FormRules<DepartmentFormModel> = {
  name: [{ required: true, message: '请输入科室名称', trigger: 'blur' }],
  type: [{ required: true, message: '请选择科室类型', trigger: 'change' }],
  category: [{ required: true, message: '请选择科室类别', trigger: 'change' }],
  sort: [{ required: true, message: '请输入排序号', trigger: 'change' }]
}

async function open(openOptions: DepartmentDialogOpenOptions = {}) {
  Object.assign(form, createEmptyDepartmentForm(openOptions.parentId ?? null))
  Object.assign(options, {
    typeOptions: [],
    categoryOptions: [],
    leaderOptions: [],
    departmentOptions: []
  })
  dialogRef.value?.openDialog()
  await nextTick()
  formRef.value?.clearValidate()

  loading.value = true
  try {
    const [formOptions, detail] = await Promise.all([
      loadDepartmentFormOptions(openOptions.id),
      openOptions.id ? getDepartmentDetail(openOptions.id) : Promise.resolve(null)
    ])
    Object.assign(options, formOptions)
    if (detail) Object.assign(form, toDepartmentForm(detail))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '科室表单加载失败')
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
      name: form.name,
      code: form.code,
      parentId: form.type === '1' ? null : form.parentId,
      type: form.type,
      category: form.category,
      leaderId: form.leaderId,
      sort: form.sort,
      enabled: form.enabled
      ,ybNo: form.ybNo, ybName: form.ybName, medinsId: form.medinsId,
      medinsAdmdvs: form.medinsAdmdvs, medinsType: form.medinsType, medinsLv: form.medinsLv,
      caty: form.caty, pyStr: form.pyStr, wbStr: form.wbStr
    }
    // 医院节点必须位于组织树根部，避免产生“医院隶属于科室”的无效层级。
    if (form.id) await updateDepartment(form.id, payload)
    else await createDepartment(payload)
    ElMessage.success(form.id ? '科室更新成功' : '科室创建成功')
    dialogRef.value?.closeDialog()
    emit('saved')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '科室保存失败')
  } finally {
    saving.value = false
  }
}

defineExpose({ open })
</script>
