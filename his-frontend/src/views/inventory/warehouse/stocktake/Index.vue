<template>
  <DocumentWorkstationShell :has-selection="true">
    <template #queue-header>
      <span class="queue-header-title">历史盘点单</span>
    </template>

    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="盘点药库"
        :dropdown1-options="queueDropdown1Options"
        dropdown2-placeholder="请选择目的药房"
        :dropdown2-options="queueDropdown2Options"
        :show-dropdown2="false"
        add-button-text="新增单据"
        :add-disabled="addDisabled"
        :orders="orders"
        :loading="loading"
        :selected-order="selectedOrder"
        @select="handleQueueSelect"
        @add="handleQueueAdd"
        @search="handleQueueSearch"
        @reset="handleQueueReset"
      />
    </template>

    <template #main>
      <PurchaseInboundMainPane
        ref="mainPaneRef"
        :order-id="selectedOrder?.id as string"
        title="药库盘点单"
        :extra-search-fields="mainSearchFields"
        :form-columns="formColumns"
        :row-defaults="rowDefaults"
        :select-table-columns="selectTableColumns"
        :select-table-load-page="handleSelectTableLoad"
        :on-supplier-fill="onSupplierFill"
        :on-quantity-change="onQuantityChange"
        :load-detail-fn="getPurchaseInboundDetail"
        @action="handleAction"
      />
    </template>
  </DocumentWorkstationShell>

  <el-dialog v-model="rejectVisible" title="驳回原因" width="400px">
    <el-input v-model="rejectReason" type="textarea" :rows="3" placeholder="请输入驳回原因" />
    <template #footer>
      <el-button @click="cancelReject()">取消</el-button>
      <el-button type="primary" @click="confirmReject()">确定</el-button>
    </template>
  </el-dialog>
</template>

<script setup lang="ts">
import { ref, reactive, computed, onMounted } from 'vue'
import DocumentWorkstationShell from '@/components/warehouse/DocumentWorkstationShell.vue'
import PurchaseInboundQueuePane from '@/views/inventory/components/PurchaseInboundQueuePane.vue'
import type { QueueSearchParams } from '@/views/inventory/components/PurchaseInboundQueuePane.vue'
import PurchaseInboundMainPane from '@/views/inventory/components/PurchaseInboundMainPane.vue'
import type { ExtraSearchField } from '@/views/inventory/components/PurchaseInboundMainPane.vue'
import { useApprovalFlow } from '@/views/inventory/common/useApprovalFlow'
import {
  loadSupplierOptions,
  pagePurchaseInboundOrders,
  getPurchaseInboundDetail,
  createPurchaseInbound,
  updatePurchaseInbound,
  removePurchaseInbound,
  submitPurchaseInbound,
  withdrawPurchaseInbound,
  approvePurchaseInbound,
  rejectPurchaseInbound
} from '@/apis/inventory/warehouse/stocktake'
import { FORM_COLUMNS, ROW_DEFAULTS, SELECT_TABLE_COLUMNS } from './type'
import {
  computeBatchNo,
  computeActualAmount,
  computeProfitLoss,
  computeProfitLossAmount
} from '@/views/inventory/common/utils'

// ==================== 选项 ====================

const warehouseOptions = [
  { label: '耗材库', value: '322954857622802432' },
  { label: '西药库', value: '322954885380706304' },
  { label: '中药库', value: '中药库' }
]

const queueDropdown1Options = warehouseOptions
const queueDropdown2Options: { label: string; value: string }[] = []

// ==================== 单据队列状态 ====================

const orders = ref<Record<string, unknown>[]>([])
const loading = ref(false)
const selectedOrder = ref<Record<string, unknown> | null>(null)
const addDisabled = computed(() => (selectedOrder.value?.status as number) === 0)

// ==================== 审批流 ====================

const mainPaneRef = ref<InstanceType<typeof PurchaseInboundMainPane>>()
const approval = useApprovalFlow({
  orders,
  selectedOrder,
  getDetails: () => mainPaneRef.value?.details ?? [],
  refreshMainPane: () => mainPaneRef.value?.loadDetail(),
  getSearchPharmacy: () => (mainPaneRef.value?.getSearchValue('pharmacy') as string) ?? '',
  getSearchTargetPharmacy: () => '',
  getSearchSupplier: () => '',
  refreshQueue: () => {
    if (lastSearchParams.value) handleQueueSearch(lastSearchParams.value)
  },
  apis: {
    createPurchaseInbound,
    updatePurchaseInbound,
    removePurchaseInbound,
    submitPurchaseInbound,
    withdrawPurchaseInbound,
    approvePurchaseInbound,
    rejectPurchaseInbound
  } as any
})
const { handleAction, rejectVisible, rejectReason, confirmReject, cancelReject } = approval
/** 缓存最近一次搜索参数，供操作成功后刷新列表使用 */
const lastSearchParams = ref<QueueSearchParams>({
  checkNo: '',
  pageSize: 10,
  checkType: '',
  startDate: '',
  endDate: '',
  pageIndex: 1
})

let idCounter = 0

/** 缓存左侧队列下拉框选中的药库值（checkType），新增草稿时使用 */
let queuePharmacyValue = ''

// ==================== Queue 事件处理 ====================

async function handleQueueSearch(params: QueueSearchParams) {
  lastSearchParams.value = params
  queuePharmacyValue = params.checkType ?? ''
  loading.value = true
  try {
    const result = await pagePurchaseInboundOrders(params as any)
    orders.value = result.rows as unknown as Record<string, unknown>[]
    orders.value.forEach((o) => {
      o.purposeLocationId_dictText = ''
    })
  } catch {
  } finally {
    loading.value = false
  }
}

function handleQueueSelect(order: Record<string, unknown>) {
  selectedOrder.value = order
  mainPaneRef.value?.setSearchValue(
    'pharmacy',
    (order.sourceLocationId as string) ?? (order.sourceLocationId_dictText as string) ?? ''
  )
}

function handleQueueAdd() {
  idCounter++
  const pharmacyId =
    (queuePharmacyValue || (mainPaneRef.value?.getSearchValue('pharmacy') as string)) ?? ''
  const pharmacyLabel = warehouseOptions.find((o) => o.value === pharmacyId)?.label ?? ''
  const newOrder: Record<string, unknown> = {
    id: `pi-new-${idCounter}`,
    status: 0,
    category: '',
    statusEnum: 0,
    statusEnum_enumText: '编辑中',
    applicantId_dictText: '',
    applyTime: '',
    approverId_dictText: '',
    approvalTime: '',
    supplierId_dictText: '',
    sourceLocationId: pharmacyId,
    sourceLocationId_dictText: pharmacyLabel,
    purposeLocationId: pharmacyId,
    totalAmount: 0,
    _draft: true,
    _clientDraftId: `pi-new-${idCounter}`
  }
  orders.value.unshift(newOrder)
  selectedOrder.value = newOrder
  mainPaneRef.value?.resetSearchValues()
  mainPaneRef.value?.setSearchValue('pharmacy', pharmacyId)
}

function handleQueueReset() {
  selectedOrder.value = null
  handleQueueSearch({
    checkNo: '',
    pageSize: 10,
    checkType: '',
    startDate: '',
    endDate: '',
    pageIndex: 1
  })
}

// ==================== 检索区配置 ====================

const mainSearchFields = reactive<ExtraSearchField[]>([
  { label: '单据号', prop: 'docNo', type: 'input', placeholder: '保存后生成', disabled: true },
  {
    label: '盘点药库',
    prop: 'pharmacy',
    type: 'select',
    placeholder: '请选择',
    options: warehouseOptions
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
  }
])

// ==================== 表单配置 ====================

const formColumns = ref(FORM_COLUMNS)
const rowDefaults = ref(ROW_DEFAULTS)
const selectTableColumns = ref(SELECT_TABLE_COLUMNS)

function handleSelectTableLoad(query: { keyword: string; pageIndex: number; pageSize: number }) {
  // const pharmacy = mainPaneRef.value?.getSearchValue('pharmacy') as string | undefined
  // // 非西药库时暂不加载项目（后续中药库/耗材库接入后可放开此限制）
  // if (pharmacy !== '322954885380706304') return Promise.resolve({ rows: [], total: 0 })
  return loadSupplierOptions(query)
}

/** SelectTable 回填业务逻辑 */
function onSupplierFill(row: Record<string, any>, item: Record<string, any>) {
  const bookQuantity = item.stockQuantity
  const actualQuantity = item.stockQuantity
  const profitLoss = computeProfitLoss(actualQuantity, bookQuantity)
  const actualAmount = computeActualAmount(item.purchasePrice, actualQuantity)
  const profitLossAmount = computeProfitLossAmount(profitLoss, item.purchasePrice)
  return {
    ...row,
    itemName: item.itemName,
    specification: item.specification,
    manufacturer: item.manufacturer,
    batchNo: computeBatchNo(item.itemCode),
    bookQuantity,
    actualQuantity,
    actualUnit: item.packageUnit,
    unitCode: (item.unitCode as string) ?? '',
    profitLoss,
    unitPrice: item.purchasePrice,
    actualAmount,
    profitLossAmount,
    remark: '',
    inventoryItemId: (item.inventoryItemId as string) ?? '',
    traceNo: ''
  }
}

/** 实盘数量变更时重算盈亏和金额 */
function onQuantityChange(row: Record<string, any>, value: number) {
  const profitLoss = computeProfitLoss(value, row.bookQuantity)
  const actualAmount = computeActualAmount(row.unitPrice, value)
  const profitLossAmount = computeProfitLossAmount(profitLoss, row.unitPrice)
  return { ...row, actualQuantity: value, profitLoss, actualAmount, profitLossAmount }
}

onMounted(() => {
  handleQueueSearch({
    checkNo: '',
    pageSize: 10,
    checkType: '',
    startDate: '',
    endDate: '',
    pageIndex: 1
  })
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
