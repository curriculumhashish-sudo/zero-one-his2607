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
}

const props = withDefaults(defineProps<Props>(), {
  items: () => [],
  loading: false,
  hasPatient: false
})

const emit = defineEmits<{
  cancelExecute: [item: NurseExecutionItemVO]
  selectionChange: [ids: string[]]
  refresh: []
}>()

interface NurseExecutionAdviceOrder extends AdviceOrder {
  requestTypeName: string
  executedCountText: string
  executorName: string
  executedAt: string
  raw: NurseExecutionItemVO
}

const tableRef = ref<InstanceType<typeof AdviceOrderTable> | null>(null)

const columns: AdviceColumn[] = [
  { prop: 'requestTypeName', label: '类型', width: 80, align: 'center' },
  { prop: 'name', label: '医嘱内容', minWidth: 190, showOverflowTooltip: true },
  { prop: 'specification', label: '规格/剂量', minWidth: 180, showOverflowTooltip: true },
  { prop: 'executedCountText', label: '执行次数', width: 96, align: 'center' },
  { prop: 'status', label: '状态', width: 92, align: 'center' },
  { prop: 'executorName', label: '执行人', width: 96, align: 'center' },
  { prop: 'executedAt', label: '执行时间', width: 150, align: 'center' }
]

const adviceRows = computed<NurseExecutionAdviceOrder[]>(() =>
  props.items.map((item) => ({
    key: item.itemId,
    name: item.orderContent,
    specification: item.specification,
    status: item.status,
    requestTypeName: item.requestTypeName,
    executedCountText: `${item.executedCount}/${item.plannedCount}`,
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
        <el-button
          v-if="executionItem(row).executedCount > 0"
          link
          type="danger"
          @click="emit('cancelExecute', executionItem(row))"
        >
          取消执行
        </el-button>
        <span v-else>—</span>
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
</style>
