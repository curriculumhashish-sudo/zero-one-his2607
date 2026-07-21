<template>
  <DocumentWorkstationShell
    :has-selection="true"
    v-model:queue-width="queueWidth"
  >
    <!-- 左侧 Queue 区头部 -->
    <template #queue-header>
      <span style="font-weight: 600">单据列表</span>
    </template>

    <!-- 左侧 Queue 区主体 -->
    <template #queue>
      <DocumentQueueList
        :orders="filteredOrders"
        :row-key="'id'"
        v-model:selected-key="selectedKey"
        :fields="queueFields"
        :status-map="statusMap"
        :loading="queueLoading"
        :show-create="true"
        empty-text="暂无单据"
        @select="onOrderSelect"
        @create="onCreate"
        @refresh="onRefresh"
      >
        <template #filters>
          <div style="display: flex; flex-direction: column; gap: 6px">
            <DateRangeField v-model="dateRange" />
            <el-select
              v-model="statusFilter"
              placeholder="状态筛选"
              clearable
              size="small"
              style="width: 100%"
            >
              <el-option label="全部" value="" />
              <el-option label="待审核" value="pending" />
              <el-option label="已审核" value="approved" />
              <el-option label="已驳回" value="rejected" />
            </el-select>
          </div>
        </template>
      </DocumentQueueList>
    </template>

    <!-- 右侧 Main 区 -->
    <template #main>
      <DocumentHeadPanel
        title="采购入库单"
        v-model:status="docStatus"
        v-model:keyword="searchKeyword"
        v-model:dropdown-value="searchDropdownValue"
        :status-map="docStatusMap"
        :dropdown-options="searchDropdownOptions"
        :search-field-count="2"
        :show-search="false"
        :show-reset="false"
        @action="onDocAction"
        @search="onSearch"
        @reset="onSearchReset"
      >
        <!-- 查询区额外字段 -->
        <template #searchFields>
          <div class="query-shell__field">
            <label class="query-shell__label">药品名称</label>
            <el-input v-model="drugName" placeholder="请输入药品名称" clearable size="default" />
          </div>
          <div class="query-shell__field">
            <label class="query-shell__label">批号</label>
            <el-input v-model="batchNo" placeholder="请输入批号" clearable size="default" />
          </div>
        </template>

        <!-- Body：可编辑明细表格 -->
        <template #body>
          <DocumentDetailEditableTable
            v-model="detailRows"
            :columns="detailColumns"
            :rules="detailRules"
            :show-add="true"
            :show-delete="true"
            :max-rows="20"
            @row-add="onRowAdd"
            @row-delete="onRowDelete"
            @cell-change="onCellChange"
          >
            <template #actions="{ row }">
              <el-button link type="danger" size="small" @click="onDeleteRow(row)">
                删除
              </el-button>
            </template>
          </DocumentDetailEditableTable>
        </template>
      </DocumentHeadPanel>
    </template>
  </DocumentWorkstationShell>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import DocumentWorkstationShell from '@/components/warehouse/DocumentWorkstationShell.vue'
import DocumentQueueList from '@/components/warehouse/DocumentQueueList.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import DocumentHeadPanel from '@/components/warehouse/DocumentHeadPanel.vue'
import DocumentDetailEditableTable from '@/components/warehouse/DocumentDetailEditableTable.vue'
import type { StatusDisplayOption } from '@/components/common/type'

// ==================== 状态定义 ====================

const queueWidth = ref(320)
const selectedKey = ref<string | number | null>(null)
const queueLoading = ref(false)
const statusFilter = ref('')
const dateRange = ref<[string, string] | null>(null)

// ==================== 左侧队列数据 ====================

interface MockOrder {
  id: number
  docNo: string
  date: string
  handler: string
  counterparty: string
  status: string
}

const mockOrders = ref<MockOrder[]>([
  { id: 1, docNo: 'CG-2026-001', date: '2026-07-20', handler: '张三', counterparty: '北京医药有限公司', status: 'pending' },
  { id: 2, docNo: 'XS-2026-002', date: '2026-07-19', handler: '李四', counterparty: '上海生物科技公司', status: 'approved' },
])

const queueFields = [
  { key: 'docNo', label: '单据号' },
  { key: 'date', label: '日期' },
  { key: 'handler', label: '经办人' },
  { key: 'counterparty', label: '往来单位' },
]

const statusMap: StatusDisplayOption[] = [
  { value: 'pending', label: '待审核', type: 'warning' },
  { value: 'approved', label: '已审核', type: 'success' },
  { value: 'rejected', label: '已驳回', type: 'danger' },
]

const filteredOrders = computed(() => {
  let list = mockOrders.value

  // 状态过滤
  if (statusFilter.value) {
    list = list.filter((o) => o.status === statusFilter.value)
  }

  // 日期范围过滤
  if (dateRange.value) {
    const [start, end] = dateRange.value
    list = list.filter((o) => o.date >= start && o.date <= end)
  }

  return list
})

// ==================== 右侧单据详情 ====================

const docStatus = ref('draft')

const docStatusMap: StatusDisplayOption[] = [
  { value: 'draft', label: '草稿', type: 'info' },
  { value: 'pending', label: '待审核', type: 'warning' },
  { value: 'approved', label: '已审核', type: 'success' },
  { value: 'rejected', label: '已驳回', type: 'danger' },
]

const searchKeyword = ref('')
const searchDropdownValue = ref<string | number | null>(null)
const searchDropdownOptions = [
  { label: '全部', value: '' },
  { label: '北京医药', value: 'bj' },
  { label: '上海生物', value: 'sh' },
]
const drugName = ref('')
const batchNo = ref('')

// ==================== 明细表格 ====================

const detailRows = ref([
  { id: 1, drugName: '阿莫西林胶囊', spec: '0.25g×24粒', batchNo: '20260701', quantity: 100, unit: '盒', price: 12.50, amount: 1250.00 },
  { id: 2, drugName: '头孢克洛缓释片', spec: '0.375g×6片', batchNo: '20260702', quantity: 200, unit: '盒', price: 28.00, amount: 5600.00 },
  { id: 3, drugName: '布洛芬缓释胶囊', spec: '0.3g×20粒', batchNo: '20260703', quantity: 150, unit: '盒', price: 15.80, amount: 2370.00 },
])

const detailColumns = [
  { label: '药品名称', prop: 'drugName', width: 160, editable: true },
  { label: '规格', prop: 'spec', width: 120, editable: true },
  { label: '批号', prop: 'batchNo', width: 120, editable: true },
  { label: '数量', prop: 'quantity', width: 100, editable: true, editorType: 'number' as const, isQty: true },
  { label: '单位', prop: 'unit', width: 80 },
  { label: '单价', prop: 'price', width: 100, editable: true, editorType: 'number' as const, isAmount: true },
  { label: '金额', prop: 'amount', width: 120, isAmount: true },
]

const detailRules = [
  { prop: 'drugName', required: true },
  { prop: 'quantity', required: true, min: 1 },
  { prop: 'price', required: true, min: 0 },
]

// ==================== 事件处理 ====================

function onOrderSelect(order: MockOrder) {
  console.log('选中单据:', order)
}

function onCreate() {
  console.log('新增单据')
}

function onRefresh() {
  queueLoading.value = true
  setTimeout(() => { queueLoading.value = false }, 800)
}

function onDocAction(action: string) {
  console.log('单据操作:', action)
}

function onSearch() {
  console.log('查询:', { keyword: searchKeyword.value, dropdownValue: searchDropdownValue.value, drugName: drugName.value, batchNo: batchNo.value })
}

function onSearchReset() {
  searchKeyword.value = ''
  searchDropdownValue.value = null
  drugName.value = ''
  batchNo.value = ''
}

function onRowAdd(row: any) {
  console.log('添加行:', row)
}

function onDeleteRow(row: any) {
  detailRows.value = detailRows.value.filter((r) => r.id !== row.id)
}

function onRowDelete(rows: any[]) {
  const ids = new Set(rows.map((r: any) => r.id))
  detailRows.value = detailRows.value.filter((r) => r.id == null || !ids.has(r.id))
}

function onCellChange(payload: { rowKey: string | number; prop: string; value: any; row: any }) {
  // 金额 = 数量 × 单价
  if (payload.prop === 'quantity' || payload.prop === 'price') {
    const row = payload.row
    row.amount = (row.quantity || 0) * (row.price || 0)
  }
}
</script>
