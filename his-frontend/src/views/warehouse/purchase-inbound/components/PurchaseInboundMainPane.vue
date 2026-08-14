<template>
  <div class="main-pane">
    <DocumentHeadPanel
      :title="title"
      :status="order ? order.status : 0"
      :status-map="statusMap"
      :loading="loading"
    >
      <template #body>
        <!-- ==================== 检索区 ==================== -->
        <div v-if="extraSearchFields.length" class="main-pane__search">
          <template v-for="field in extraSearchFields" :key="field.prop">
            <div v-if="!field.hidden" class="main-pane__search-field">
              <label class="main-pane__search-label">{{ field.label }}</label>
              <!-- 输入框 -->
              <el-input
                v-if="field.type === 'input'"
                v-model="searchValues[field.prop]"
                :placeholder="field.placeholder"
                :disabled="field.disabled || searchDisabled"
                size="default"
              />
              <!-- 下拉菜单 -->
              <el-select
                v-else-if="field.type === 'select'"
                v-model="searchValues[field.prop]"
                :placeholder="field.placeholder"
                :disabled="field.disabled || searchDisabled"
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
              <!-- 日期选择器 -->
              <el-date-picker
                v-else-if="field.type === 'date'"
                v-model="searchValues[field.prop]"
                type="daterange"
                :start-placeholder="field.placeholder"
                :end-placeholder="field.endPlaceholder"
                :disabled="field.disabled || searchDisabled"
                size="default"
                style="width: 100%"
              />
            </div>
          </template>
          <!-- "从请领单选择"按钮 -->
          <div v-if="showRequisitionButton" class="main-pane__search-btns">
            <el-button size="default" @click="emit('requisition')"> 从请领单选择 </el-button>
          </div>
        </div>

        <!-- ==================== 表单区 ==================== -->
        <DocumentDetailEditableTable
          v-model="details"
          :columns="formColumns"
          :loading="loading"
          row-key="id"
          show-add
          show-delete
          @row-add="handleRowAdd"
        >
          <!-- 项目名称： 点击输入框弹出 SelectTable -->
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
                @change="(v: number | undefined) => handleActualQuantityChange(row, v ?? 0)"
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

      <!-- ==================== 底部按钮 ==================== -->
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

    <!-- 表单项选择弹窗 -->
    <SelectTable
      v-model:visible="selectTableVisible"
      title="选择项目"
      row-key="itemCode"
      :columns="selectTableColumns as any"
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

import { loadSupplierOptions, getPurchaseInboundDetail } from '@/apis/warehouse/purchase-inbound'
import type { PurchaseInboundOrder, SupplierItem } from '@/apis/warehouse/purchase-inbound/type'
import type { InventoryLineItem } from '../type'
import { computeBatchNo, computeActualAmount, computeProfitLossAmount } from '../utils'

// ==================== 类型 ====================

export interface ExtraSearchField {
  label: string
  prop: string
  type: 'input' | 'select' | 'date'
  placeholder?: string
  /** date 类型时的结束占位 */
  endPlaceholder?: string
  options?: { label: string; value: string }[]
  defaultValue?: string
  disabled?: boolean
  /** 是否隐藏该字段 */
  hidden?: boolean
}

// ==================== Props ====================

const props = withDefaults(
  defineProps<{
    orderId?: string
    title?: string
    extraSearchFields?: ExtraSearchField[]
    /** 检索区全局禁用 */
    searchDisabled?: boolean
    /** 是否显示"从请领单选择"按钮（默认隐藏） */
    showRequisitionButton?: boolean
    /** 表单列定义 */
    formColumns?: {
      prop: string
      label: string
      width?: number | string
      minWidth?: number | string
      fixed?: 'left' | 'right'
      editable?: boolean
      editorType?: 'input' | 'number' | 'select' | 'date'
      editorProps?: Record<string, any>
    }[]
    /** 新增行默认值 */
    rowDefaults?: Record<string, any>
    /** SelectTable 列定义 */
    selectTableColumns?: {
      prop: string
      label: string
      width?: number | string
      minWidth?: number | string
    }[]
  }>(),
  {
    orderId: '',
    title: '右侧标题',
    extraSearchFields: () => [],
    searchDisabled: false,
    showRequisitionButton: false,
    formColumns: () => [],
    rowDefaults: () => ({}),
    selectTableColumns: () => []
  }
)

// ==================== Emits ====================

const emit = defineEmits<{
  action: [action: string]
  requisition: []
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

// ==================== 检索状态 ====================

const searchValues = reactive<Record<string, any>>({})

function buildSearchValues() {
  props.extraSearchFields.forEach((f) => {
    if (!(f.prop in searchValues)) {
      searchValues[f.prop] = f.defaultValue ?? (f.type === 'date' ? null : '')
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

function openSelectTable(rowKey: string | number) {
  selectTargetRowId.value = String(rowKey)
  selectTableVisible.value = true
}

function handleSupplierSelect(_value: unknown, rows: SupplierItem[]) {
  const item = rows[0]
  if (!item) return
  const idx = details.value.findIndex((r) => r.id === selectTargetRowId.value)
  if (idx === -1) return

  const actualAmount = computeActualAmount(item.purchasePrice, item.stockQuantity)
  details.value.splice(idx, 1, {
    ...details.value[idx],
    itemName: item.itemName,
    specification: item.specification,
    manufacturer: item.manufacturer,
    batchNo: computeBatchNo(item.itemCode),
    bookQuantity: item.stockQuantity,
    actualQuantity: item.stockQuantity,
    actualUnit: item.packageUnit,
    profitLoss: 0,
    unitPrice: item.purchasePrice,
    actualAmount,
    profitLossAmount: 0,
    remark: ''
  } as InventoryLineItem)

  selectTargetRowId.value = ''
}

// ==================== 实盘数量联动 ====================

function handleActualQuantityChange(row: Record<string, any>, value: number) {
  const idx = details.value.findIndex((r) => r.id === row.id)
  if (idx === -1) return
  const actualAmount = computeActualAmount(row.unitPrice, value)
  details.value.splice(idx, 1, {
    ...row,
    actualQuantity: value,
    profitLoss: value - row.bookQuantity,
    actualAmount,
    profitLossAmount: computeProfitLossAmount(actualAmount, row.unitPrice, row.bookQuantity)
  } as InventoryLineItem)
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

function handleRowAdd(row: Record<string, any>) {
  Object.entries(props.rowDefaults).forEach(([key, val]) => {
    row[key] = val
  })
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

.main-pane__search-btns {
  display: flex;
  align-items: center;
  gap: 8px;
  flex-shrink: 0;
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
