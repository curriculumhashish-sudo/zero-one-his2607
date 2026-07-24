<!-- src/views/pharmacy/outpatient-dispense/components/DispenseCenterPanel.vue -->
<template>
  <div class="dispense-center-panel">
    <!-- 处方表格 -->
    <div class="dispense-center-panel__table">
      <AdviceOrderTable
        ref="orderTableRef"
        :rows="displayRows"
        row-key="key"
        :columns="ORDER_COLUMNS"
        :loading="loading"
        :readonly="queueScope === 'completed'"
        selectable
        :status-map="PRESCRIPTION_STATUS_MAP"
        @selection-change="handleSelectionChange"
      >
        <!-- 医嘱内容列使用 AdviceOrderDisplayCell -->
        <template #cell-content="{ row }">
          <AdviceOrderDisplayCell
            :order="row"
            :fields="{ name: 'name', specification: 'specification', dose: 'dose', unit: 'unit', frequency: 'frequency', route: 'route' }"
            compact
          />
        </template>

        <!-- 自定义状态列 -->
        <template #cell-status="{ row }">
          <StatusDisplay
            :status="row.status"
            :status-map="PRESCRIPTION_STATUS_MAP"
            :tag-type-map="PRESCRIPTION_STATUS_TAG_TYPE"
          />
        </template>
      </AdviceOrderTable>
    </div>

    <!-- 底部分汇总 + 药师选择 + 打印 -->
    <div class="dispense-center-panel__footer">
      <div class="dispense-center-panel__footer-left">
        <!-- 处方汇总 -->
        <DescriptionList
          :data="summary"
          :items="PRESCRIPTION_SUMMARY_ITEMS"
          :column="4"
          :border="false"
          size="small"
          label-width="60px"
          dense
        />
      </div>

      <div class="dispense-center-panel__footer-right">
        <!-- 打印 -->
        <PrintActionPanel
          :templates="printTemplates"
          :printers="printers"
          :print="handlePrint"
          :context="printContext"
          :disabled="!hasSelectedPatient || loading"
          show-template-select
          show-printer-select
        />
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, watch } from 'vue'

import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import AdviceOrderDisplayCell from '@/components/medical/AdviceOrderDisplayCell.vue'
import PrintActionPanel from '@/components/common/PrintActionPanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import DescriptionList from '@/components/common/DescriptionList.vue'
import type { PrintActionContext } from '@/components/common/PrintActionPanel.vue'

import {
  ORDER_COLUMNS,
  PRESCRIPTION_STATUS_MAP,
  PRESCRIPTION_SUMMARY_ITEMS,
} from '../constants'
import { buildPrescriptionSummary } from '../adapter'

import type {
  DispensePrescriptionBlock,
  DispensePrescriptionItemRow,
} from '../type'

// ============ Props ============
const props = defineProps<{
  prescriptionBlocks: DispensePrescriptionBlock[]
  loading: boolean
  queueScope: 'pending' | 'completed'
  dispenserOptions: { label: string; value: string }[]
  selectedDispenserId: string | null
  dispenseLoading?: boolean
  prescriptionType?: 'western' | 'chinese'
}>()

// ============ Emits ============
const emit = defineEmits<{
  'update:selectedDispenserId': [value: string | null]
  dispense: [params: { dispenseIds: string[] }]
  traceScan: []
  viewPrescription: []
  print: [context: PrintActionContext]
}>()

// ============ 状态 ============
const selectedRowKeys = ref<string[]>([])
const orderTableRef = ref<InstanceType<typeof AdviceOrderTable> | null>(null)

// 打印相关
const printTemplates = ref([{ value: 'prescription', label: '处方打印模板' }])
const printers = ref([{ value: 'default', label: '默认打印机' }])

// ============ 计算属性 ============
/** 是否有选中的患者 */
const hasSelectedPatient = computed(() => props.prescriptionBlocks.length > 0)

/** 当前显示的处方块（按中西药过滤） */
const filteredBlocks = computed(() => {
  return props.prescriptionBlocks
})

/** 展开的明细行 */
const displayRows = computed(() => {
  const rows: (DispensePrescriptionItemRow & { checked?: boolean })[] = []
  for (const block of filteredBlocks.value) {
    for (const item of block.items) {
      rows.push({
        ...item,
        checked: selectedRowKeys.value.includes(item.id),
      })
    }
  }
  return rows
})

/** 处方汇总 */
const summary = computed(() => {
  return buildPrescriptionSummary(
    filteredBlocks.value,
    props.dispenserOptions.find((d) => d.value === props.selectedDispenserId)?.label || '',
  )
})

/** 打印上下文 */
const printContext = computed(() => ({
  prescriptionBlocks: filteredBlocks.value,
  dispenserName: props.dispenserOptions.find((d) => d.value === props.selectedDispenserId)?.label || '',
}))

/** 处方状态标签类型映射 */
const PRESCRIPTION_STATUS_TAG_TYPE = {
  pending: 'warning',
  partial: 'warning',
  completed: 'success',
  blocking: 'danger',
}

// ============ 方法 ============

/** 处理选择变化 */
function handleSelectionChange(selected: any[]) {
  selectedRowKeys.value = selected.map((row) => row.id)
}

/** 清空勾选 */
function clearSelection() {
  selectedRowKeys.value = []
  ;(orderTableRef.value as any)?.clearSelection?.()
}

/** 获取某行的库房选择 */
function getInventoryPick(lineId: string): string {
  void lineId
  return ''
}

/** 处理打印 */
function handlePrint(context: PrintActionContext) {
  emit('print', context)
}

// ============ 暴露 ============
defineExpose({
  clearSelection,
  getInventoryPick,
  getSelectedIds: () => selectedRowKeys.value,
})

// ============ 监听 ============
watch(
  () => props.prescriptionBlocks,
  () => {
    // 处方数据变化时清空勾选
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
  padding: 0 0 0 0;
}

.dispense-center-panel__table {
  flex: 1;
  min-height: 0;
  overflow: hidden;
  padding: 0 20px;
}

.dispense-center-panel__footer {
  flex-shrink: 0;
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 12px 20px;
  border-top: 1px solid #e5e7eb;
  gap: 12px;
  flex-wrap: wrap;
  background: #fafafa;
}

.dispense-center-panel__footer-left {
  display: flex;
  align-items: center;
  gap: 16px;
  flex: 1;
  flex-wrap: wrap;
}

.dispense-center-panel__footer-right {
  display: flex;
  align-items: center;
  gap: 8px;
  flex-shrink: 0;
}
</style>
