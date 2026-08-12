<template>
  <DocumentWorkstationShell :has-selection="true">
    <template #queue-header>
      <span class="queue-header-title">历史出库单</span>
    </template>

    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="请选择药源库"
        :dropdown1-options="queueDropdown1Options"
        dropdown2-placeholder="请选择目的药房"
        :dropdown2-options="queueDropdown2Options"
        :show-dropdown2="true"
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
        title="出库单信息"
        :extra-search-fields="mainSearchFields"
        :form-columns="formColumns"
        :row-defaults="rowDefaults"
        :select-table-columns="selectTableColumns"
        :select-table-load-page="handleSelectTableLoad"
        :on-supplier-fill="onSupplierFill"
        :on-quantity-change="onQuantityChange"
        :show-requisition-button="true"
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
import { ElMessage } from 'element-plus'
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
  addStockOut,
  createPurchaseInbound,
  updatePurchaseInbound,
  removePurchaseInbound,
  submitPurchaseInbound,
  withdrawPurchaseInbound,
  approvePurchaseInbound,
  rejectPurchaseInbound
} from '@/apis/inventory/warehouse/stock-out'
import type { StockOutQuery } from '@/apis/inventory/warehouse/stock-out/type'
import { FORM_COLUMNS, ROW_DEFAULTS, SELECT_TABLE_COLUMNS } from './type'
import { computeBatchNo, computeActualAmount } from '@/views/inventory/common/utils'

// ==================== 选项 ====================

const drugSourceDatabase = [
  { label: '耗材库', value: '322954857622802432' },
  { label: '西药库', value: '322954885380706304' },
  { label: '中药库', value: '322954919841107968' }
]
const pharmacyOptions = [
  { label: '门诊西药房', value: '322954949549363200' },
  { label: '门诊中药房', value: '322954984441778176' },
  { label: '门诊耗材房', value: '322955075642724352' },
  { label: '住院西药房', value: '322955009603407872' },
  { label: '住院中药房', value: '322955039064199168' },
  { label: '住院耗材房', value: '322955098803671040' }
]

const queueDropdown1Options = drugSourceDatabase
const queueDropdown2Options = pharmacyOptions

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
  getSearchTargetPharmacy: () =>
    (mainPaneRef.value?.getSearchValue('targetPharmacy') as string) ?? '',
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

/** 缓存最近一次搜索参数，供 handleQueueAdd 读取 dropdown 值 */
const lastSearchParams = ref<QueueSearchParams>({
  checkNo: '',
  pageSize: 10,
  checkType: '',
  startDate: '',
  endDate: '',
  pageIndex: 1
})

// ==================== Queue 事件处理 ====================

async function handleQueueSearch(params: QueueSearchParams) {
  lastSearchParams.value = params
  loading.value = true
  try {
    const query: StockOutQuery = {
      busNokey: params.checkNo,
      startDate: params.startDate,
      endDate: params.endDate,
      drugDatabase: params.checkType,
      pharmacyWarehouse: params.purposeLocationId,
      pageIndex: params.pageIndex,
      pageSize: params.pageSize
    }
    const result = await pagePurchaseInboundOrders(query)
    orders.value = result.rows as unknown as Record<string, unknown>[]
  } catch {
  } finally {
    loading.value = false
  }
}

function handleQueueSelect(order: Record<string, unknown>) {
  selectedOrder.value = order
  mainPaneRef.value?.setSearchValue('pharmacy', order.sourceLocationId_dictText ?? '')
  mainPaneRef.value?.setSearchValue('targetPharmacy', order.purposeLocationId_dictText ?? '')
}

async function handleQueueAdd() {
  const pharmacy = lastSearchParams.value.checkType
  const targetPharmacy = lastSearchParams.value.purposeLocationId
  if (!pharmacy || !targetPharmacy) {
    ElMessage.warning('请先在左侧检索区选择药源库和目的药房')
    return
  }
  try {
    const result = await addStockOut({
      sourceLocationId: pharmacy,
      purposeLocationId: targetPharmacy
    })
    orders.value.unshift(result.order as unknown as Record<string, unknown>)
    selectedOrder.value = result.order as unknown as Record<string, unknown>
    // 将后端返回的药源库/目的药房名称回填到右侧搜索区，确保保存时 sourceLocationId 等字段有值
    mainPaneRef.value?.setSearchValue('pharmacy', result.order.sourceLocationId_dictText ?? '')
    mainPaneRef.value?.setSearchValue(
      'targetPharmacy',
      result.order.purposeLocationId_dictText ?? ''
    )
    ElMessage.success('新增出库单成功')
  } catch {
    ElMessage.error('新增出库单失败')
  }
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
  { label: '制单时间', prop: 'createTime', type: 'datetime', placeholder: '选择日期时间' },
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

// ==================== 表单配置 ====================

const formColumns = ref(FORM_COLUMNS)
const rowDefaults = ref(ROW_DEFAULTS)
const selectTableColumns = ref(SELECT_TABLE_COLUMNS)

/** SelectTable：仅药源库=西药库 且 目的药房=门诊西药房 时返回数据 */
function handleSelectTableLoad(query: { keyword: string; pageIndex: number; pageSize: number }) {
  // const pharmacy = mainPaneRef.value?.getSearchValue('pharmacy') as string | undefined
  // const target = mainPaneRef.value?.getSearchValue('targetPharmacy') as string | undefined
  // if (pharmacy !== '西药库' || target !== '门诊西药房')
  //   return Promise.resolve({ rows: [], total: 0 })
  return loadSupplierOptions(query)
}

function onSupplierFill(row: Record<string, any>, item: Record<string, any>) {
  const actualAmount = computeActualAmount(item.purchasePrice, item.stockQuantity)
  return {
    ...row,
    inventoryItemId: item.inventoryItemId,
    itemName: item.itemName,
    specification: item.specification,
    manufacturer: item.manufacturer,
    actualQuantity: item.stockQuantity,
    actualUnit: item.packageUnit,
    unitCode: (item.unitCode as string) || (item.packageUnit as string) || '',
    unitPrice: item.purchasePrice,
    actualAmount,
    batchNo: computeBatchNo(item.itemCode),
    remark: ''
  }
}

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
