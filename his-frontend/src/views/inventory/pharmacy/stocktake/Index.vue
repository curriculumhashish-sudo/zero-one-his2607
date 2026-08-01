<template>
  <DocumentWorkstationShell :has-selection="true">
    <!-- 左侧标题 -->
    <template #queue-header>
      <span class="queue-header-title">历史盘点单</span>
    </template>

    <!-- 左侧 Queue 区 -->
    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="盘点药房"
        :dropdown1-options="queueDropdown1Options"
        dropdown2-placeholder="请选择目的药房"
        :dropdown2-options="queueDropdown2Options"
        :show-dropdown2="false"
        add-button-text="新增单据"
<<<<<<< HEAD
=======
        :add-disabled="addDisabled"
>>>>>>> remotes/origin/f1-cat
        :orders="orders"
        :loading="loading"
        :selected-order="selectedOrder"
        @select="handleQueueSelect"
        @add="handleQueueAdd"
        @search="handleQueueSearch"
        @reset="handleQueueReset"
      />
    </template>

    <!-- 右侧 Main 区 -->
    <template #main>
      <PurchaseInboundMainPane
<<<<<<< HEAD
=======
        ref="mainPaneRef"
>>>>>>> remotes/origin/f1-cat
        :order-id="selectedOrder?.id"
        title="药房盘点单"
        :extra-search-fields="mainSearchFields"
        :form-columns="formColumns"
        :row-defaults="rowDefaults"
        :select-table-columns="selectTableColumns"
        :select-table-load-page="loadSupplierOptions"
        :on-supplier-fill="onSupplierFill"
<<<<<<< HEAD
      />
    </template>
=======
        @action="handleAction"
      />
    </template>

    <!-- 驳回原因弹窗 -->
    <el-dialog v-model="rejectVisible" title="驳回原因" width="400px">
      <el-input
        v-model="rejectReason"
        type="textarea"
        :rows="3"
        placeholder="请输入驳回原因"
      />
      <template #footer>
        <el-button @click="cancelReject()">取消</el-button>
        <el-button type="primary" @click="confirmReject()">确定</el-button>
      </template>
    </el-dialog>
>>>>>>> remotes/origin/f1-cat
  </DocumentWorkstationShell>
</template>

<script setup lang="ts">
<<<<<<< HEAD
import { ref, reactive, onMounted } from 'vue'
=======
import { ref, reactive, computed, onMounted } from 'vue'
>>>>>>> remotes/origin/f1-cat
import DocumentWorkstationShell from '@/components/warehouse/DocumentWorkstationShell.vue'
import PurchaseInboundQueuePane from '@/views/inventory/components/PurchaseInboundQueuePane.vue'
import type { QueueSearchParams } from '@/views/inventory/components/PurchaseInboundQueuePane.vue'
import PurchaseInboundMainPane from '@/views/inventory/components/PurchaseInboundMainPane.vue'
import type { ExtraSearchField } from '@/views/inventory/components/PurchaseInboundMainPane.vue'
import {
  getFormConfig,
  loadSupplierOptions,
  pagePurchaseInboundOrders
} from '@/apis/inventory/pharmacy/stocktake'
<<<<<<< HEAD
=======
import { useApprovalFlow } from '@/views/inventory/common/useApprovalFlow'
>>>>>>> remotes/origin/f1-cat
import { computeBatchNo, computeActualAmount } from './utils'

// ==================== 药房列表（静态数据） ====================

const pharmacyOptions = [
  { label: '门诊西药房', value: '门诊西药房' },
  { label: '门诊中药房', value: '门诊中药房' },
  { label: '门诊耗材房', value: '门诊耗材房' },
  { label: '住院西药房', value: '住院西药房' },
  { label: '住院中药房', value: '住院中药房' },
  { label: '住院耗材房', value: '住院耗材房' }
]

// ==================== 左侧 Queue 区下拉菜单配置 ====================

const queueDropdown1Options = pharmacyOptions
const queueDropdown2Options = pharmacyOptions

// ==================== 单据队列状态（父组件管理） ====================

const orders = ref<Record<string, unknown>[]>([])
const loading = ref(false)
const selectedOrder = ref<Record<string, unknown> | null>(null)

<<<<<<< HEAD
=======
/** 编辑中(0)时禁用新增按钮 */
const addDisabled = computed(() => (selectedOrder.value?.status as number) === 0)

// ==================== 审批流 ====================

const mainPaneRef = ref<InstanceType<typeof PurchaseInboundMainPane>>()

const approval = useApprovalFlow({
  orders,
  selectedOrder,
  getDetails: () => mainPaneRef.value?.details ?? [],
  refreshMainPane: () => mainPaneRef.value?.loadDetail(),
})
const { handleAction, rejectVisible, rejectReason, confirmReject, cancelReject } = approval

>>>>>>> remotes/origin/f1-cat
/** 新增空单据自增计数器 */
let idCounter = 0

// ==================== Queue 区事件处理 ====================

// 分页查询
async function handleQueueSearch(params: QueueSearchParams) {
  loading.value = true
  try {
    const result = await pagePurchaseInboundOrders({
      checkNo: params.checkNo,
      pageSize: params.pageSize,
      checkType: params.checkType,
      startDate: params.startDate,
      endDate: params.endDate,
      pageIndex: params.pageIndex
    })
    // 双重类型断言：先转为 unknown，再转为目标类型，绕过 TypeScript 的类型检查
    orders.value = result.rows as unknown as Record<string, unknown>[]
  } catch {
    // 加载失败保持现有列表
  } finally {
    loading.value = false
  }
}

function handleQueueSelect(order: Record<string, unknown>) {
  selectedOrder.value = order
}

function handleQueueAdd() {
  idCounter++
  const now = new Date()
  const pad = (n: number) => String(n).padStart(2, '0')
  const applyTime = `${now.getFullYear()}-${pad(now.getMonth() + 1)}-${pad(now.getDate())} ${pad(now.getHours())}:${pad(now.getMinutes())}`
  const newOrder: Record<string, unknown> = {
    id: `pi-new-${idCounter}`,
    status: 0,
    category: '',
    statusEnum: 0,
    statusEnum_enumText: '编辑中',
    applicantId_dictText: '当前用户',
    applyTime,
    approverId_dictText: '',
    approvalTime: '',
    supplierId_dictText: '',
    sourceLocationId_dictText: '',
    purposeLocationId_dictText: '',
    totalAmount: 0,
    _draft: true,
    _clientDraftId: `pi-new-${idCounter}`
  }
  orders.value.unshift(newOrder)
  selectedOrder.value = newOrder
<<<<<<< HEAD
=======
  mainPaneRef.value?.resetSearchValues()
>>>>>>> remotes/origin/f1-cat
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
    label: '盘点药房',
    prop: 'pharmacy',
    type: 'select',
    placeholder: '请选择',
    options: pharmacyOptions
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

// ==================== Main 区表单配置（从 mock 加载） ====================

const formColumns = ref<any[]>([])
const rowDefaults = ref<Record<string, any>>({})
const selectTableColumns = ref<any[]>([])

/** SelectTable 回填业务逻辑 */
function onSupplierFill(row: Record<string, any>, item: Record<string, any>) {
  const actualAmount = computeActualAmount(item.purchasePrice, item.stockQuantity)
  return {
    ...row,
    itemName: item.itemName,
    specification: item.specification,
    manufacturer: item.manufacturer,
    batchNo: computeBatchNo(item.itemCode),
    bookQuantity: item.stockQuantity,
    profitLoss: 0,
    unitPrice: item.purchasePrice,
    actualAmount,
    profitLossAmount: 0,
    remark: ''
  }
}

onMounted(() => {
  // 初始加载单据列表
  handleQueueSearch({
    checkNo: '',
    pageSize: 10,
    checkType: '',
    startDate: '',
    endDate: '',
    pageIndex: 1
  })
  // 加载弹出表单数据
  getFormConfig()
    .then((config) => {
      formColumns.value = config.columns
      rowDefaults.value = config.rowDefaults
      selectTableColumns.value = config.selectTableColumns
    })
    .catch(() => {
      // 加载失败使用默认空值
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
