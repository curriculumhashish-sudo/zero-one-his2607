<template>
  <section class="advice-order-table" :aria-busy="loading">
    <header v-if="$slots.toolbar" class="advice-order-table__toolbar">
      <slot name="toolbar" />
    </header>

    <el-table
      ref="tableRef"
      v-loading="loading"
      :data="displayRows"
      :row-key="String(rowKey)"
      :height="height"
      :border="border"
      :stripe="stripe"
      :scrollbar-always-on="scrollbarAlwaysOn"
      highlight-current-row
      :row-class-name="rowClass"
      @selection-change="onSelectionChange"
      @current-change="onCurrentChange"
    >
      <el-table-column
        v-if="selectable"
        type="selection"
        width="44"
        fixed="left"
        :selectable="canSelectRow"
      />
      <el-table-column
        v-for="column in columns"
        :key="column.prop"
        :prop="column.prop"
        :label="column.label"
        :width="column.width"
        :min-width="column.minWidth"
        :fixed="column.fixed"
        :align="column.align"
        :show-overflow-tooltip="column.showOverflowTooltip"
      >
        <template #default="scope">
          <slot :name="`cell-${column.prop}`" :row="scope.row as AdviceOrder" :column="column">
            <slot v-if="column.prop === 'order'" name="order" :row="scope.row as AdviceOrder">
              <AdviceOrderDisplayCell :order="scope.row as AdviceOrder" compact />
            </slot>
            <template v-else>{{ cellText(scope.row as AdviceOrder, column) }}</template>
          </slot>
        </template>
      </el-table-column>
      <el-table-column v-if="$slots.actions" label="操作" width="120" fixed="right">
        <template #default="scope">
          <slot
            name="actions"
            :row="scope.row as AdviceOrder"
            :action="
              (action: string) => emit('rowAction', { row: scope.row as AdviceOrder, action })
            "
          />
        </template>
      </el-table-column>
      <template #empty>
        <slot name="empty">{{ emptyText }}</slot>
      </template>
    </el-table>

    <footer v-if="$slots.summary" class="advice-order-table__summary">
      <slot name="summary" />
    </footer>
  </section>
</template>

<script setup lang="ts">
import { computed, ref } from 'vue'
import type { TableInstance } from 'element-plus'
import AdviceOrderDisplayCell from './AdviceOrderDisplayCell.vue'
import type { AdviceColumn, AdviceOrder, MedicalKey } from './type'

/**
 * C54 AdviceOrderTable —— 医嘱表格（门诊/住院通用）。
 *
 * 纯展示：行数据、列定义、分组、当前行均由页面注入。组件不请求医嘱接口、
 * 不写死列，也不内置分页数据；分页/排序仅透传事件给页面。
 */
defineOptions({ name: 'AdviceOrderTable' })

const props = withDefaults(
  defineProps<{
    /** 医嘱行数据 */
    rows?: AdviceOrder[]
    /** 行主键字段 */
    rowKey?: keyof AdviceOrder
    /** 列定义 */
    columns?: AdviceColumn[]
    /** 加载中 */
    loading?: boolean
    /** 表格内容区高度；传入 100% 时由父级 flex 布局分配剩余空间。 */
    height?: string | number
    /** 内容超宽时始终显示横向滚动条。 */
    scrollbarAlwaysOn?: boolean
    /** 只读：禁用勾选 */
    readonly?: boolean
    /** 是否显示勾选列 */
    selectable?: boolean
    /** 分组字段或取值函数（用于行样式分组） */
    groupBy?: keyof AdviceOrder | ((row: AdviceOrder) => string)
    /** 当前高亮行 key（受控） */
    currentRowKey?: MedicalKey | null
    /** 判断某行是否禁用勾选 */
    disabledRow?: (row: AdviceOrder) => boolean
    /** 空数据文案 */
    emptyText?: string
    /** 是否显示纵向边框。 */
    border?: boolean
    /** 是否显示斑马纹。 */
    stripe?: boolean
  }>(),
  {
    rows: () => [],
    rowKey: 'key',
    columns: () => [
      { prop: 'order', label: '医嘱内容', minWidth: 220 },
      { prop: 'status', label: '状态', width: 100 }
    ],
    loading: false,
    height: undefined,
    scrollbarAlwaysOn: true,
    readonly: false,
    selectable: false,
    groupBy: undefined,
    currentRowKey: null,
    disabledRow: () => false,
    emptyText: '暂无医嘱',
    border: false,
    stripe: false
  }
)

const tableRef = ref<TableInstance>()

const emit = defineEmits<{
  selectionChange: [rows: AdviceOrder[]]
  currentChange: [row: AdviceOrder | undefined]
  rowAction: [value: { row: AdviceOrder; action: string }]
}>()

const displayRows = computed(() => props.rows)

function cellText(row: AdviceOrder, column: AdviceColumn): string {
  if (column.formatter) return column.formatter(row)
  const value = row[column.prop as keyof AdviceOrder]
  return value === undefined || value === null || value === '' ? '-' : String(value)
}

function groupValue(row: AdviceOrder): string {
  if (typeof props.groupBy === 'function') return props.groupBy(row)
  if (props.groupBy) return String(row[props.groupBy] ?? '')
  return ''
}

function rowClass({ row }: { row: AdviceOrder }): string {
  const group = groupValue(row)
  return [
    props.currentRowKey !== null && props.currentRowKey === row[props.rowKey] ? 'is-current' : '',
    group ? `advice-group-${group}` : ''
  ]
    .filter(Boolean)
    .join(' ')
}

function canSelectRow(row: AdviceOrder): boolean {
  return !props.readonly && !props.disabledRow(row)
}

function onSelectionChange(rows: AdviceOrder[]) {
  emit('selectionChange', rows)
}

function onCurrentChange(row: AdviceOrder | null) {
  emit('currentChange', row ?? undefined)
}

/** 清空表格选择，供切换患者或提交完成后调用。 */
function clearSelection() {
  tableRef.value?.clearSelection()
}

defineExpose({
  tableRef,
  clearSelection
})
</script>

<style scoped>
.advice-order-table {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  min-height: 0;
  overflow: hidden;
  flex-direction: column;
  background: var(--el-bg-color);
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
}

.advice-order-table > :deep(.el-table) {
  min-width: 0;
  min-height: 0;
  flex: 1;
}

.advice-order-table__toolbar,
.advice-order-table__summary {
  padding: 10px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.advice-order-table__summary {
  border-top: 1px solid var(--el-border-color-lighter);
  border-bottom: 0;
}

.advice-order-table :deep(.is-current td.el-table__cell) {
  background: var(--el-color-primary-light-9);
}
</style>
