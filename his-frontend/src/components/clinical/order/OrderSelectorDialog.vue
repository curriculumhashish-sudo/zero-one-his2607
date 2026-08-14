<script setup lang="ts">
import { ref, watch } from 'vue'

import SelectTable from '@/components/common/SelectTable.vue'
import type { SelectTableColumn, SelectValue } from '@/components/common/select-type'

import type { ClinicalOrderCandidate, ClinicalOrderCatalogLoader } from './type'

defineOptions({ name: 'OrderSelectorDialog' })

const props = withDefaults(
  defineProps<{
    /** 场景页面注入的医嘱项目加载函数 */
    loadPage: ClinicalOrderCatalogLoader
    /** 当前已添加的项目 ID，用于防止重复添加 */
    existingCatalogIds?: string[]
  }>(),
  {
    existingCatalogIds: () => []
  }
)

const emit = defineEmits<{
  confirm: [rows: ClinicalOrderCandidate[]]
  loadError: [error: unknown]
}>()

const visible = defineModel<boolean>('visible', { default: false })
const selectedIds = ref<SelectValue[]>([])

const columns: SelectTableColumn<ClinicalOrderCandidate>[] = [
  { prop: 'code', label: '项目编码', width: 140 },
  { prop: 'name', label: '项目名称', minWidth: 220 },
  { prop: 'categoryName', label: '项目分类', width: 110 },
  { prop: 'specification', label: '规格', minWidth: 160 }
]

function isExistingOrder(row: unknown) {
  return props.existingCatalogIds.includes((row as ClinicalOrderCandidate).id)
}

function confirmSelection(
  _value: SelectValue | SelectValue[] | null,
  rows: ClinicalOrderCandidate[]
) {
  emit('confirm', rows)
}

watch(visible, (isVisible) => {
  if (isVisible) selectedIds.value = []
})
</script>

<template>
  <SelectTable
    v-model="selectedIds"
    v-model:visible="visible"
    title="选择医嘱项目"
    selection-mode="multiple"
    row-key="id"
    :columns="columns"
    :load-page="loadPage"
    :disabled-row="isExistingOrder"
    keyword-placeholder="请输入项目编码或名称"
    search-label="医嘱项目"
    empty-text="暂无匹配医嘱项目"
    @confirm="confirmSelection"
    @load-error="emit('loadError', $event)"
  >
    <template #cell-code="{ row }">
      <el-tag effect="plain">{{ row.code }}</el-tag>
    </template>
    <template #cell-categoryName="{ row }">
      <el-tag type="info" effect="light">{{ row.categoryName }}</el-tag>
    </template>
    <template #cell-name="{ row }">
      <span>{{ row.name }}</span>
      <el-tag v-if="isExistingOrder(row)" class="order-selector__added" type="info">
        已添加
      </el-tag>
    </template>
  </SelectTable>
</template>

<style scoped>
.order-selector__added {
  margin-left: 8px;
}
</style>
