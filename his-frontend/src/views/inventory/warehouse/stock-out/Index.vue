<template>
  <DocumentWorkstationShell :has-selection="true">
    <!-- 左侧标题 -->
    <template #queue-header>
      <span class="queue-header-title">历史出库单</span>
    </template>

    <!-- 左侧 Queue 区 -->
    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="请选择药源库"
        :dropdown1-options="queueDropdown1Options"
        dropdown2-placeholder="请选择目的药房"
        :dropdown2-options="queueDropdown2Options"
        :show-dropdown2="true"
        add-button-text="新增单据"
        @select="selectedOrder = $event"
        @create="selectedOrder = null"
        @reset="selectedOrder = null"
      />
    </template>

    <!-- 右侧 Main 区 -->
    <template #main>
      <PurchaseInboundMainPane
        :order-id="selectedOrder?.id"
        title="出库单信息"
        :extra-search-fields="mainSearchFields"
        :form-columns="formColumns"
        :row-defaults="rowDefaults"
        :select-table-columns="selectTableColumns"
        :select-table-load-page="emptySelectTableLoad"
        :on-supplier-fill="onSupplierFill"
        :on-quantity-change="onQuantityChange"
        :show-requisition-button="true"
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
import { getFormConfig } from '@/apis/inventory/warehouse/stock-out'
import type { PurchaseInboundOrder } from '@/apis/inventory/warehouse/stock-out/type'
import { computeBatchNo, computeActualAmount } from './utils'

// ==================== 药房列表（静态数据） ====================

const drugSourceDatabase = [
  { label: '耗材库', value: '耗材库' },
  { label: '西药库', value: '西药库' },
  { label: '中药库', value: '中药库' }
]

const pharmacyOptions = [
  { label: '门诊西药房', value: '门诊西药房' },
  { label: '门诊中药房', value: '门诊中药房' },
  { label: '门诊耗材房', value: '门诊耗材房' },
  { label: '住院西药房', value: '住院西药房' },
  { label: '住院中药房', value: '住院中药房' },
  { label: '住院耗材房', value: '住院耗材房' }
]

// ==================== 选中状态 ====================

const selectedOrder = ref<PurchaseInboundOrder | null>(null)

// ==================== 左侧 Queue 区下拉菜单配置 ====================

const queueDropdown1Options = drugSourceDatabase
const queueDropdown2Options = pharmacyOptions

// ==================== 右侧 Main 区检索配置 ====================

const mainSearchFields = reactive<ExtraSearchField[]>([
  {
    label: '单据号',
    prop: 'docNo',
    type: 'input',
    placeholder: '保存后生成',
    disabled: true
  },
  {
    label: '药源库',
    prop: 'pharmacy',
    type: 'select',
    placeholder: '入库药源库',
    options: drugSourceDatabase
  },
  {
    label: '目的药房',
    prop: 'targetPharmacy',
    type: 'select',
    placeholder: '调入药房',
    options: pharmacyOptions
  },
  {
    label: '制单时间',
    prop: 'createTime',
    type: 'datetime',
    placeholder: '选择日期时间'
  },
  {
    label: '项目类型',
    prop: 'itemType',
    type: 'select',
    placeholder: '请选择',
    options: [
      { label: '药品', value: '药品' },
      { label: '耗材', value: '耗材' }
    ]
  },
  {
    label: '关联请领',
    prop: 'linkedRequisition',
    type: 'input',
    placeholder: '可选，已同意的请领单号'
  }
])

// ==================== Main 区表单配置（从 mock 加载） ====================

const formColumns = ref<any[]>([])
const rowDefaults = ref<Record<string, any>>({})
const selectTableColumns = ref<any[]>([])

/** SelectTable 空数据 */
async function emptySelectTableLoad() {
  return { rows: [], total: 0 }
}

/** SelectTable 回填业务逻辑（原在 MainPane 中） */
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

/** 数量变更业务逻辑（原在 MainPane 中） */
function onQuantityChange(row: Record<string, any>, value: number) {
  const totalAmount = computeActualAmount(row.unitPrice, value)
  return { ...row, quantity: value, totalAmount }
}

onMounted(async () => {
  try {
    const config = await getFormConfig()
    // 父组件控制列项：过滤掉不需要的列即可
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
