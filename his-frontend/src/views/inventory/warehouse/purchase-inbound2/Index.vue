<template>
  <DocumentWorkstationShell :has-selection="true">
    <template #queue-header>
      <span class="queue-header-title">历史进货单</span>
    </template>

    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="请选择库房"
        :dropdown1-options="queueDropdown1Options"
        dropdown2-placeholder="请选择目的药房"
        :dropdown2-options="queueDropdown2Options"
        :show-dropdown2="false"
        add-button-text="新增单据"
        :add-disabled="addDisabled"
        :orders="orders"
        :loading="loading"
        :selected-order="selectedOrder"
        :show-supplier="true"
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
        title="进货单信息"
        :extra-search-fields="mainSearchFields"
        :form-columns="formColumns"
        :row-defaults="rowDefaults"
        :select-table-columns="selectTableColumns"
        :select-table-load-page="loadSupplierOptions"
        :on-supplier-fill="onSupplierFill"
        :on-quantity-change="onQuantityChange"
        :load-detail-fn="getPurchaseInboundDetail"
        :show-requisition-button="true"
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
} from '@/apis/inventory/warehouse/purchase-inbound2'
import { FORM_COLUMNS, ROW_DEFAULTS, SELECT_TABLE_COLUMNS } from './type'
import { computeActualAmount } from '@/views/inventory/common/utils'

// ==================== 选项 ====================

const drugSourceDatabase = [
  { label: '耗材库', value: '耗材库' },
  { label: '西药库', value: '西药库' },
  { label: '中药库', value: '中药库' }
]
const supplierOptions = [
  { label: '华南中药饮片供应中心', value: '华南中药饮片供应中心' },
  { label: '华中药品配送有限公司', value: '华中药品配送有限公司' },
  { label: '东部医疗物流供应链有限公司', value: '东部医疗物流供应链有限公司' },
  { label: '北方疫苗冷链供应有限公司', value: '北方疫苗冷链供应有限公司' },
  { label: '中原中药材供应有限公司', value: '中原中药材供应有限公司' }
]

const queueDropdown1Options = drugSourceDatabase
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
  getSearchPharmacy: () => (mainPaneRef.value?.getSearchValue('warehouse') as string) ?? '',
  getSearchTargetPharmacy: () => '',
  getSearchSupplier: () => (mainPaneRef.value?.getSearchValue('supplier') as string) ?? '',
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
/** 新增空单据自增计数器 */
let idCounter = 0

// ==================== Queue 事件处理 ====================

async function handleQueueSearch(params: QueueSearchParams) {
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
  // sync search values（MainPane loadDetail 不覆盖已存在的 key，这里手动设）
  mainPaneRef.value?.setSearchValue('supplier', order.supplierId_dictText ?? '')
  mainPaneRef.value?.setSearchValue('warehouse', order.sourceLocationId_dictText ?? '')
}

function handleQueueAdd() {
  idCounter++
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
    supplierId_dictText: (mainPaneRef.value?.getSearchValue('supplier') as string) ?? '',
    sourceLocationId_dictText: (mainPaneRef.value?.getSearchValue('warehouse') as string) ?? '',
    totalAmount: 0,
    _draft: true,
    _clientDraftId: `pi-new-${idCounter}`
  }
  orders.value.unshift(newOrder)
  selectedOrder.value = newOrder
  mainPaneRef.value?.resetSearchValues()
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

const now = new Date()
const pad = (n: number) => String(n).padStart(2, '0')
const defaultCreateTime = `${now.getFullYear()}-${pad(now.getMonth() + 1)}-${pad(now.getDate())} ${pad(now.getHours())}:${pad(now.getMinutes())}`

const mainSearchFields = reactive<ExtraSearchField[]>([
  { label: '单据号', prop: 'docNo', type: 'input', placeholder: '未选择', disabled: true },
  {
    label: '供应商',
    prop: 'supplier',
    type: 'select',
    placeholder: '请选择',
    options: supplierOptions
  },
  {
    label: '制单时间',
    prop: 'createTime',
    type: 'datetime',
    placeholder: '选择日期时间',
    defaultValue: defaultCreateTime
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

// ==================== 表单配置 ====================

const formColumns = ref(FORM_COLUMNS)
const rowDefaults = ref(ROW_DEFAULTS)
const selectTableColumns = ref(SELECT_TABLE_COLUMNS)

/** SelectTable 回填业务逻辑 */
function onSupplierFill(row: Record<string, any>, item: Record<string, any>) {
  const actualAmount = computeActualAmount(item.purchasePrice, item.stockQuantity)
  return {
    ...row,
    itemCode: item.itemCode,
    itemName: item.itemName,
    specification: item.specification,
    manufacturer: item.manufacturer,
    actualQuantity: item.stockQuantity,
    actualUnit: item.packageUnit,
    unitCode: (item.unitCode as string) ?? '',
    unitPrice: item.purchasePrice,
    actualAmount,
    batchNo: '',
    traceNo: '',
    remark: ''
  }
}

/** 实盘数量变更时重算盈亏和金额 */
function onQuantityChange(row: Record<string, any>, value: number) {
  const actualAmount = computeActualAmount(row.unitPrice, value)
  return { ...row, actualQuantity: value, actualAmount }
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
