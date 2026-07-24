<template>
  <div class="main-pane">
    <DocumentHeadPanel
      :title="title"
      :status="order?.status ?? 0"
      :status-map="statusMap"
      :loading="loading || actionLoading"
      :readonly="readonly"
    >
      <template #body>
        <div v-if="extraSearchFields.length" class="main-pane__search">
          <div v-for="field in extraSearchFields" :key="field.prop" class="main-pane__search-field">
            <label class="main-pane__search-label">{{ field.label }}</label>
            <el-input
              v-if="field.type === 'input'"
              v-model="searchValues[field.prop]"
              :placeholder="field.placeholder"
              :disabled="field.disabled || readonly"
            />
            <el-select
              v-else
              v-model="searchValues[field.prop]"
              :placeholder="field.placeholder"
              :disabled="field.disabled || readonly"
              clearable
            >
              <el-option
                v-for="option in field.options"
                :key="option.value"
                :label="option.label"
                :value="option.value"
              />
            </el-select>
          </div>
        </div>

        <DocumentDetailEditableTable
          ref="detailTableRef"
          v-model="details"
          :columns="columns"
          :loading="loading"
          :readonly="readonly"
          row-key="id"
          :show-add="!readonly"
          :show-delete="!readonly"
          @row-add="handleRowAdd"
        >
          <template #cell-itemName="{ row, rowKey }">
            <div
              class="main-pane__cell-edit"
              :class="{ 'is-readonly': readonly }"
              @click="openSelectTable(rowKey)"
            >
              <el-icon v-if="!readonly"><Edit /></el-icon>
              <el-input
                :model-value="row.itemName"
                placeholder="点击选择药品"
                readonly
                :disabled="readonly"
              />
            </div>
          </template>

          <template #cell-bookQuantity="{ row }">
            <div class="main-pane__cell-edit">
              <el-icon v-if="!readonly"><Edit /></el-icon>
              <el-input-number
                :model-value="row.bookQuantity"
                :min="0"
                :disabled="readonly"
                controls-position="right"
                style="width: 90px"
                @change="(value: number | undefined) => handleBookQuantityChange(row, value)"
              />
            </div>
          </template>

          <template #cell-actualQuantity="{ row }">
            <div class="main-pane__cell-edit">
              <el-input-number
                :model-value="row.actualQuantity"
                :min="0"
                :disabled="readonly"
                controls-position="right"
                style="width: 90px"
                @change="(value: number | undefined) => handleActualQuantityChange(row, value)"
              />
              <el-select
                :model-value="row.actualUnit"
                :disabled="readonly"
                style="width: 72px"
                @change="(value: string) => updateActualUnit(row, value)"
              >
                <el-option
                  v-for="option in unitOptions"
                  :key="option.value"
                  :label="option.label"
                  :value="option.value"
                />
              </el-select>
            </div>
          </template>

          <template #cell-profitLoss="{ row }">{{ row.profitLoss }}</template>
          <template #cell-unitPrice="{ row }">{{ formatAmount(row.unitPrice) }}</template>
          <template #cell-actualAmount="{ row }">{{ formatAmount(row.actualAmount) }}</template>
          <template #cell-profitLossAmount="{ row }">
            {{ formatAmount(row.profitLossAmount) }}
          </template>
          <template #cell-remark="{ row }">
            <el-input
              :model-value="row.remark"
              :disabled="readonly"
              @update:model-value="(value: string) => updateRemark(row, value)"
            />
          </template>
          <template #actions="{ index }">
            <el-link v-if="!readonly" type="danger" :underline="false" @click="deleteRow(index)">
              删除
            </el-link>
          </template>
        </DocumentDetailEditableTable>
      </template>

      <template #extra>
        <div class="main-pane__footer">
          <template v-if="canEdit">
            <el-button type="primary" :loading="actionLoading" @click="handleAction('save')">
              保存单据
            </el-button>
            <el-button
              type="success"
              :loading="actionLoading"
              @click="handleAction('saveAndSubmit')"
            >
              保存并提交审批
            </el-button>
            <el-button type="danger" :disabled="actionLoading" @click="handleAction('delete')">
              删除单据
            </el-button>
            <el-button type="warning" :loading="actionLoading" @click="handleAction('submit')">
              提交审批
            </el-button>
          </template>
          <template v-else-if="order?.status === 1">
            <el-button :loading="actionLoading" @click="handleAction('withdraw')">撤回</el-button>
            <el-button type="success" :loading="actionLoading" @click="handleAction('approve')">
              审批通过
            </el-button>
            <el-button type="danger" :loading="actionLoading" @click="handleAction('reject')">
              驳回
            </el-button>
          </template>
          <span v-else class="main-pane__completed-text">盘点单已完成，当前为只读状态</span>
        </div>
      </template>
    </DocumentHeadPanel>

    <SelectTable
      v-model:visible="selectTableVisible"
      title="选择盘点项目"
      row-key="itemCode"
      :columns="selectTableColumns"
      :load-page="loadSupplierOptions"
      @confirm="handleProductSelect"
    />
  </div>
</template>

<script setup lang="ts">
import { computed, reactive, ref, watch } from 'vue'
import { Edit } from '@element-plus/icons-vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import DocumentHeadPanel from '@/components/warehouse/DocumentHeadPanel.vue'
import DocumentDetailEditableTable from '@/components/warehouse/DocumentDetailEditableTable.vue'
import SelectTable from '@/components/common/SelectTable.vue'
import type { SelectTableColumn } from '@/components/common/select-type'
import {
  approvePurchaseInbound,
  createPurchaseInbound,
  getPurchaseInboundDetail,
  loadSupplierOptions,
  rejectPurchaseInbound,
  removePurchaseInbound,
  submitPurchaseInbound,
  updatePurchaseInbound,
  withdrawPurchaseInbound
} from '@/apis/warehouse/purchase-inbound'
import type {
  PurchaseInboundOrder,
  PurchaseInboundSavePayload,
  SupplierItem
} from '@/apis/warehouse/purchase-inbound'
import type { InventoryLineItem } from '../type'
import { computeActualAmount, computeBatchNo, computeProfitLossAmount } from '../utils'

export interface ExtraSearchField {
  label: string
  prop: string
  type: 'input' | 'select'
  placeholder?: string
  options?: { label: string; value: string }[]
  defaultValue?: string
  disabled?: boolean
}

const props = withDefaults(
  defineProps<{
    selectedOrder?: PurchaseInboundOrder | null
    title?: string
    extraSearchFields?: ExtraSearchField[]
  }>(),
  {
    selectedOrder: null,
    title: '药房盘点单',
    extraSearchFields: () => []
  }
)

const emit = defineEmits<{
  changed: [order: PurchaseInboundOrder]
  removed: [id: string]
}>()

const statusMap = [
  { label: '编辑中', value: 0, type: 'primary' as const },
  { label: '待审批', value: 1, type: 'warning' as const },
  { label: '已完成', value: 2, type: 'success' as const },
  { label: '已驳回', value: 3, type: 'danger' as const }
]

const unitOptions = ['支', '瓶', '盒', '袋'].map((value) => ({ label: value, value }))
const columns = [
  { prop: 'itemName', label: '项目名称', width: 200 },
  { prop: 'specification', label: '规格', width: 120 },
  { prop: 'manufacturer', label: '厂家', minWidth: 160 },
  { prop: 'batchNo', label: '批号', width: 120 },
  { prop: 'bookQuantity', label: '账面数量', width: 100 },
  { prop: 'actualQuantity', label: '实盘数量', width: 180 },
  { prop: 'profitLoss', label: '盈亏数量', width: 90 },
  { prop: 'unitPrice', label: '单价', width: 100 },
  { prop: 'actualAmount', label: '实盘金额', width: 100 },
  { prop: 'profitLossAmount', label: '盈亏金额', width: 100 },
  { prop: 'remark', label: '备注', width: 140 }
]
const selectTableColumns: SelectTableColumn<SupplierItem>[] = [
  { prop: 'itemCode', label: '项目编码', width: 100 },
  { prop: 'itemName', label: '项目名称', minWidth: 140 },
  { prop: 'manufacturer', label: '厂家/产地', minWidth: 160 },
  { prop: 'specification', label: '规格', width: 120 },
  { prop: 'stockQuantity', label: '库存数量', width: 90 },
  { prop: 'packageUnit', label: '包装单位', width: 90 }
]

const searchValues = reactive<Record<string, string>>({})
const order = ref<PurchaseInboundOrder | null>(null)
const details = ref<InventoryLineItem[]>([])
const loading = ref(false)
const actionLoading = ref(false)
const detailTableRef = ref<InstanceType<typeof DocumentDetailEditableTable> | null>(null)
const selectTableVisible = ref(false)
const selectTargetRowId = ref('')

const canEdit = computed(() => !order.value || order.value.status === 0 || order.value.status === 3)
const readonly = computed(() => !canEdit.value)

function syncSearchValues() {
  for (const field of props.extraSearchFields) {
    searchValues[field.prop] = field.defaultValue ?? ''
  }
  searchValues.docNo = order.value?.docNo ?? ''
  searchValues.pharmacy = order.value?.type ?? ''
  searchValues.itemType = order.value?.category ?? ''
}

async function loadDetail() {
  const selected = props.selectedOrder
  if (!selected) {
    order.value = null
    details.value = []
    syncSearchValues()
    return
  }
  if (!selected.id) {
    order.value = { ...selected }
    details.value = []
    syncSearchValues()
    return
  }

  loading.value = true
  try {
    const result = await getPurchaseInboundDetail(selected.id)
    order.value = result.order
    // 修复：历史实现获取详情后直接清空数组，导致历史明细永远无法回显。
    details.value = result.details.map((item) => ({ ...item }))
    syncSearchValues()
  } catch (error) {
    order.value = null
    details.value = []
    ElMessage.error(error instanceof Error ? error.message : '盘点单详情加载失败')
  } finally {
    loading.value = false
  }
}

function asInventoryRow(row: unknown): InventoryLineItem {
  return row as InventoryLineItem
}

function openSelectTable(rowKey: string | number) {
  if (readonly.value) return
  selectTargetRowId.value = String(rowKey)
  selectTableVisible.value = true
}

function handleProductSelect(_value: unknown, rows: SupplierItem[]) {
  const item = rows[0]
  const row = details.value.find((detail) => detail.id === selectTargetRowId.value)
  if (!item || !row) return

  row.itemCode = item.itemCode
  row.itemName = item.itemName
  row.specification = item.specification
  row.manufacturer = item.manufacturer
  row.batchNo = computeBatchNo(item.itemCode)
  row.bookQuantity = item.stockQuantity
  row.actualQuantity = item.stockQuantity
  row.actualUnit = item.packageUnit
  row.unitPrice = item.purchasePrice
  recalculateRow(row)
  row.remark = ''
  selectTargetRowId.value = ''
}

function recalculateRow(row: InventoryLineItem) {
  row.profitLoss = row.actualQuantity - row.bookQuantity
  row.actualAmount = computeActualAmount(row.unitPrice, row.actualQuantity)
  row.profitLossAmount = computeProfitLossAmount(row.actualAmount, row.unitPrice, row.bookQuantity)
}

function handleActualQuantityChange(source: unknown, value: number | undefined) {
  const row = asInventoryRow(source)
  row.actualQuantity = value ?? 0
  recalculateRow(row)
}

function handleBookQuantityChange(source: unknown, value: number | undefined) {
  const row = asInventoryRow(source)
  row.bookQuantity = value ?? 0
  // 账面数量变化后必须同步重算盈亏，避免界面显示值与保存值不一致。
  recalculateRow(row)
}

function updateActualUnit(source: unknown, value: string) {
  asInventoryRow(source).actualUnit = value
}

function updateRemark(source: unknown, value: string) {
  asInventoryRow(source).remark = value
}

function handleRowAdd(source: unknown) {
  const row = asInventoryRow(source)
  Object.assign(row, {
    itemCode: '',
    itemName: '',
    specification: '',
    manufacturer: '',
    batchNo: '',
    bookQuantity: 0,
    actualQuantity: 0,
    actualUnit: '盒',
    profitLoss: 0,
    unitPrice: 0,
    actualAmount: 0,
    profitLossAmount: 0,
    remark: ''
  })
}

function deleteRow(index: number) {
  if (!readonly.value) details.value.splice(index, 1)
}

function formatAmount(value: unknown) {
  return `${Number(value || 0).toFixed(2)}元`
}

function buildPayload(): PurchaseInboundSavePayload | null {
  const pharmacy = searchValues.pharmacy?.trim()
  const itemType = searchValues.itemType?.trim()
  if (!pharmacy) {
    ElMessage.warning('请选择盘点药房')
    return null
  }
  if (!itemType) {
    ElMessage.warning('请选择项目类型')
    return null
  }
  if (details.value.length === 0) {
    ElMessage.warning('请至少添加一条盘点明细')
    return null
  }
  if (details.value.some((item) => !item.itemCode || !item.itemName)) {
    ElMessage.warning('请完成所有盘点项目的选择')
    return null
  }
  if (details.value.some((item) => item.actualQuantity < 0)) {
    ElMessage.warning('实盘数量不能小于零')
    return null
  }
  if (details.value.some((item) => item.bookQuantity < 0)) {
    ElMessage.warning('账面数量不能小于零')
    return null
  }
  return {
    pharmacy,
    itemType,
    details: details.value.map((item) => {
      const next = { ...item }
      recalculateRow(next)
      return next
    })
  }
}

async function persist(): Promise<PurchaseInboundOrder | null> {
  const payload = buildPayload()
  if (!payload) return null
  const result = order.value?.id
    ? await updatePurchaseInbound(order.value.id, payload)
    : await createPurchaseInbound(payload)
  order.value = result.order
  details.value = result.details.map((item) => ({ ...item }))
  syncSearchValues()
  emit('changed', result.order)
  return result.order
}

async function handleAction(action: string) {
  if (actionLoading.value) return
  actionLoading.value = true
  try {
    if (action === 'save') {
      if (await persist()) ElMessage.success('盘点单已保存')
      return
    }
    if (action === 'saveAndSubmit' || action === 'submit') {
      // 新建单据没有正式 ID，必须先保存，再执行状态流转。
      const saved = await persist()
      if (!saved) return
      order.value = await submitPurchaseInbound(saved.id)
      emit('changed', order.value)
      ElMessage.success('盘点单已提交审批')
      return
    }
    if (action === 'delete') {
      await ElMessageBox.confirm('确认删除当前盘点单？', '删除确认', { type: 'warning' })
      const id = order.value?.id ?? ''
      if (id) await removePurchaseInbound(id)
      emit('removed', id)
      ElMessage.success('盘点单已删除')
      return
    }
    if (!order.value?.id) return
    if (action === 'withdraw') {
      order.value = await withdrawPurchaseInbound(order.value.id)
      emit('changed', order.value)
      ElMessage.success('盘点单已撤回')
    } else if (action === 'approve') {
      await ElMessageBox.confirm('确认审批通过当前盘点单？', '审批确认', { type: 'warning' })
      order.value = await approvePurchaseInbound(order.value.id)
      emit('changed', order.value)
      ElMessage.success('盘点单已审批通过')
    } else if (action === 'reject') {
      const { value } = await ElMessageBox.prompt('请输入驳回原因', '驳回盘点单', {
        inputPattern: /\S+/,
        inputErrorMessage: '驳回原因不能为空'
      })
      order.value = await rejectPurchaseInbound(order.value.id, value)
      emit('changed', order.value)
      ElMessage.success('盘点单已驳回')
    }
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '操作失败')
  } finally {
    actionLoading.value = false
  }
}

watch(() => props.selectedOrder, loadDetail, { immediate: true })
watch(() => props.extraSearchFields, syncSearchValues, { deep: true })
</script>

<style scoped>
.main-pane {
  display: flex;
  flex-direction: column;
  height: 100%;
  min-height: 0;
}

.main-pane :deep(.doc-head-panel) {
  display: flex;
  flex: 1;
  flex-direction: column;
  min-height: 0;
}

.main-pane :deep(.doc-head-panel__body) {
  flex: 1;
  min-height: 0;
  overflow: auto;
}

.main-pane__search {
  display: flex;
  align-items: center;
  flex-wrap: wrap;
  gap: 16px;
  padding-bottom: 10px;
  margin-bottom: 10px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.main-pane__search-field,
.main-pane__cell-edit,
.main-pane__footer {
  display: flex;
  align-items: center;
  gap: 8px;
}

.main-pane__search-field .el-input,
.main-pane__search-field .el-select {
  width: 170px;
}

.main-pane__search-label {
  font-size: 13px;
  font-weight: 500;
  white-space: nowrap;
}

.main-pane__cell-edit:not(.is-readonly) {
  cursor: pointer;
}

.main-pane__cell-edit .el-icon {
  color: var(--el-color-primary);
}

.main-pane__footer {
  justify-content: flex-end;
  flex-wrap: wrap;
}

.main-pane__completed-text {
  color: var(--el-text-color-secondary);
}
</style>
