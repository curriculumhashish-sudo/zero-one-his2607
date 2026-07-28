<template>
  <div class="return-medicine-center-panel">
    <div class="return-medicine-center-panel__table">
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
          :selectable="(row: any) => row.status !== 'completed'"
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
        <el-table-column
          label="退药原因"
          width="200"
        >
          <template #default="{ row }">
            <el-input
              v-if="row.status !== 'completed'"
              v-model="reasons[row.key]"
              placeholder="请输入退药原因"
              size="small"
              @input="handleReasonsChange"
            />
            <span v-else class="reason-text">{{ row.reason || '-' }}</span>
          </template>
        </el-table-column>
      </el-table>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, watch, reactive } from 'vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import { ORDER_COLUMNS } from '../constants'
import type { ReturnMedicinePrescriptionBlock, ReturnMedicinePrescriptionItemRow } from '../type'

const props = defineProps<{
  prescriptionBlocks: ReturnMedicinePrescriptionBlock[]
  loading: boolean
}>()

const emit = defineEmits<{
  (e: 'update:reasons', reasons: Record<string, string>): void
}>()

const selectedRowKeys = ref<string[]>([])
const orderTableRef = ref<any>(null)
const reasons = reactive<Record<string, string>>({})

const displayRows = computed(() => {
  const rows: ReturnMedicinePrescriptionItemRow[] = []
  for (const block of props.prescriptionBlocks) {
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
  { value: 'pending', label: '待退药', type: 'warning' as const },
  { value: 'partial', label: '部分退药', type: 'info' as const },
  { value: 'completed', label: '已退药', type: 'success' as const },
  { value: 'blocking', label: '拦截', type: 'danger' as const }
]

function handleSelectionChange(selected: any[]) {
  selectedRowKeys.value = selected.map((row: any) => String(row.key))
}

function handleReasonsChange() {
  emit('update:reasons', { ...reasons })
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
.return-medicine-center-panel {
  display: flex;
  flex: 1;
  flex-direction: column;
  min-height: 0;
  background: #ffffff;
  padding: 0;
}

.return-medicine-center-panel__table {
  flex: 1;
  min-height: 0;
  overflow: auto;
  padding: 0 20px;
}

.reason-text {
  font-size: 13px;
  color: #666;
}
</style>