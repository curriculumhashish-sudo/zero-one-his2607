<template>
  <DocumentWorkstationShell :has-selection="true">
    <!-- 左侧标题 -->
    <template #queue-header>
      <span class="queue-header-title">历史进货单</span>
    </template>

    <!-- 左侧 Queue 区 -->
    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="请选择库房"
        :dropdown1-options="queueDropdown1Options"
        dropdown2-placeholder="请选择目的药房"
        :dropdown2-options="queueDropdown2Options"
        :show-dropdown2="false"
        add-button-text="新增单据"
        @select="handleQueueSelect"
        @create="selectedOrder = null"
        @reset="selectedOrder = null"
      />
    </template>

    <!-- 右侧 Main 区 -->
    <template #main>
      <PurchaseInboundMainPane
        :order-id="selectedOrder?.id"
        title="进货单信息"
        :extra-search-fields="mainSearchFields"
        :search-disabled="true"
        :form-columns="formColumns"
        :row-defaults="rowDefaults"
        :select-table-columns="selectTableColumns"
        :select-table-load-page="loadSupplierOptions"
        :on-supplier-fill="onSupplierFill"
        :hidden-footer-actions="['approve', 'reject']"
      />
    </template>
  </DocumentWorkstationShell>
</template>

<script setup lang="ts">
import { ref, reactive, onMounted } from 'vue'
import DocumentWorkstationShell from '@/components/warehouse/DocumentWorkstationShell.vue'
import PurchaseInboundQueuePane from '@/views/inventory/components/PurchaseInboundQueuePane.vue'
import PurchaseInboundMainPane from '@/views/inventory/components/PurchaseInboundMainPane.vue'
import type { ExtraSearchField } from '@/views/inventory/components/PurchaseInboundMainPane.vue'
import { getFormConfig, loadSupplierOptions } from '@/apis/inventory/warehouse/purchase-inbound2'
import type { PurchaseInboundOrder } from '@/apis/inventory/warehouse/purchase-inbound2/type'
import { computeBatchNo, computeActualAmount } from './utils'

// ==================== 库房列表 ====================

const drugSourceDatabase = [
  { label: '耗材库', value: '耗材库' },
  { label: '西药库', value: '西药库' },
  { label: '中药库', value: '中药库' }
]

// ==================== 选中状态 ====================

const selectedOrder = ref<PurchaseInboundOrder | null>(null)

function handleQueueSelect(order: Record<string, unknown>) {
  selectedOrder.value = order as unknown as PurchaseInboundOrder
}

// ==================== 左侧 Queue 区下拉菜单配置 ====================

const queueDropdown1Options = drugSourceDatabase
const queueDropdown2Options: { label: string; value: string }[] = []

// ==================== 右侧 Main 区检索配置 ====================

const mainSearchFields = reactive<ExtraSearchField[]>([
  {
    label: '单据号',
    prop: 'docNo',
    type: 'input',
    placeholder: '未选择'
  },
  {
    label: '供应商',
    prop: 'supplier',
    type: 'select',
    placeholder: '请选择',
    options: []
  },
  {
    label: '制单时间',
    prop: 'createTime',
    type: 'datetime',
    placeholder: '选择日期时间'
  },
  {
    label: '库房',
    prop: 'warehouse',
    type: 'select',
    placeholder: '请选择',
    options: drugSourceDatabase
  },
  {
    label: '项目类型',
    prop: 'itemType',
    type: 'select',
    placeholder: '请选择',
    options: [
      { label: '药库', value: '药库' },
      { label: '耗材', value: '耗材' }
    ]
  }
])

// ==================== Main 区表单配置 ====================

const formColumns = ref<any[]>([])
const rowDefaults = ref<Record<string, any>>({})
const selectTableColumns = ref<any[]>([])

/** SelectTable 回填业务逻辑 */
function onSupplierFill(row: Record<string, any>, item: Record<string, any>) {
  const totalAmount = computeActualAmount(item.purchasePrice, item.stockQuantity)
  return {
    ...row,
    itemName: item.itemName,
    specification: item.specification,
    manufacturer: item.manufacturer,
    batchNo: computeBatchNo(item.itemCode),
    quantity: item.stockQuantity,
    unitPrice: item.purchasePrice,
    totalAmount,
    remark: ''
  }
}

onMounted(async () => {
  try {
    const config = await getFormConfig()
    formColumns.value = config.columns
    rowDefaults.value = config.rowDefaults
    selectTableColumns.value = config.selectTableColumns
  } catch {
    // 加载失败使用默认空值
  }
})
</script>

<style scoped>
.queue-header-title {
  font-size: 16px;
  font-weight: 600;
  color: var(--el-text-color-primary);
  position: relative;
  padding-left: 10px;
}

.queue-header-title::before {
  position: absolute;
  top: 50%;
  left: 0;
  width: 3px;
  height: 14px;
  background: var(--el-color-primary);
  border-radius: 2px;
  content: '';
  transform: translateY(-50%);
}
</style>
