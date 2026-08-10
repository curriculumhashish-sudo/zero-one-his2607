<script setup lang="ts">
import { ref, watch } from 'vue'

import SelectTable from '@/components/common/SelectTable.vue'
import type { SelectTableColumn, SelectValue } from '@/components/common/select-type'

import type { ClinicalDiagnosisCandidate, ClinicalDiagnosisDictionaryLoader } from './type'

defineOptions({
  name: 'DiagnosisSelectorDialog'
})

const props = withDefaults(
  defineProps<{
    /** 由具体业务页面注入的诊断字典加载函数 */
    loadPage: ClinicalDiagnosisDictionaryLoader
    /** 已经添加的字典 ID；这些行会显示但不可重复选择 */
    existingCatalogIds?: string[]
  }>(),
  {
    existingCatalogIds: () => []
  }
)

const emit = defineEmits<{
  confirm: [rows: ClinicalDiagnosisCandidate[]]
  loadError: [error: unknown]
}>()

const visible = defineModel<boolean>('visible', { default: false })
const selectedIds = ref<SelectValue[]>([])

const columns: SelectTableColumn<ClinicalDiagnosisCandidate>[] = [
  { prop: 'code', label: '诊断编码', width: 150 },
  { prop: 'name', label: '诊断名称', minWidth: 240 },
  { prop: 'typeName', label: '诊断分类', width: 130 }
]

function isExistingDiagnosis(row: unknown) {
  return props.existingCatalogIds.includes((row as ClinicalDiagnosisCandidate).id)
}

function confirmSelection(
  _value: SelectValue | SelectValue[] | null,
  rows: ClinicalDiagnosisCandidate[]
) {
  emit('confirm', rows)
}

// 每次打开都从空草稿开始，避免上一次确认的选择残留在弹窗中。
watch(visible, (isVisible) => {
  if (isVisible) selectedIds.value = []
})
</script>

<template>
  <SelectTable
    v-model="selectedIds"
    v-model:visible="visible"
    title="选择诊断"
    selection-mode="multiple"
    row-key="id"
    :columns="columns"
    :load-page="loadPage"
    :disabled-row="isExistingDiagnosis"
    keyword-placeholder="请输入诊断编码或诊断名称"
    search-label="诊断"
    empty-text="暂无匹配诊断"
    @confirm="confirmSelection"
    @load-error="emit('loadError', $event)"
  >
    <template #cell-code="{ row }">
      <el-tag effect="plain">{{ row.code }}</el-tag>
    </template>
    <template #cell-name="{ row }">
      <span>{{ row.name }}</span>
      <el-tag v-if="isExistingDiagnosis(row)" class="diagnosis-selector__added" type="info">
        已添加
      </el-tag>
    </template>
  </SelectTable>
</template>

<style scoped>
.diagnosis-selector__added {
  margin-left: 8px;
}
</style>
