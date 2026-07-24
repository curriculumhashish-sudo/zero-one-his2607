<template>
  <div class="ddet" v-loading="loading">
    <!-- ==================== Header：添加行 / 删除行 ==================== -->
    <div v-if="showAdd || showDelete || $slots.toolbar" class="ddet__toolbar">
      <slot name="toolbar" />
      <el-button
        v-if="showAdd && !readonly"
        size="default"
        type="primary"
        :disabled="maxRows > 0 && modelValue.length >= maxRows"
        @click="addRow"
      >
        添加行
      </el-button>
      <el-button
        v-if="showDelete && !readonly"
        size="default"
        type="danger"
        :disabled="selectedRows.length === 0"
        @click="deleteRows"
      >
        删除行
      </el-button>
    </div>

    <!-- ==================== Body：DataTablePanel ==================== -->
    <DataTablePanel
      :data="modelValue"
      :loading="loading"
      :total="0"
      :selectable="isSelectable"
      :show-index="showIndex"
      @selectable="onSelectionChange"
    >
      <!-- 动态生成列 -->
      <el-table-column
        v-for="col in columns"
        :key="col.prop"
        :prop="col.prop"
        :label="col.label"
        :width="col.width"
        :min-width="col.minWidth"
        :align="col.align || 'center'"
        :fixed="col.fixed"
        :class-name="`ddet-col-${String(col.prop)}`"
        show-overflow-tooltip
      >
        <template #default="{ row, $index }">
          <!-- 自定义编辑器 slot -->
          <template v-if="$slots[`editor-${col.prop}`]">
            <slot
              :name="`editor-${col.prop}`"
              :row="row"
              :value="row[col.prop]"
              :index="$index"
              :row-key="keyOf(row)"
              :error="err(keyOf(row), col.prop)"
            />
          </template>
          <!-- 自定义展示 slot -->
          <template v-else-if="$slots[`cell-${col.prop}`]">
            <slot
              :name="`cell-${col.prop}`"
              :row="row"
              :value="row[col.prop]"
              :index="$index"
              :row-key="keyOf(row)"
            />
          </template>
          <!-- 可编辑单元格 -->
          <template v-else-if="col.editable && !isCellReadonly(row, col)">
            <el-tooltip
              :content="err(keyOf(row), col.prop)"
              :visible="!!err(keyOf(row), col.prop)"
              placement="top"
            >
              <component
                :is="editorTag(col)"
                :model-value="row[col.prop]"
                size="small"
                v-bind="col.editorProps"
                :class="{ 'ddet-cell--error': err(keyOf(row), col.prop) }"
                @update:model-value="(v: any) => onChange(row, col.prop, v)"
                @blur="validateCell(row, col.prop)"
              >
                <template v-if="editorTag(col) === 'el-select'" #default>
                  <el-option
                    v-for="o in col.options || []"
                    :key="o.value"
                    :label="o.label"
                    :value="o.value"
                  />
                </template>
              </component>
            </el-tooltip>
          </template>
          <!-- 只读展示 -->
          <span v-else>{{ fmt(row, col.prop) }}</span>
        </template>
      </el-table-column>

      <!-- 操作列 -->
      <el-table-column v-if="$slots.actions" label="操作" width="120" fixed="right" align="center">
        <template #default="{ row, $index }">
          <slot
            name="actions"
            :row="row"
            :index="$index"
            :row-key="keyOf(row)"
            :disabled="isDisabled(row)"
          />
        </template>
      </el-table-column>
    </DataTablePanel>

    <!-- ==================== 空状态 / 汇总 ==================== -->
    <div v-if="!modelValue.length && $slots.empty" class="ddet__empty">
      <slot name="empty" />
    </div>
    <div v-if="$slots.summary" class="ddet__summary">
      <slot name="summary" />
    </div>
  </div>
</template>

<script lang="ts" setup>
import { ref, computed } from 'vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'

defineOptions({ name: 'DocumentDetailEditableTable' })

// ==================== 类型定义 ====================

interface EditableColumn {
  label: string
  prop: string
  width?: string | number
  minWidth?: string | number
  align?: 'left' | 'center' | 'right'
  fixed?: 'left' | 'right'
  editable?: boolean
  readonly?: boolean
  required?: boolean
  editorType?: 'input' | 'number' | 'select' | 'date'
  editorProps?: Record<string, any>
  options?: Array<{ label: string; value: any }>
  formatter?: (v: any, row: any) => string
  isAmount?: boolean
  isQty?: boolean
}

interface EditableTableRule {
  prop: string
  required?: boolean
  validator?: (v: any, row: any) => string | null
  min?: number
  max?: number
}

// ==================== Props & Emits ====================

const props = withDefaults(
  defineProps<{
    /** 明细行数据（v-model） */
    modelValue?: any[]
    /** 行唯一键 */
    rowKey?: string | ((row: any) => string)
    /** 列配置 */
    columns?: EditableColumn[]
    /** 整表只读 */
    readonly?: boolean
    /** 加载状态 */
    loading?: boolean
    /** 禁用行判断 */
    disabledRow?: (row: any) => boolean
    /** 校验规则 */
    rules?: EditableTableRule[]
    /** 是否显示"添加行"按钮 */
    showAdd?: boolean
    /** 是否显示"删除行"按钮 */
    showDelete?: boolean
    /** 最大行数 */
    maxRows?: number
    /** 是否显示序号列 */
    showIndex?: boolean
  }>(),
  {
    modelValue: () => [],
    rowKey: 'id',
    columns: () => [],
    readonly: false,
    loading: false,
    rules: () => [],
    showAdd: false,
    showDelete: false,
    maxRows: 0,
    showIndex: true
  }
)

const emit = defineEmits<{
  'update:modelValue': [rows: any[]]
  rowAdd: [row: any]
  rowDelete: [rows: any[]]
  rowChange: [payload: { rowKey: string | number; row: any }]
  cellChange: [payload: { rowKey: string | number; prop: string; value: any; row: any }]
  amountChange: [payload: { rowKey: string | number; prop: string; value: any; row: any }]
  qtyChange: [payload: { rowKey: string | number; prop: string; value: string | number; row: any }]
  validationError: [errors: Record<string, Record<string, string>>]
  selectionChange: [rows: any[]]
}>()

// ==================== 内部状态 ====================

const errors = ref<Record<string, Record<string, string>>>({})
const selectedRows = ref<any[]>([])
let tempId = 0

const EDITORS: Record<string, string> = {
  input: 'el-input',
  number: 'el-input-number',
  select: 'el-select',
  date: 'el-date-picker'
}

// ==================== 工具函数 ====================

const keyOf = (row: any): string | number =>
  !row
    ? ''
    : typeof props.rowKey === 'function'
      ? props.rowKey(row)
      : (row[props.rowKey as string] ?? '')

const colConfig = (prop: string) => props.columns?.find((c) => c.prop === prop)
const editorTag = (col: EditableColumn) => EDITORS[col.editorType || 'input'] || 'el-input'
const isDisabled = (row: any) => props.readonly || !!props.disabledRow?.(row)
const isCellReadonly = (row: any, col: EditableColumn) => isDisabled(row) || !!col.readonly
const err = (rk: string | number, prop: string) => errors.value[rk]?.[prop] || ''

const fmt = (row: any, prop: string) => {
  const v = row[prop]
  const c = colConfig(prop)
  return c?.formatter ? c.formatter(v, row) : v == null ? '' : String(v)
}

const isSelectable = computed(() => props.showDelete && !props.readonly)

// ==================== 选中行 ====================

function onSelectionChange(selection: any[]) {
  selectedRows.value = selection
  emit('selectionChange', selection)
}

// ==================== 新增行 ====================

function addRow() {
  if (props.maxRows > 0 && props.modelValue.length >= props.maxRows) return
  const newRow: any = {}
  const rk = `__temp_${++tempId}_${Date.now()}`
  if (typeof props.rowKey === 'string') newRow[props.rowKey] = rk
  else newRow._rowKey = rk
  props.columns?.forEach((c) => {
    newRow[c.prop] = c.editorProps?.default ?? (c.editorType === 'number' ? undefined : '')
  })
  emit('update:modelValue', [...props.modelValue, newRow])
  emit('rowAdd', newRow)
}

// ==================== 删除行 ====================

function deleteRows() {
  if (selectedRows.value.length === 0) return
  const deletedRows = [...selectedRows.value]
  const deletedKeys = new Set(deletedRows.map((row) => keyOf(row)))
  emit(
    'update:modelValue',
    props.modelValue.filter((row) => !deletedKeys.has(keyOf(row)))
  )
  emit('rowDelete', deletedRows)
  selectedRows.value = []
}

// ==================== 编辑 & 校验 ====================

function onChange(row: any, prop: string, value: any) {
  const rk = keyOf(row)
  const updatedRow = { ...row, [prop]: value }
  const updatedRows = props.modelValue.map((item) => (keyOf(item) === rk ? updatedRow : item))
  emit('update:modelValue', updatedRows)
  emit('rowChange', { rowKey: rk, row: updatedRow })
  emit('cellChange', { rowKey: rk, prop, value, row: updatedRow })
  const c = colConfig(prop)
  if (c?.isAmount) emit('amountChange', { rowKey: rk, prop, value, row: updatedRow })
  if (c?.isQty) emit('qtyChange', { rowKey: rk, prop, value, row: updatedRow })
}

function getRuleMessage(row: any, rule: EditableTableRule): string {
  const value = row[rule.prop]
  if (rule.required && (value == null || value === '')) {
    return `${colConfig(rule.prop)?.label || rule.prop} 不能为空`
  }
  if (rule.validator) return rule.validator(value, row) || ''
  if (rule.min != null && Number(value) < rule.min) return `最小值为 ${rule.min}`
  if (rule.max != null && Number(value) > rule.max) return `最大值为 ${rule.max}`
  return ''
}

function validateCell(row: any, prop: string) {
  const rk = keyOf(row)
  const rule = props.rules?.find((r) => r.prop === prop)
  if (!rule) return
  const msg = getRuleMessage(row, rule)

  if (!errors.value[rk]) errors.value[rk] = {}
  if (msg) {
    errors.value[rk][prop] = msg
  } else {
    delete errors.value[rk][prop]
    if (!Object.keys(errors.value[rk]).length) delete errors.value[rk]
  }
}

// ==================== 暴露方法 ====================

function validate(): Record<string, Record<string, string>> {
  clearValidate()
  props.modelValue.forEach((row) =>
    (props.rules || []).forEach((rule) => {
      const rk = keyOf(row)
      const msg = getRuleMessage(row, rule)
      if (msg) {
        if (!errors.value[rk]) errors.value[rk] = {}
        errors.value[rk][rule.prop] = msg
      }
    })
  )
  if (Object.keys(errors.value).length) emit('validationError', { ...errors.value })
  return { ...errors.value }
}

function clearValidate() {
  errors.value = {}
}

function focusCell(_rowKey: string | number, _prop: string) {
  const columnClass = `ddet-col-${CSS.escape(String(_prop))}`
  const el = document.querySelector(
    `.ddet td.${columnClass} input, .ddet td.${columnClass} .el-input__inner`
  )
  if (el) (el as HTMLElement).focus()
}

defineExpose({ validate, clearValidate, focusCell })
</script>

<style scoped>
/* ==================== 根容器 ==================== */
.ddet {
  display: flex;
  flex-direction: column;
  min-height: 0;
}

/* ==================== 工具栏 ==================== */
.ddet__toolbar {
  display: flex;
  align-items: center;
  gap: 8px;
  flex-shrink: 0;
  padding-bottom: 8px;
}

/* ==================== 空状态 / 汇总 ==================== */
.ddet__empty {
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 40px 16px;
}

.ddet__summary {
  flex-shrink: 0;
  padding-top: 8px;
  border-top: 1px solid var(--el-border-color-lighter);
}

/* ==================== 校验错误 ==================== */
.ddet-cell--error :deep(.el-input__wrapper),
.ddet-cell--error :deep(.el-input-number__wrapper) {
  box-shadow: 0 0 0 1px var(--el-color-danger) inset !important;
}
</style>
