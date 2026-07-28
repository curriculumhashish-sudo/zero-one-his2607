<!-- src/views/pharmacy/outpatient-dispense/components/DispenseCenterPanel.vue -->
<template>
  <div class="dispense-center-panel">
    <div class="dispense-center-panel__table">
      <el-table
        ref="orderTableRef"
        :data="displayRows"
        row-key="key"
        :loading="loading"
        @selection-change="handleSelectionChange"
        style="width: 100%;"
      >
        <el-table-column
          type="selection"
          width="44"
          :selectable="() => queueScope !== 'completed'"
        />
        <el-table-column
          prop="rxNo"
          label="处方号"
          width="160"
          fixed="left"
        />
        <el-table-column
          prop="status"
          label="状态"
          width="80"
          fixed="left"
        >
          <template #default="{ row }">
            <StatusDisplay
              :value="row.status"
              :options="statusOptions"
            />
          </template>
        </el-table-column>
        <el-table-column
          v-for="column in dynamicColumns"
          :key="column.prop"
          :prop="column.prop"
          :label="column.label"
          :width="column.width"
          :min-width="column.minWidth"
        />
      </el-table>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, watch } from 'vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import { ORDER_COLUMNS } from '../constants'
import type { DispensePrescriptionBlock, DispensePrescriptionItemRow } from '../type'

const props = defineProps<{
  prescriptionBlocks: DispensePrescriptionBlock[]
  loading: boolean
  queueScope: 'pending' | 'completed'
  prescriptionType?: 'western' | 'chinese'
}>()

const selectedRowKeys = ref<string[]>([])
const orderTableRef = ref<any>(null)

const filteredBlocks = computed(() => {
  const type = props.prescriptionType
  if (!type) return props.prescriptionBlocks

  return props.prescriptionBlocks.map(block => ({
    ...block,
    items: block.items.filter(item => item.medicationType === type)
  })).filter(block => block.items.length > 0)
})

const displayRows = computed(() => {
  const rows: DispensePrescriptionItemRow[] = []
  for (const block of filteredBlocks.value) {
    for (const item of block.items) {
      rows.push({ ...item })
    }
  }
  return rows
})

const dynamicColumns = computed(() => {
  return ORDER_COLUMNS.filter(col => col.prop !== 'rxNo' && col.prop !== 'status')
})

const statusOptions = [
  { value: 'pending', label: '待发药', type: 'warning' as const },
  { value: 'partial', label: '部分可发', type: 'warning' as const },
  { value: 'completed', label: '已发药', type: 'success' as const },
  { value: 'blocking', label: '拦截', type: 'danger' as const }
]

function handleSelectionChange(selected: any[]) {
  selectedRowKeys.value = selected.map((row: any) => String(row.key))
}

function clearSelection() {
  selectedRowKeys.value = []
  orderTableRef.value?.clearSelection()
}

defineExpose({
  clearSelection,
  getSelectedIds: () => selectedRowKeys.value
})

watch(
  () => props.prescriptionBlocks,
  () => {
    selectedRowKeys.value = []
  },
)
</script>

<style scoped>
.dispense-center-panel {
  display: flex;
  flex: 1;
  flex-direction: column;
  min-height: 0;
  background: #ffffff;
  padding: 0;
}

.dispense-center-panel__table {
  flex: 1;
  min-height: 0;
  overflow: auto;
  padding: 0 20px;
}
</style>
