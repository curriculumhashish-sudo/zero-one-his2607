<template>
  <div class="main-pane">
    <DocumentHeadPanel
      :title="title"
      :status="order ? order.status : 0"
      :status-map="statusMap"
      :loading="loading"
    >
      <template #body>
        <!-- 检索字段 -->
        <div v-if="extraSearchFields.length" class="main-pane__search">
          <div v-for="field in extraSearchFields" :key="field.prop" class="main-pane__search-field">
            <label class="main-pane__search-label">{{ field.label }}</label>
            <el-input
              v-if="field.type === 'input'"
              v-model="searchValues[field.prop]"
              :placeholder="field.placeholder"
              :disabled="field.disabled"
              size="default"
            />
            <el-select
              v-else
              v-model="searchValues[field.prop]"
              :placeholder="field.placeholder"
              :disabled="field.disabled"
              clearable
              size="default"
            >
              <el-option
                v-for="opt in field.options"
                :key="opt.value"
                :label="opt.label"
                :value="opt.value"
              />
            </el-select>
          </div>
        </div>

        <!-- 明细表单 -->
        <DocumentDetailEditableTable
          v-model="details"
          :columns="columns"
          :loading="loading"
          row-key="id"
          show-add
          show-delete
          @row-add="handleRowAdd"
        >
          <!-- 项目名称：编辑icon + 点击输入框弹出 SelectTable -->
          <template #cell-itemName="{ row, rowKey }">
            <div class="main-pane__cell-edit" @click="openSelectTable(rowKey)">
              <el-icon><Edit /></el-icon>
              <el-input
                :model-value="row.itemName"
                placeholder="请输入关键词检索药品"
                size="default"
                readonly
              />
            </div>
          </template>

          <!-- 实盘数量：number输入框 + 单位下拉 -->
          <template #cell-actualQuantity="{ row }">
            <div class="main-pane__cell-edit">
              <el-icon><Edit /></el-icon>
              <el-input-number
                :model-value="row.actualQuantity"
                :min="0"
                size="default"
                controls-position="right"
                style="width: 80px"
                @change="(v: number) => handleActualQuantityChange(row, v)"
              />
              <el-select
                :model-value="row.actualUnit"
                size="default"
                style="width: 70px"
                @change="(v: string) => (row.actualUnit = v)"
              >
                <el-option
                  v-for="u in unitOptions"
                  :key="u.value"
                  :label="u.label"
                  :value="u.value"
                />
              </el-select>
            </div>
          </template>

          <!-- 盈亏数量：计算展示 -->
          <template #cell-profitLoss="{ row }">
            <span>{{ row.profitLoss }}</span>
          </template>

          <!-- 单价：格式化展示 -->
          <template #cell-unitPrice="{ row }">
            <span>{{ row.unitPrice != null ? row.unitPrice.toFixed(2) + '元' : '' }}</span>
          </template>

          <!-- 实盘金额：计算展示 -->
          <template #cell-actualAmount="{ row }">
            <span>{{ row.actualAmount != null ? row.actualAmount.toFixed(2) : '' }}</span>
          </template>

          <!-- 盈亏金额：格式化展示 -->
          <template #cell-profitLossAmount="{ row }">
            <span>{{
              row.profitLossAmount != null ? row.profitLossAmount.toFixed(2) + '元' : ''
            }}</span>
          </template>

          <!-- 备注：输入框 -->
          <template #cell-remark="{ row }">
            <el-input
              :model-value="row.remark"
              size="default"
              @update:model-value="(v: string) => (row.remark = v)"
            />
          </template>

          <!-- 操作：删除链接 -->
          <template #actions="{ row, index }">
            <el-link type="danger" :underline="false" @click="deleteRow(index)">删除</el-link>
          </template>
        </DocumentDetailEditableTable>
      </template>

      <!-- Footer：固定在底部，不随表单滚动 -->
      <template #extra>
        <div class="main-pane__footer">
          <el-button type="primary" size="default" @click="emit('action', 'save')"
            >保存单据</el-button
          >
          <el-button type="success" size="default" @click="emit('action', 'saveAndSubmit')"
            >保存并提交审批</el-button
          >
          <el-button type="danger" size="default" @click="emit('action', 'delete')"
            >删除单据</el-button
          >
          <span class="main-pane__divider" />
          <el-button type="warning" size="default" @click="emit('action', 'submit')"
            >提交审批</el-button
          >
          <el-button size="default" @click="emit('action', 'withdraw')">撤回</el-button>
          <span class="main-pane__divider" />
          <el-button type="success" size="default" @click="emit('action', 'approve')"
            >审批</el-button
          >
          <el-button type="danger" size="default" @click="emit('action', 'reject')">驳回</el-button>
        </div>
      </template>
    </DocumentHeadPanel>

    <!-- SelectTable 药品选择弹窗 -->
    <SelectTable
      v-model:visible="selectTableVisible"
      title="选择药品"
      row-key="itemCode"
      :columns="selectTableColumns"
      :load-page="loadSupplierOptions"
      @confirm="handleSupplierSelect"
    />
  </div>
</template>

<script setup lang="ts">
import { ref, reactive, watch, onMounted } from 'vue'
import { Edit } from '@element-plus/icons-vue'
import DocumentHeadPanel from '@/components/warehouse/DocumentHeadPanel.vue'
import DocumentDetailEditableTable from '@/components/warehouse/DocumentDetailEditableTable.vue'
import SelectTable from '@/components/common/SelectTable.vue'
import type { SelectTableColumn } from '@/components/common/select-type'
import { loadSupplierOptions, getPurchaseInboundDetail } from '@/apis/warehouse/purchase-inbound'
import type { PurchaseInboundOrder, SupplierItem } from '@/apis/warehouse/purchase-inbound/type'
import type { InventoryLineItem } from '../type'
import { computeBatchNo, computeActualAmount, computeProfitLossAmount } from '../utils'

// ==================== 类型 ====================

export interface ExtraSearchField {
  label: string
  prop: string
  type: 'input' | 'select'
  placeholder?: string
  options?: { label: string; value: string }[]
  defaultValue?: string
  disabled?: boolean
}

// ==================== Props ====================

const props = withDefaults(
  defineProps<{
    orderId?: string
    title?: string
    extraSearchFields?: ExtraSearchField[]
  }>(),
  {
    orderId: '',
    title: '右侧标题',
    extraSearchFields: () => []
  }
)

// ==================== Emits ====================

const emit = defineEmits<{
  action: [action: string]
}>()

// ==================== 常量 ====================

const statusMap = [
  { label: '编辑中', value: 0 },
  { label: '入库中', value: 1 },
  { label: '已完成', value: 2 },
  { label: '已作废', value: 3 }
]

const unitOptions = [
  { label: '支', value: '支' },
  { label: '瓶', value: '瓶' },
  { label: '盒', value: '盒' },
  { label: '袋', value: '袋' }
]

const columns = [
  { prop: 'itemName', label: '项目名称', editable: false, width: 200 },
  { prop: 'specification', label: '规格', editable: false, width: 120 },
  { prop: 'manufacturer', label: '厂家', editable: false, minWidth: 160 },
  { prop: 'batchNo', label: '批号', editable: false, width: 120 },
  {
    prop: 'bookQuantity',
    label: '账面数量',
    editable: true,
    editorType: 'number' as const,
    width: 100
  },
  { prop: 'actualQuantity', label: '实盘数量', editable: false, width: 180 },
  { prop: 'profitLoss', label: '盈亏数量', editable: false, width: 90 },
  { prop: 'unitPrice', label: '单价', editable: false, width: 100 },
  { prop: 'actualAmount', label: '实盘金额', editable: false, width: 100 },
  { prop: 'profitLossAmount', label: '盈亏金额', editable: false, width: 100 },
  { prop: 'remark', label: '备注', editable: false, width: 140 }
]

const selectTableColumns: SelectTableColumn<SupplierItem>[] = [
  { prop: 'itemCode', label: '项目编码', width: 100 },
  { prop: 'itemName', label: '项目名称', minWidth: 140 },
  { prop: 'manufacturer', label: '厂家/产地', minWidth: 160 },
  { prop: 'specification', label: '规格', width: 120 },
  { prop: 'stockQuantity', label: '库存数量', width: 90 },
  { prop: 'packageUnit', label: '包装单位', width: 90 }
]

// ==================== 检索状态 ====================

const searchValues = reactive<Record<string, string>>({})

function buildSearchValues() {
  props.extraSearchFields.forEach((f) => {
    if (!(f.prop in searchValues)) {
      searchValues[f.prop] = f.defaultValue ?? ''
    }
  })
}

// ==================== 明细状态 ====================

const order = ref<PurchaseInboundOrder | null>(null)
const details = ref<InventoryLineItem[]>([])
const loading = ref(false)

/** SelectTable 弹窗状态 */
const selectTableVisible = ref(false)
const selectTargetRowId = ref<string>('')

// ==================== SelectTable ====================

function openSelectTable(rowKey: string) {
  selectTargetRowId.value = String(rowKey)
  selectTableVisible.value = true
}

function handleSupplierSelect(_value: unknown, rows: SupplierItem[]) {
  if (!rows.length) return
  const item = rows[0]
  const idx = details.value.findIndex((r) => r.id === selectTargetRowId.value)
  if (idx === -1) return

  const row = details.value[idx]
  row.itemName = item.itemName
  row.specification = item.specification
  row.manufacturer = item.manufacturer
  row.batchNo = computeBatchNo(item.itemCode)
  row.bookQuantity = item.stockQuantity
  row.actualQuantity = item.stockQuantity
  row.actualUnit = item.packageUnit
  row.profitLoss = 0
  row.unitPrice = item.purchasePrice
  row.actualAmount = computeActualAmount(item.purchasePrice, item.stockQuantity)
  row.profitLossAmount = 0
  row.remark = ''

  selectTargetRowId.value = ''
}

// ==================== 实盘数量联动 ====================

function handleActualQuantityChange(row: InventoryLineItem, value: number) {
  row.actualQuantity = value
  row.profitLoss = value - row.bookQuantity
  row.actualAmount = computeActualAmount(row.unitPrice, value)
  row.profitLossAmount = computeProfitLossAmount(row.actualAmount, row.unitPrice, row.bookQuantity)
}

// ==================== 方法 ====================

async function loadDetail() {
  if (!props.orderId) {
    order.value = null
    details.value = []
    return
  }
  loading.value = true
  try {
    const data = await getPurchaseInboundDetail(props.orderId)
    order.value = data.order
    details.value = []
  } catch {
    order.value = null
    details.value = []
  } finally {
    loading.value = false
  }
}

function handleRowAdd(row: InventoryLineItem) {
  row.itemName = ''
  row.specification = ''
  row.manufacturer = ''
  row.batchNo = ''
  row.bookQuantity = 0
  row.actualQuantity = 0
  row.actualUnit = '单位'
  row.profitLoss = 0
  row.unitPrice = 0
  row.actualAmount = 0
  row.profitLossAmount = 0
  row.remark = '无'
}

function deleteRow(index: number) {
  details.value.splice(index, 1)
}

// ==================== 监听 ====================

watch(() => props.orderId, loadDetail)
watch(() => props.extraSearchFields, buildSearchValues, { immediate: true })

onMounted(() => {
  if (props.orderId) loadDetail()
})
</script>

<style scoped>
/* 主容器：让 DocumentHeadPanel 的 body 区可滚动，footer 固定 */
.main-pane {
  height: 100%;
  display: flex;
  flex-direction: column;
  min-height: 0;
}

.main-pane :deep(.doc-head-panel) {
  height: 100%;
  display: flex;
  flex-direction: column;
}

.main-pane :deep(.doc-head-panel__body) {
  flex: 1;
  min-height: 0;
  overflow: hidden auto;
}

/* 检索字段行 */
.main-pane__search {
  display: flex;
  align-items: center;
  gap: 16px;
  padding-bottom: 10px;
  margin-bottom: 10px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  flex-wrap: wrap;
}

.main-pane__search-field {
  display: flex;
  align-items: center;
  gap: 8px;
  min-width: 0;
}

.main-pane__search-label {
  flex-shrink: 0;
  font-size: 13px;
  font-weight: 500;
  color: var(--el-text-color-regular);
  white-space: nowrap;
}

.main-pane__search-field .el-input,
.main-pane__search-field .el-select {
  width: 160px;
}

/* 带编辑icon的单元格 */
.main-pane__cell-edit {
  display: flex;
  align-items: center;
  gap: 4px;
}

.main-pane__cell-edit .el-icon {
  flex-shrink: 0;
  font-size: 14px;
  color: var(--el-color-primary);
  cursor: pointer;
}

/* Footer */
.main-pane__footer {
  display: flex;
  justify-content: flex-end;
  align-items: center;
  gap: 15px;
}

.main-pane__divider {
  display: inline-block;
  width: 1px;
  height: 20px;
  background: var(--el-border-color-light);
}
</style>
