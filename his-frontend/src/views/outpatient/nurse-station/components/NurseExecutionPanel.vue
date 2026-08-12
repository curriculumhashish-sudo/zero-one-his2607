<script setup lang="ts">
import { computed, ref } from 'vue'

import type { NurseExecutionItemVO } from '@/apis/outpatient/nurse-station'
import EmptyState from '@/components/common/EmptyState.vue'
import SectionTitle from '@/components/common/SectionTitle.vue'
import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import type { AdviceColumn, AdviceOrder } from '@/components/medical/type'

import { executionStatusTagType } from '../adapter'

defineOptions({
  name: 'NurseExecutionPanel'
})

interface Props {
  items?: NurseExecutionItemVO[]
  loading?: boolean
  hasPatient?: boolean
  printingItemId?: string
}

const props = withDefaults(defineProps<Props>(), {
  items: () => [],
  loading: false,
  hasPatient: false,
  printingItemId: ''
})

const emit = defineEmits<{
  cancelExecute: [item: NurseExecutionItemVO]
  print: [item: NurseExecutionItemVO]
  selectionChange: [ids: string[]]
  refresh: []
}>()

interface NurseExecutionAdviceOrder extends AdviceOrder {
  categoryName: string
  executionCountText: string
  executorName: string
  executedAt: string
  raw: NurseExecutionItemVO
}

const tableRef = ref<InstanceType<typeof AdviceOrderTable> | null>(null)

const columns: AdviceColumn[] = [
  { prop: 'categoryName', label: '类型', width: 80, align: 'center' },
  { prop: 'name', label: '项目名称', minWidth: 190, showOverflowTooltip: true },
  { prop: 'specification', label: '数量/单位', minWidth: 120, showOverflowTooltip: true },
  { prop: 'executionCountText', label: '执行次数', width: 96, align: 'center' },
  { prop: 'status', label: '状态', width: 92, align: 'center' },
  { prop: 'executorName', label: '执行人', width: 96, align: 'center' },
  { prop: 'executedAt', label: '执行时间', width: 150, align: 'center' }
]

const adviceRows = computed<NurseExecutionAdviceOrder[]>(() =>
  props.items.map((item) => ({
    key: item.itemId,
    name: item.itemName,
    specification: [item.quantity, item.unitName].filter(Boolean).join(' ') || item.remark,
    status: item.status,
    categoryName: item.categoryName || item.categoryCode || '—',
    executionCountText: String(item.executionCount),
    executorName: item.executorName || '—',
    executedAt: item.executedAt || '—',
    raw: item
  }))
)

function disabledRow(row: AdviceOrder) {
  return row.status !== 'PENDING'
}

function handleSelectionChange(rows: AdviceOrder[]) {
  emit(
    'selectionChange',
    rows.filter((row) => !disabledRow(row)).map((row) => String(row.key))
  )
}

function clearSelection() {
  tableRef.value?.clearSelection()
}

defineExpose({
  clearSelection
})

function executionItem(row: AdviceOrder): NurseExecutionItemVO {
  return (row as NurseExecutionAdviceOrder).raw
}
</script>

<template>
  <section class="nurse-execution-panel">
    <header class="nurse-execution-panel__header">
      <SectionTitle title="执行项目" />
      <el-button size="small" :loading="loading" :disabled="!hasPatient" @click="emit('refresh')">
        刷新
      </el-button>
    </header>

    <div v-if="!hasPatient" class="nurse-execution-panel__empty">
      <EmptyState compact title="请选择患者" description="选择患者后查看其待执行项目" />
    </div>

    <AdviceOrderTable
      v-else
      ref="tableRef"
      class="nurse-execution-panel__table"
      :rows="adviceRows"
      :columns="columns"
      :loading="loading"
      :disabled-row="disabledRow"
      border
      stripe
      selectable
      height="100%"
      empty-text="当前患者暂无执行项目"
      @selection-change="handleSelectionChange"
    >
      <template #cell-status="{ row }">
        <el-tag :type="executionStatusTagType(executionItem(row).status)" size="small">
          {{ executionItem(row).statusName }}
        </el-tag>
      </template>
      <template #actions="{ row }">
        <div class="nurse-execution-panel__actions">
          <el-button
            link
            type="primary"
            :loading="printingItemId === executionItem(row).itemId"
            :disabled="Boolean(printingItemId)"
            @click="emit('print', executionItem(row))"
          >
            打印
          </el-button>
          <el-button
            v-if="executionItem(row).status === 'COMPLETED'"
            link
            type="danger"
            @click="emit('cancelExecute', executionItem(row))"
          >
            取消执行
          </el-button>
        </div>
      </template>
    </AdviceOrderTable>
  </section>
</template>

<style scoped>
.nurse-execution-panel {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
}

.nurse-execution-panel__header {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: space-between;
  min-height: 48px;
  padding: 8px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.nurse-execution-panel__empty {
  display: grid;
  flex: 1;
  place-items: center;
}

.nurse-execution-panel__table {
  min-height: 0;
  flex: 1;
  border: 0;
  border-radius: 0;
}

.nurse-execution-panel__table :deep(.el-table__header-wrapper th.el-table__cell) {
  border-bottom-color: var(--el-color-primary-light-7);
  color: var(--el-text-color-primary, #303133);
  background: var(--el-color-primary-light-9);
  font-weight: 600;
}

.nurse-execution-panel__table :deep(.el-table__header-wrapper .cell) {
  color: inherit;
}

.nurse-execution-panel__actions {
  display: inline-flex;
  flex-direction: column;
  align-items: flex-start;
  gap: 2px;
}

.nurse-execution-panel__actions :deep(.el-button) {
  margin-left: 0;
}
</style>
