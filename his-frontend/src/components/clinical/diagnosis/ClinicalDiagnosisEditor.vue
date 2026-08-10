<script setup lang="ts">
import { computed, ref } from 'vue'
import { Delete, Plus } from '@element-plus/icons-vue'

import EmptyState from '@/components/common/EmptyState.vue'

import DiagnosisSelectorDialog from './DiagnosisSelectorDialog.vue'
import type {
  ClinicalDiagnosisCandidate,
  ClinicalDiagnosisDictionaryLoader,
  ClinicalDiagnosisItem
} from './type'
import { addClinicalDiagnoses, removeClinicalDiagnosis, setPrimaryClinicalDiagnosis } from './utils'

defineOptions({
  name: 'ClinicalDiagnosisEditor'
})

const props = withDefaults(
  defineProps<{
    /** 受控诊断列表，组件不会直接修改这个数组 */
    modelValue?: ClinicalDiagnosisItem[]
    /** 是否已经选择患者 */
    patientSelected?: boolean
    /** 当前患者是否允许诊疗编辑 */
    accepted?: boolean
    /** 完诊或无权限时设为只读 */
    readonly?: boolean
    /** 是否允许手动设置主诊断；为 false 时仅只读展示主诊断标签 */
    allowSetPrimary?: boolean
    /** 诊断列表是否正在加载 */
    loading?: boolean
    /** 诊断列表加载失败信息 */
    error?: string
    /** 当前列表是否存在未保存修改 */
    dirty?: boolean
    /** 是否由场景页面在变更后自动保存。 */
    autoSave?: boolean
    /** 最近保存时间 */
    updatedAt?: string
    /** 门诊或住院页面提供的诊断字典加载函数 */
    loadDictionary?: ClinicalDiagnosisDictionaryLoader
  }>(),
  {
    modelValue: () => [],
    patientSelected: false,
    accepted: false,
    readonly: false,
    allowSetPrimary: true,
    loading: false,
    error: '',
    dirty: false,
    autoSave: false,
    updatedAt: '',
    loadDictionary: undefined
  }
)

const emit = defineEmits<{
  'update:modelValue': [items: ClinicalDiagnosisItem[]]
  change: [items: ClinicalDiagnosisItem[]]
  retry: []
  openHistory: []
  dictionaryError: [error: unknown]
}>()

const selectorVisible = ref(false)
const canDisplayDiagnoses = computed(() => props.accepted || props.readonly)
const existingCatalogIds = computed(() => props.modelValue.map((item) => item.catalogId))

/** 所有编辑动作最终都通过同一个函数向父页面提交新数组。 */
function commit(items: ClinicalDiagnosisItem[]) {
  emit('update:modelValue', items)
  emit('change', items)
}

function addDiagnoses(candidates: ClinicalDiagnosisCandidate[]) {
  commit(addClinicalDiagnoses(props.modelValue, candidates))
}

function removeDiagnosis(row: unknown) {
  const item = row as ClinicalDiagnosisItem
  commit(removeClinicalDiagnosis(props.modelValue, item.key))
}

function setPrimary(row: unknown) {
  const item = row as ClinicalDiagnosisItem
  if (item.primary) return
  commit(setPrimaryClinicalDiagnosis(props.modelValue, item.key))
}
</script>

<template>
  <section class="clinical-diagnosis-editor">
    <EmptyState
      v-if="!patientSelected"
      title="请选择患者"
      description="从患者队列选择患者后，可查看本次诊断"
    />

    <div v-else-if="loading" class="clinical-diagnosis-editor__skeleton">
      <el-skeleton :rows="9" animated />
    </div>

    <EmptyState
      v-else-if="error"
      title="诊断加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <EmptyState
      v-else-if="!canDisplayDiagnoses"
      title="请先接诊患者"
      :description="
        autoSave ? '接诊后可新增和调整诊断，修改会自动保存' : '接诊后才能新增、调整并保存本次诊断'
      "
    />

    <div v-else class="clinical-diagnosis-editor__content">
      <header class="clinical-diagnosis-editor__toolbar">
        <div class="clinical-diagnosis-editor__title">
          <strong>本次诊断</strong>
          <el-tag size="small" effect="plain">{{ modelValue.length }}</el-tag>
          <span v-if="readonly" class="is-readonly">已完诊，只读</span>
          <span v-else-if="dirty" class="is-dirty">
            {{ autoSave ? '待自动保存' : '有未保存修改' }}
          </span>
          <span v-else class="is-saved">{{ autoSave ? '已自动保存' : '内容已保存' }}</span>
        </div>

        <div class="clinical-diagnosis-editor__actions">
          <span v-if="updatedAt" class="clinical-diagnosis-editor__time">
            最近保存：{{ updatedAt }}
          </span>
          <el-button @click="emit('openHistory')">历史诊断</el-button>
          <el-button
            v-if="!readonly"
            type="primary"
            :icon="Plus"
            :disabled="!loadDictionary"
            @click="selectorVisible = true"
          >
            新增诊断
          </el-button>
        </div>
      </header>

      <el-table
        :data="modelValue"
        row-key="key"
        border
        stripe
        class="clinical-diagnosis-editor__table"
      >
        <el-table-column type="index" label="序号" width="64" align="center" />
        <el-table-column label="主诊断" width="110" align="center">
          <template #default="{ row }">
            <el-tag v-if="row.primary" type="danger" effect="light">主诊断</el-tag>
            <el-button
              v-else-if="allowSetPrimary && !readonly"
              type="primary"
              link
              @click="setPrimary(row)"
            >
              设为主诊断
            </el-button>
            <span v-else>—</span>
          </template>
        </el-table-column>
        <el-table-column prop="code" label="诊断编码" width="150" />
        <el-table-column prop="name" label="诊断名称" min-width="220" />
        <el-table-column prop="typeName" label="诊断分类" width="130" />
        <el-table-column prop="diagnosedAt" label="诊断时间" min-width="170">
          <template #default="{ row }">{{ row.diagnosedAt || '保存后生成' }}</template>
        </el-table-column>
        <el-table-column v-if="!readonly" label="操作" width="90" fixed="right">
          <template #default="{ row }">
            <el-button type="danger" link :icon="Delete" @click="removeDiagnosis(row)">
              删除
            </el-button>
          </template>
        </el-table-column>
        <template #empty>
          <el-empty description="暂无诊断，请点击“新增诊断”" :image-size="88" />
        </template>
      </el-table>

      <el-alert
        v-if="!readonly"
        :title="
          allowSetPrimary
            ? '保存前至少需要一条诊断，并且只能设置一个主诊断。'
            : '保存前至少需要一条诊断；新增的第一条诊断自动作为主诊断。'
        "
        type="info"
        :closable="false"
        show-icon
      />
    </div>

    <DiagnosisSelectorDialog
      v-if="loadDictionary && !readonly"
      v-model:visible="selectorVisible"
      :load-page="loadDictionary"
      :existing-catalog-ids="existingCatalogIds"
      @confirm="addDiagnoses"
      @load-error="emit('dictionaryError', $event)"
    />
  </section>
</template>

<style scoped>
.clinical-diagnosis-editor {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  min-height: 100%;
  justify-content: center;
  color: #303133;
  background: #ffffff;
}

.clinical-diagnosis-editor__skeleton,
.clinical-diagnosis-editor__content {
  box-sizing: border-box;
  width: 100%;
  padding: 16px;
}

.clinical-diagnosis-editor__content {
  display: flex;
  min-height: 0;
  flex-direction: column;
  gap: 12px;
}

.clinical-diagnosis-editor__toolbar,
.clinical-diagnosis-editor__title,
.clinical-diagnosis-editor__actions {
  display: flex;
  align-items: center;
}

.clinical-diagnosis-editor__toolbar {
  justify-content: space-between;
  gap: 16px;
}

.clinical-diagnosis-editor__title,
.clinical-diagnosis-editor__actions {
  gap: 10px;
}

.clinical-diagnosis-editor__title strong {
  color: #303133;
  font-weight: 700;
}

.clinical-diagnosis-editor__title .is-readonly,
.clinical-diagnosis-editor__time {
  color: #606266;
  font-size: 12px;
}

.clinical-diagnosis-editor__title .is-dirty {
  color: var(--el-color-warning);
  font-size: 13px;
}

.clinical-diagnosis-editor__title .is-saved {
  color: var(--el-color-success);
  font-size: 13px;
}

.clinical-diagnosis-editor__table {
  --el-table-text-color: #303133;
  --el-table-header-text-color: #606266;
  --el-table-border-color: #dcdfe6;
  --el-table-header-bg-color: #f5f7fa;
  --el-table-row-hover-bg-color: #f5f7fa;
  width: 100%;
  color: #303133;
}

/* 明确覆盖表格内部文字，避免 Chrome 从宿主主题继承过浅的颜色变量。 */
.clinical-diagnosis-editor__table :deep(.el-table__cell),
.clinical-diagnosis-editor__table :deep(.cell) {
  color: #303133;
}

.clinical-diagnosis-editor__table :deep(.el-table__header .el-table__cell),
.clinical-diagnosis-editor__table :deep(.el-table__header .cell) {
  color: #606266;
  font-weight: 600;
}

.clinical-diagnosis-editor :deep(.el-alert__title),
.clinical-diagnosis-editor :deep(.el-alert__description) {
  color: #606266;
}

@media (max-width: 1100px) {
  .clinical-diagnosis-editor__toolbar,
  .clinical-diagnosis-editor__actions {
    align-items: flex-start;
    flex-direction: column;
  }
}
</style>
