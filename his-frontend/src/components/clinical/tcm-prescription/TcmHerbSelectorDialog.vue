<script setup lang="ts">
import { ref, watch } from 'vue'

import type { TcmHerbCatalogItemVO } from '@/apis/clinical/tcm-prescription'
import SelectTable from '@/components/common/SelectTable.vue'
import type { SelectTableColumn, SelectValue } from '@/components/common/select-type'
import type { TcmHerbCatalogLoader } from './type'

defineOptions({ name: 'TcmHerbSelectorDialog' })

const props = withDefaults(
  defineProps<{
    loadPage: TcmHerbCatalogLoader
    existingCatalogIds?: string[]
  }>(),
  {
    existingCatalogIds: () => []
  }
)

const emit = defineEmits<{
  confirm: [rows: TcmHerbCatalogItemVO[]]
  loadError: [error: unknown]
}>()

const visible = defineModel<boolean>('visible', { default: false })
const selectedIds = ref<SelectValue[]>([])

const columns: SelectTableColumn<TcmHerbCatalogItemVO>[] = [
  { prop: 'code', label: '药品编码', width: 130 },
  { prop: 'name', label: '药材名称', minWidth: 180 },
  { prop: 'specification', label: '规格', width: 120 },
  { prop: 'defaultDose', label: '默认剂量', width: 100, align: 'right' },
  { prop: 'retailPrice', label: '单价', width: 90, align: 'right' }
]

function isExisting(row: unknown) {
  return props.existingCatalogIds.includes((row as TcmHerbCatalogItemVO).id)
}

watch(visible, (value) => {
  if (value) selectedIds.value = []
})
</script>

<template>
  <SelectTable
    v-model="selectedIds"
    v-model:visible="visible"
    title="选择中药材"
    selection-mode="multiple"
    row-key="id"
    :columns="columns"
    :load-page="loadPage"
    :disabled-row="isExisting"
    keyword-placeholder="请输入药材编码或名称"
    search-label="中药材"
    empty-text="暂无匹配的中药材"
    @confirm="
      (_value: SelectValue | SelectValue[] | null, rows: TcmHerbCatalogItemVO[]) =>
        emit('confirm', rows)
    "
    @load-error="emit('loadError', $event)"
  >
    <template #cell-code="{ row }">
      <el-tag effect="plain">{{ row.code }}</el-tag>
    </template>
    <template #cell-name="{ row }">
      <span>{{ row.name }}</span>
      <el-tag v-if="isExisting(row)" class="tcm-herb-selector__added" type="info"> 已添加 </el-tag>
    </template>
    <template #cell-defaultDose="{ row }"> {{ row.defaultDose }}{{ row.doseUnit }} </template>
    <template #cell-retailPrice="{ row }"> ¥{{ Number(row.retailPrice).toFixed(2) }} </template>
  </SelectTable>
</template>

<style scoped>
.tcm-herb-selector__added {
  margin-left: 8px;
}
</style>
