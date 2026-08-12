<script setup lang="ts">
import { computed, reactive, ref } from 'vue'
import { Plus } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'

import EmptyState from '@/components/common/EmptyState.vue'
import type { InpatientDiagnosisItem } from '../type'

defineOptions({
  name: 'InpatientDiagnosisEditor'
})

const props = withDefaults(
  defineProps<{
    modelValue?: InpatientDiagnosisItem[]
    patientSelected?: boolean
    accepted?: boolean
    loading?: boolean
    saving?: boolean
    error?: string
    dirty?: boolean
    readonly?: boolean
  }>(),
  {
    modelValue: () => [],
    patientSelected: false,
    accepted: false,
    loading: false,
    saving: false,
    error: '',
    dirty: false,
    readonly: false
  }
)

const emit = defineEmits<{
  'update:modelValue': [items: InpatientDiagnosisItem[]]
  retry: []
}>()

const dialogVisible = ref(false)
const form = reactive({
  definitionCode: '',
  description: '',
  diagnosisTypeCode: '入院初步诊断',
  tcmFlag: 0,
  primaryFlag: 0
})

const canEdit = computed(() => props.accepted && !props.readonly && !props.saving)
const diagnosisTypeOptions = computed(() => {
  const options = new Map<string, string>([['入院初步诊断', '入院初步诊断']])
  props.modelValue.forEach((item) => {
    const value = item.diagnosisTypeCode || item.diagnosisTypeName
    if (value) options.set(value, item.diagnosisTypeName || value)
  })
  return Array.from(options, ([value, label]) => ({ value, label }))
})

function commit(items: InpatientDiagnosisItem[]) {
  emit(
    'update:modelValue',
    items.map((item, index) => ({ ...item, rank: index + 1 }))
  )
}

function openAddDialog() {
  form.definitionCode = ''
  form.description = ''
  form.diagnosisTypeCode = '入院初步诊断'
  form.tcmFlag = 0
  form.primaryFlag = props.modelValue.length === 0 ? 1 : 0
  dialogVisible.value = true
}

function addDiagnosis() {
  const description = form.description.trim()
  if (!description) {
    ElMessage.warning('请输入诊断描述')
    return
  }
  if (props.modelValue.some((item) => item.description.trim() === description)) {
    ElMessage.warning('本次住院已存在相同诊断')
    return
  }

  const current =
    form.primaryFlag === 1
      ? props.modelValue.map((item) => ({ ...item, primaryFlag: 0 }))
      : [...props.modelValue]
  current.push({
    key: `draft-${Date.now()}-${current.length}`,
    definitionCode: form.definitionCode.trim(),
    description,
    kindName: form.tcmFlag === 1 ? '中医' : '西医',
    diagnosisTypeCode: form.diagnosisTypeCode,
    diagnosisTypeName:
      diagnosisTypeOptions.value.find((item) => item.value === form.diagnosisTypeCode)?.label ??
      form.diagnosisTypeCode,
    primaryFlag: form.primaryFlag === 1 || current.length === 0 ? 1 : 0,
    rank: current.length + 1,
    tcmFlag: form.tcmFlag
  })
  commit(current)
  dialogVisible.value = false
}

function updateDiagnosisType(row: unknown, diagnosisTypeCode: string) {
  const diagnosis = row as InpatientDiagnosisItem
  const diagnosisTypeName =
    diagnosisTypeOptions.value.find((item) => item.value === diagnosisTypeCode)?.label ??
    diagnosisTypeCode
  commit(
    props.modelValue.map((item) => ({
      ...item,
      diagnosisTypeCode: item.key === diagnosis.key ? diagnosisTypeCode : item.diagnosisTypeCode,
      diagnosisTypeName: item.key === diagnosis.key ? diagnosisTypeName : item.diagnosisTypeName
    }))
  )
}

function removeDiagnosis(row: unknown) {
  const diagnosis = row as InpatientDiagnosisItem
  const remaining = props.modelValue.filter((item) => item.key !== diagnosis.key)
  if (diagnosis.primaryFlag === 1 && remaining.length > 0) {
    remaining[0] = { ...remaining[0]!, primaryFlag: 1 }
  }
  commit(remaining)
}
</script>

<template>
  <section class="inpatient-diagnosis-editor">
    <EmptyState
      v-if="!patientSelected"
      title="请选择患者"
      description="从患者队列选择患者后，可查看本次住院诊断"
    />

    <div v-else-if="loading" class="inpatient-diagnosis-editor__skeleton">
      <el-skeleton :rows="8" animated />
    </div>

    <EmptyState
      v-else-if="error"
      title="诊断加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <EmptyState
      v-else-if="!accepted && !readonly"
      title="当前患者不可编辑"
      description="仅在患者住院诊疗阶段允许维护诊断"
    />

    <div v-else class="inpatient-diagnosis-editor__content">
      <header class="inpatient-diagnosis-editor__toolbar">
        <div class="inpatient-diagnosis-editor__title">
          <strong>本次诊断</strong>
          <el-tag size="small" effect="plain">{{ modelValue.length }}</el-tag>
          <span v-if="readonly" class="is-readonly">只读</span>
          <span v-else-if="dirty" class="is-dirty">有未保存修改</span>
          <span v-else class="is-saved">内容已保存</span>
        </div>

        <el-button
          v-if="!readonly"
          type="primary"
          :icon="Plus"
          :disabled="!canEdit"
          @click="openAddDialog"
        >
          新增诊断
        </el-button>
      </header>

      <el-table
        :data="modelValue"
        row-key="key"
        border
        table-layout="fixed"
        class="inpatient-diagnosis-editor__table"
      >
        <el-table-column type="index" label="#" width="56" align="center" />
        <el-table-column label="诊断编码" min-width="154" show-overflow-tooltip>
          <template #default="{ row }">
            {{ row.definitionCode || '—' }}
          </template>
        </el-table-column>
        <el-table-column
          prop="description"
          label="诊断名称"
          min-width="260"
          show-overflow-tooltip
        />
        <el-table-column label="类别" width="92" align="center">
          <template #default="{ row }">
            {{ row.kindName || (row.tcmFlag === 1 ? '中医' : '西医') }}
          </template>
        </el-table-column>
        <el-table-column label="诊断类型" min-width="164" align="center">
          <template #default="{ row }">
            <el-select
              :model-value="row.diagnosisTypeCode || row.diagnosisTypeName"
              :disabled="!canEdit"
              placeholder="请选择诊断类型"
              @change="updateDiagnosisType(row, $event)"
            >
              <el-option
                v-for="option in diagnosisTypeOptions"
                :key="option.value"
                :label="option.label"
                :value="option.value"
              />
            </el-select>
          </template>
        </el-table-column>
        <el-table-column label="主诊" width="76" align="center">
          <template #default="{ row }">
            {{ row.primaryFlag === 1 ? '是' : '否' }}
          </template>
        </el-table-column>
        <el-table-column v-if="!readonly" label="操作" width="88" align="center" fixed="right">
          <template #default="{ row }">
            <el-button type="danger" link :disabled="!canEdit" @click="removeDiagnosis(row)">
              删除
            </el-button>
          </template>
        </el-table-column>
        <template #empty>
          <el-empty description="暂无诊断，请点击“新增诊断”" :image-size="88" />
        </template>
      </el-table>
    </div>

    <el-dialog v-model="dialogVisible" title="新增住院诊断" width="460px" destroy-on-close>
      <el-form label-width="86px" @submit.prevent>
        <el-form-item label="诊断编码">
          <el-input
            v-model="form.definitionCode"
            maxlength="32"
            clearable
            placeholder="请输入诊断编码"
          />
        </el-form-item>
        <el-form-item label="诊断名称" required>
          <el-input
            v-model="form.description"
            maxlength="200"
            show-word-limit
            clearable
            placeholder="请输入诊断描述"
            @keyup.enter="addDiagnosis"
          />
        </el-form-item>
        <el-form-item label="类别">
          <el-radio-group v-model="form.tcmFlag">
            <el-radio :value="0">西医</el-radio>
            <el-radio :value="1">中医</el-radio>
          </el-radio-group>
        </el-form-item>
        <el-form-item label="诊断类型" required>
          <el-select v-model="form.diagnosisTypeCode" style="width: 100%">
            <el-option
              v-for="option in diagnosisTypeOptions"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="主诊断">
          <el-switch v-model="form.primaryFlag" :active-value="1" :inactive-value="0" />
        </el-form-item>
      </el-form>

      <template #footer>
        <el-button @click="dialogVisible = false">取消</el-button>
        <el-button type="primary" @click="addDiagnosis">确定</el-button>
      </template>
    </el-dialog>
  </section>
</template>

<style scoped>
.inpatient-diagnosis-editor {
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  min-height: 100%;
  padding: 16px;
}

.inpatient-diagnosis-editor__content {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.inpatient-diagnosis-editor__table {
  width: 100%;
}

.inpatient-diagnosis-editor__table :deep(.el-table__header-wrapper th.el-table__cell) {
  height: 52px;
  background: var(--el-fill-color-light);
  color: var(--el-text-color-regular);
  font-weight: 600;
}

.inpatient-diagnosis-editor__table :deep(.el-table__row .el-table__cell) {
  height: 56px;
}

.inpatient-diagnosis-editor__table :deep(.el-select) {
  width: 100%;
}

.inpatient-diagnosis-editor__toolbar,
.inpatient-diagnosis-editor__title {
  display: flex;
  align-items: center;
  gap: 10px;
}

.inpatient-diagnosis-editor__toolbar {
  justify-content: space-between;
}

.is-readonly,
.is-saved {
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.is-dirty {
  color: var(--el-color-warning);
  font-size: 13px;
}

.inpatient-diagnosis-editor__skeleton {
  width: 100%;
}
</style>
