<template>
  <div class="main-pane">
    <DocumentHeadPanel
      :title="title"
      :status="order?.status ?? 0"
      :status-map="statusMap"
      :loading="loading"
    >
      <template #body>
        <!-- ==================== 检索区 ==================== -->
        <div v-if="extraSearchFields.length" class="main-pane__search">
          <template v-for="field in extraSearchFields" :key="field.prop">
            <div v-if="!field.hidden" class="main-pane__search-field">
              <label class="main-pane__search-label">
                <span v-if="field.required" class="main-pane__search-required">*</span>
                {{ field.label }}
              </label>
              <!-- 输入框 -->
              <el-input
                v-if="field.type === 'input'"
                v-model="searchValues[field.prop]"
                :placeholder="field.placeholder"
                :disabled="field.disabled || !canEditRows"
                size="default"
              />
              <!-- 下拉菜单 -->
              <el-select
                v-else-if="field.type === 'select'"
                v-model="searchValues[field.prop]"
                :placeholder="field.placeholder"
                :disabled="field.disabled || !canEditRows"
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
              <!-- 单个日期时间选择器 -->
              <el-date-picker
                v-else-if="field.type === 'datetime'"
                v-model="searchValues[field.prop]"
                type="datetime"
                :placeholder="field.placeholder"
                :disabled="field.disabled || !canEditRows"
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
          :disable-add="!canEditRows"
          :disable-delete="!canEditRows"
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

          <!-- 实盘数量：number输入框 + 单位输入框 -->
          <template v-if="hasColumn('actualQuantity')" #cell-actualQuantity="{ row }">
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
              <el-input :model-value="row.actualUnit" size="default" style="width: 70px" readonly />
            </div>
          </template>

          <!-- 账面数量：只读输入框 -->
          <template v-if="hasColumn('bookQuantity')" #cell-bookQuantity="{ row }">
            <el-input :model-value="row.bookQuantity" size="default" readonly />
          </template>

          <!-- 盈亏数量：计算展示 -->
          <template v-if="hasColumn('profitLoss')" #cell-profitLoss="{ row }">
            <span>{{ row.profitLoss }}</span>
          </template>

          <!-- 单价：格式化展示 -->
          <template v-if="hasColumn('unitPrice')" #cell-unitPrice="{ row }">
            <span>{{ row.unitPrice != null ? row.unitPrice.toFixed(2) + '元' : '' }}</span>
          </template>

          <!-- 实盘金额：计算展示 -->
          <template v-if="hasColumn('actualAmount')" #cell-actualAmount="{ row }">
            <span>{{ row.actualAmount != null ? row.actualAmount.toFixed(2) : '' }}</span>
          </template>

          <!-- 盈亏金额：格式化展示 -->
          <template v-if="hasColumn('profitLossAmount')" #cell-profitLossAmount="{ row }">
            <span>{{
              row.profitLossAmount != null ? row.profitLossAmount.toFixed(2) + '元' : ''
            }}</span>
          </template>

          <!-- 生产日期~有效期至：日期范围选择器 -->
          <template v-if="hasColumn('timeRange')" #cell-timeRange="{ row }">
            <el-date-picker
              :model-value="row.timeRange"
              type="daterange"
              start-placeholder="生产日期"
              end-placeholder="有效期至"
              style="width: 230px"
              @update:model-value="(v: string) => (row.timeRange = v)"
            >
            </el-date-picker>
          </template>

          <!-- 追溯码：输入框 -->
          <template v-if="hasColumn('traceCode')" #cell-traceCode="{ row }">
            <el-input
              :model-value="row.traceCode"
              size="default"
              @update:model-value="(v: string) => (row.traceCode = v)"
            />
          </template>

          <!-- 备注：输入框 -->
          <template v-if="hasColumn('remark')" #cell-remark="{ row }">
            <el-input
              :model-value="row.remark"
              size="default"
              @update:model-value="(v: string) => (row.remark = v)"
            />
          </template>

          <!-- 操作：删除链接 -->
          <template #actions="{ index }">
            <el-link type="danger" :underline="false" @click="deleteRow(index)">删除</el-link>
          </template>
        </DocumentDetailEditableTable>
      </template>

      <!-- ==================== 底部按钮 ==================== -->
      <template #extra>
        <div class="main-pane__footer">
          <el-button color="#006BE6" :disabled="!btnVisible.save" @click="emit('action', 'save')"
            >保存单据</el-button
          >
          <el-button
            color="#006BE6"
            plain
            :disabled="!btnVisible.saveAndSubmit"
            @click="emit('action', 'saveAndSubmit')"
            >保存并提交审批</el-button
          >
          <el-button
            color="#FF3860"
            plain
            :disabled="!btnVisible.delete"
            @click="emit('action', 'delete')"
            >删除单据</el-button
          >
          <span class="main-pane__divider" />
          <el-button
            color="#006BE6"
            plain
            :disabled="!btnVisible.submit"
            @click="emit('action', 'submit')"
            >提交审批</el-button
          >
          <el-button :disabled="!btnVisible.withdraw" @click="emit('action', 'withdraw')"
            ><el-icon><RefreshLeft /></el-icon>&nbsp;撤回</el-button
          >
          <span class="main-pane__divider" />
          <el-button
            color="#57D188"
            style="color: #ffffff"
            :disabled="!btnVisible.approve"
            @click="emit('action', 'approve')"
            ><el-icon><Check /></el-icon>审批</el-button
          >
          <el-button
            color="#EFBD48"
            style="color: #ffffff"
            :disabled="!btnVisible.reject"
            @click="emit('action', 'reject')"
            >驳回</el-button
          >
        </div>
      </template>
    </DocumentHeadPanel>

    <!-- 表单项选择弹窗 -->
    <SelectTable
      v-model:visible="selectTableVisible"
      title="选择项目"
      row-key="itemCode"
      :columns="selectTableColumns as any"
      :load-page="selectTableLoadPage!"
      @confirm="handleSupplierSelect"
    />
  </div>
</template>

<script setup lang="ts">
import { ref, reactive, computed, watch, onMounted } from 'vue'
import { Edit, RefreshLeft, Check } from '@element-plus/icons-vue'
import DocumentHeadPanel from '@/components/warehouse/DocumentHeadPanel.vue'
import DocumentDetailEditableTable from '@/components/warehouse/DocumentDetailEditableTable.vue'
import SelectTable from '@/components/common/SelectTable.vue'

// ==================== 类型 ====================

export interface ExtraSearchField {
  label: string
  prop: string
  type: 'input' | 'select' | 'datetime'
  placeholder?: string
  options?: { label: string; value: string }[]
  defaultValue?: string
  disabled?: boolean
  /** 是否隐藏该字段 */
  hidden?: boolean
  /** 是否必填 */
  required?: boolean
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
    /** SelectTable 分页加载函数 */
    selectTableLoadPage?: (query: {
      keyword: string
      pageIndex: number
      pageSize: number
    }) => Promise<{ rows: any[]; total: number }>
    /** 当前单据数据（父组件加载后传入） */
    orderData?: Record<string, any> | null
    /** SelectTable 确认回调：接收当前行和选中项，返回填充后的行 */
    onSupplierFill?: (row: Record<string, any>, item: Record<string, any>) => Record<string, any>
    /** 数量变更回调：接收当前行和新值，返回更新后的行 */
    onQuantityChange?: (row: Record<string, any>, value: number) => Record<string, any>
    /** 需要隐藏的底部按钮 action key 列表 */
    hiddenFooterActions?: string[]
    /** 自定义加载详情的函数（不传则用默认 import） */
    loadDetailFn?: (id: string) => Promise<{ order: any; details: any[] }>
  }>(),
  {
    orderId: '',
    title: '右侧标题',
    extraSearchFields: () => [],
    searchDisabled: false,
    showRequisitionButton: false,
    formColumns: () => [],
    rowDefaults: () => ({}),
    selectTableColumns: () => [],
    selectTableLoadPage: undefined,
    orderData: null,
    onSupplierFill: undefined,
    onQuantityChange: undefined,
    hiddenFooterActions: () => [],
    loadDetailFn: undefined
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
  { label: '待审核', value: 1 },
  { label: '审核中', value: 2 },
  { label: '已撤回', value: 3 },
  { label: '同意', value: 4 },
  { label: '驳回', value: 5 }
]

// ==================== 检索状态 ====================

const searchValues = reactive<Record<string, any>>({})

function buildSearchValues() {
  props.extraSearchFields.forEach((f) => {
    if (!(f.prop in searchValues)) {
      searchValues[f.prop] = f.defaultValue ?? (f.type === 'datetime' ? null : '')
    }
  })
}

// ==================== 明细状态 ====================

const details = ref<any[]>([])
const order = ref<Record<string, any> | null>(null)
const loading = ref(false)

// ==================== 状态权限控制 ====================

const currentStatus = computed(() => (order.value?.status as number) ?? 0)

/** 右侧可编辑（添加行/删除行/检索区）: 仅编辑中(0)、待审核(1) */
const canEditRows = computed(() => currentStatus.value === 0 || currentStatus.value === 1)

/** 底部按钮可见性 */
const btnVisible = computed(() => {
  const s = currentStatus.value
  return {
    save: s === 0 || s === 1, // 保存单据
    saveAndSubmit: s === 0 || s === 1, // 保存并提交审批
    delete: s === 0 || s === 1, // 删除单据
    submit: s === 0 || s === 1 || s === 3, // 提交审批
    withdraw: s === 2, // 撤回
    approve: s === 2, // 审批
    reject: s === 2 // 驳回
  }
})
/** SelectTable 弹窗状态 */
const selectTableVisible = ref(false)
const selectTargetRowId = ref<string>('')

// ==================== SelectTable ====================

function openSelectTable(rowKey: string | number) {
  selectTargetRowId.value = String(rowKey)
  selectTableVisible.value = true
}

function handleSupplierSelect(_value: unknown, rows: any[]) {
  const item = rows[0]
  if (!item || !props.onSupplierFill) return
  const idx = details.value.findIndex((r: any) => r.id === selectTargetRowId.value)
  if (idx === -1) return

  details.value.splice(idx, 1, props.onSupplierFill({ ...details.value[idx] }, item))
  selectTargetRowId.value = ''
}

// ==================== 数量变更 ====================

function handleActualQuantityChange(row: Record<string, any>, value: number) {
  if (!props.onQuantityChange) return
  const idx = details.value.findIndex((r: any) => r.id === row.id)
  if (idx === -1) return
  details.value.splice(idx, 1, props.onQuantityChange({ ...row }, value))
}

// ==================== 方法 ====================

function handleRowAdd(row: Record<string, any>) {
  Object.entries(props.rowDefaults).forEach(([key, val]) => {
    row[key] = val
  })
}

/** 检查 formColumns 中是否包含指定 prop 的列 */
function hasColumn(prop: string): boolean {
  return props.formColumns.some((c: any) => c.prop === prop)
}

function deleteRow(index: number) {
  details.value.splice(index, 1)
}

// ==================== 监听 ====================

watch(() => props.extraSearchFields, buildSearchValues, { immediate: true })
// ==================== 加载单据详情 ====================

async function loadDetail() {
  if (!props.orderId) {
    order.value = null
    details.value = []
    return
  }

  loading.value = true
  try {
    const fetcher = props.loadDetailFn
    if (!fetcher) {
      console.error('[PurchaseInboundMainPane] loadDetailFn 未提供，请在父组件传入 :load-detail-fn')
      order.value = null
      details.value = []
      loading.value = false
      return
    }
    const data = await fetcher(props.orderId)
    order.value = data.order as unknown as Record<string, any>
    details.value = (data.details as any[]) ?? []

    // 检索区同步卡片数据
    if (data.order) {
      searchValues['docNo'] = data.order.busNo ?? ''
      searchValues['pharmacy'] = data.order.sourceLocationId_dictText ?? ''
      searchValues['warehouse'] = data.order.sourceLocationId_dictText ?? ''
      searchValues['itemType'] = data.order.category ?? ''
      searchValues['targetPharmacy'] = data.order.purposeLocationId_dictText ?? ''
    }
  } catch {
    order.value = null
    details.value = []
  } finally {
    loading.value = false
  }
}

// ==================== 监听 ====================

watch(() => props.orderId, loadDetail)
watch(() => props.extraSearchFields, buildSearchValues, { immediate: true })

onMounted(() => {
  if (props.orderId) loadDetail()
})

// 向父组件暴露的属性
defineExpose({
  details,
  loadDetail,
  getSearchValue: (key: string) => searchValues[key],
  setSearchValue: (key: string, val: unknown) => {
    searchValues[key] = val
  },
  resetSearchValues: () => {
    for (const k of Object.keys(searchValues)) delete searchValues[k]
    buildSearchValues()
  }
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

.main-pane__search-required {
  margin-right: 2px;
  color: var(--el-color-danger);
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
