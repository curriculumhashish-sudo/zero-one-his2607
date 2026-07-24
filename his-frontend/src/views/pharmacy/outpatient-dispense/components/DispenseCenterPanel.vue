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
            :fields="{
              name: 'name',
              specification: 'specification',
              dose: 'dose',
              unit: 'unit',
              frequency: 'frequency',
              route: 'route'
            }"
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

        <!-- 修复：发药必须按药品行选择真实的库房和批次，不能使用页面级固定库房。 -->
        <template #cell-inventoryPick="{ row }">
          <el-select
            :model-value="inventoryPickByLineId[String(row.key)]"
            placeholder="请选择库房/批次"
            :disabled="queueScope === 'completed' || !getInventoryOptions(row).length"
            style="width: 100%"
            @update:model-value="(value: string) => setInventoryPick(String(row.key), value)"
          >
            <el-option
              v-for="option in getInventoryOptions(row)"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </template>
      </AdviceOrderTable>
    </div>

    <!-- 底部处方汇总 -->
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
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, watch } from 'vue'

import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import AdviceOrderDisplayCell from '@/components/medical/AdviceOrderDisplayCell.vue'
import type { AdviceOrder } from '@/components/medical/type'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import DescriptionList from '@/components/common/DescriptionList.vue'

import { ORDER_COLUMNS, PRESCRIPTION_STATUS_MAP, PRESCRIPTION_SUMMARY_ITEMS } from '../constants'
import { buildPrescriptionSummary } from '../adapter'

import type { DispensePrescriptionBlock, DispensePrescriptionItemRow } from '../type'

// ============ Props ============
const props = defineProps<{
  prescriptionBlocks: DispensePrescriptionBlock[]
  loading: boolean
  queueScope: 'pending' | 'completed'
  dispenserOptions: { label: string; value: string }[]
  selectedDispenserId: string | null
  dispenseLoading?: boolean
  prescriptionType: 'western' | 'herbal'
}>()

// ============ 状态 ============
const selectedRowKeys = ref<string[]>([])
const orderTableRef = ref<InstanceType<typeof AdviceOrderTable> | null>(null)
const inventoryPickByLineId = ref<Record<string, string>>({})

// ============ 计算属性 ============
/** 当前显示的处方块（按中西药过滤） */
const filteredBlocks = computed(() => {
  // 修复：原实现直接返回全部数据，导致页签只变色但内容不切换。
  return props.prescriptionBlocks.filter((block) => block.rxType === props.prescriptionType)
})

/** 展开的明细行 */
const displayRows = computed(() => {
  const rows: (DispensePrescriptionItemRow & { checked?: boolean })[] = []
  for (const block of filteredBlocks.value) {
    for (const item of block.items) {
      rows.push({
        ...item,
        checked: selectedRowKeys.value.includes(item.id)
      })
    }
  }
  return rows
})

/** 处方汇总 */
const summary = computed(() => {
  return buildPrescriptionSummary(
    filteredBlocks.value,
    props.dispenserOptions.find((d) => d.value === props.selectedDispenserId)?.label || ''
  )
})

/** 处方状态标签类型映射 */
const PRESCRIPTION_STATUS_TAG_TYPE = {
  pending: 'warning',
  partial: 'warning',
  completed: 'success',
  blocking: 'danger'
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
  return inventoryPickByLineId.value[lineId] ?? ''
}

function setInventoryPick(lineId: string, value: string) {
  inventoryPickByLineId.value = { ...inventoryPickByLineId.value, [lineId]: value }
}

function getInventoryOptions(row: AdviceOrder) {
  return (row as DispensePrescriptionItemRow).inventoryPickOptions ?? []
}

// ============ 暴露 ============
defineExpose({
  clearSelection,
  getInventoryPick,
  getSelectedIds: () => selectedRowKeys.value
})

// ============ 监听 ============
watch(
  () => props.prescriptionBlocks,
  (blocks) => {
    // 处方数据变化时清空勾选
    selectedRowKeys.value = []
    // 修复：保留仍然有效的选择；只有唯一库存批次时自动选中，减少重复操作。
    const nextPicks: Record<string, string> = {}
    for (const block of blocks) {
      for (const item of block.items) {
        const current = inventoryPickByLineId.value[item.id]
        if (current && item.inventoryPickOptions.some((option) => option.value === current)) {
          nextPicks[item.id] = current
        } else if (item.inventoryPickOptions.length === 1) {
          nextPicks[item.id] = item.inventoryPickOptions[0]?.value ?? ''
        }
      }
    }
    inventoryPickByLineId.value = nextPicks
  },
  { immediate: true }
)

watch(
  () => props.prescriptionType,
  () => clearSelection()
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
