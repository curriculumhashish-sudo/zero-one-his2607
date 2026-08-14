<script setup lang="ts" generic="T extends Record<string, unknown>">
import type {
  SelectTableColumn,
  SelectTableLoadPage,
  SelectValue
} from './select-type'

import { computed, nextTick, onBeforeUnmount, ref, shallowRef, watch } from 'vue'

import type { TableInstance } from 'element-plus'

defineOptions({
  name: 'SelectTable'
})

interface SelectTableProps<TRow extends Record<string, unknown>> {
  columns: SelectTableColumn<TRow>[]
  rowKey?: keyof TRow & string | string
  selectionMode?: 'single' | 'multiple'
  loadPage?: SelectTableLoadPage<TRow>
  data?: TRow[]
  selectedRows?: TRow[]
  disabledRow?: (row: TRow) => boolean
  title?: string
  width?: string | number
  pageSize?: number
  pageSizes?: number[]
  keywordPlaceholder?: string
  searchLabel?: string
  emptyText?: string
}

const props = withDefaults(defineProps<SelectTableProps<T>>(), {
  rowKey: 'id',
  selectionMode: 'single',
  loadPage: undefined,
  data: () => [],
  selectedRows: () => [],
  disabledRow: undefined,
  title: '请选择',
  width: '900px',
  pageSize: 10,
  pageSizes: () => [10, 20, 50],
  keywordPlaceholder: '请输入关键字',
  searchLabel: '关键字',
  emptyText: '暂无匹配数据'
})

const emit = defineEmits<{
  confirm: [value: SelectValue | SelectValue[] | null, rows: T[]]
  cancel: []
  search: [keyword: string]
  pageChange: [pageIndex: number, pageSize: number]
  loadError: [error: unknown]
}>()

const modelValue = defineModel<SelectValue | SelectValue[] | null>({ default: null })
const visible = defineModel<boolean>('visible', { default: false })

const tableRef = shallowRef<TableInstance>()
const rows = shallowRef<T[]>([])
const total = ref(0)
const pageIndex = ref(1)
const currentPageSize = ref(props.pageSize)
const keyword = ref('')
const loading = ref(false)
const errorMessage = ref('')
const draftKeys = ref<SelectValue[]>([])
const draftRows = shallowRef(new Map<SelectValue, T>())

let requestController: AbortController | undefined
let requestSequence = 0
let syncingSelection = false
let closeReason: 'confirm' | 'cancel' | null = null

const isMultiple = computed(() => props.selectionMode === 'multiple')
const selectedCount = computed(() => draftKeys.value.length)

function getRowKey(row: T) {
  return row[props.rowKey] as SelectValue
}

function getTableRowKey(row: T) {
  return String(getRowKey(row))
}

function isRowDisabled(row: T) {
  return props.disabledRow?.(row) ?? false
}

function normalizeValue(value: SelectValue | SelectValue[] | null) {
  if (Array.isArray(value)) return [...value]
  return value === null || value === undefined ? [] : [value]
}

function resetDraft() {
  const nextKeys = normalizeValue(modelValue.value)
  const nextRows = new Map<SelectValue, T>()

  for (const row of [...props.selectedRows, ...rows.value]) {
    const key = getRowKey(row)
    if (nextKeys.includes(key)) nextRows.set(key, row)
  }

  draftKeys.value = nextKeys
  draftRows.value = nextRows
}

function cancelPendingRequest() {
  requestController?.abort()
  requestController = undefined
}

function filterLocalRows(source: T[], query: string) {
  const normalized = query.trim().toLocaleLowerCase()
  if (!normalized) return source

  return source.filter((row) =>
    props.columns.some((column) => {
      const value = column.formatter?.(row, column) ?? row[column.prop]
      return String(value ?? '')
        .toLocaleLowerCase()
        .includes(normalized)
    })
  )
}

async function syncTableSelection() {
  await nextTick()
  if (!tableRef.value) return

  syncingSelection = true
  try {
    if (isMultiple.value) {
      tableRef.value.clearSelection()
      for (const row of rows.value) {
        if (draftKeys.value.includes(getRowKey(row))) {
          tableRef.value.toggleRowSelection(row, true)
        }
      }
    } else {
      const selectedRow = rows.value.find((row) => draftKeys.value[0] === getRowKey(row))
      tableRef.value.setCurrentRow(selectedRow)
    }
  } finally {
    await nextTick()
    syncingSelection = false
  }
}

async function loadData() {
  cancelPendingRequest()
  const controller = new AbortController()
  requestController = controller
  const currentSequence = ++requestSequence
  loading.value = true
  errorMessage.value = ''

  try {
    if (props.loadPage) {
      const result = await props.loadPage(
        {
          keyword: keyword.value.trim(),
          pageIndex: pageIndex.value,
          pageSize: currentPageSize.value
        },
        { signal: controller.signal }
      )

      if (controller.signal.aborted || currentSequence !== requestSequence) return
      rows.value = result.rows
      total.value = result.total
    } else {
      const filteredRows = filterLocalRows(props.data, keyword.value)
      const start = (pageIndex.value - 1) * currentPageSize.value
      rows.value = filteredRows.slice(start, start + currentPageSize.value)
      total.value = filteredRows.length
    }

    for (const row of rows.value) {
      const key = getRowKey(row)
      if (draftKeys.value.includes(key)) draftRows.value.set(key, row)
    }

    await syncTableSelection()
  } catch (error) {
    if (controller.signal.aborted || currentSequence !== requestSequence) return
    rows.value = []
    total.value = 0
    errorMessage.value = '数据加载失败，请重试'
    emit('loadError', error)
  } finally {
    if (currentSequence === requestSequence) loading.value = false
  }
}

function handleSearch() {
  pageIndex.value = 1
  emit('search', keyword.value.trim())
  void loadData()
}

function setKeyword(value: string) {
  keyword.value = value
}

function handlePageChange(value: number) {
  pageIndex.value = value
  emit('pageChange', pageIndex.value, currentPageSize.value)
  void loadData()
}

function handlePageSizeChange(value: number) {
  currentPageSize.value = value
  pageIndex.value = 1
  emit('pageChange', pageIndex.value, currentPageSize.value)
  void loadData()
}

function handleSelectionChange(selectedRows: T[]) {
  if (syncingSelection || !isMultiple.value) return

  const currentPageKeys = new Set(rows.value.map(getRowKey))
  const nextKeys = draftKeys.value.filter((key) => !currentPageKeys.has(key))
  const nextRows = new Map(draftRows.value)

  for (const key of currentPageKeys) nextRows.delete(key)

  for (const row of selectedRows) {
    const key = getRowKey(row)
    nextKeys.push(key)
    nextRows.set(key, row)
  }

  draftKeys.value = [...new Set(nextKeys)]
  draftRows.value = nextRows
}

function handleRowClick(row: T) {
  if (isMultiple.value || isRowDisabled(row)) return

  const key = getRowKey(row)
  draftKeys.value = [key]
  draftRows.value = new Map([[key, row]])
  tableRef.value?.setCurrentRow(row)
}

function clearDraft() {
  draftKeys.value = []
  draftRows.value = new Map()
  tableRef.value?.clearSelection()
  tableRef.value?.setCurrentRow()
}

function handleConfirm() {
  const value: SelectValue | SelectValue[] | null = isMultiple.value
    ? [...draftKeys.value]
    : (draftKeys.value[0] ?? null)
  const selectedRows = draftKeys.value
    .map((key) => draftRows.value.get(key))
    .filter((row): row is T => row !== undefined)

  modelValue.value = value
  closeReason = 'confirm'
  visible.value = false
  emit('confirm', value, selectedRows)
}

function handleCancel() {
  closeReason = 'cancel'
  visible.value = false
}

function handleClosed() {
  if (closeReason !== 'confirm') emit('cancel')
  closeReason = null
}

watch(
  () => visible.value,
  (isVisible) => {
    if (!isVisible) return
    pageIndex.value = 1
    currentPageSize.value = props.pageSize
    resetDraft()
    void loadData()
  }
)

watch(
  () => props.data,
  () => {
    if (visible.value && !props.loadPage) void loadData()
  }
)

watch(
  () => modelValue.value,
  () => {
    if (!visible.value) resetDraft()
  }
)

onBeforeUnmount(() => {
  cancelPendingRequest()
  requestSequence += 1
})

defineExpose({
  open() {
    visible.value = true
  },
  close() {
    visible.value = false
  },
  reload: loadData,
  clear: clearDraft
})
</script>

<template>
  <el-dialog
    v-model="visible"
    :title="title"
    :width="width"
    append-to-body
    destroy-on-close
    :close-on-click-modal="false"
    @closed="handleClosed"
  >
    <div class="select-table">
      <slot
        name="search"
        :keyword="keyword"
        :set-keyword="setKeyword"
        :search="handleSearch"
        :loading="loading"
      >
        <el-form class="select-table__search" inline @submit.prevent="handleSearch">
          <el-form-item :label="searchLabel">
            <el-input
              v-model="keyword"
              clearable
              :placeholder="keywordPlaceholder"
              @clear="handleSearch"
              @keyup.enter="handleSearch"
            />
          </el-form-item>
          <el-form-item>
            <el-button type="primary" :loading="loading" @click="handleSearch">查询</el-button>
            <el-button :disabled="loading" @click="loadData">刷新</el-button>
          </el-form-item>
        </el-form>
      </slot>

      <el-alert
        v-if="errorMessage"
        class="select-table__error"
        type="error"
        :title="errorMessage"
        show-icon
        :closable="false"
      >
        <template #default>
          <el-button type="primary" link :disabled="loading" @click="loadData">重新加载</el-button>
        </template>
      </el-alert>

      <el-table
        ref="tableRef"
        v-loading="loading"
        :data="rows"
        :row-key="getTableRowKey"
        border
        stripe
        highlight-current-row
        max-height="420"
        @row-click="handleRowClick"
        @selection-change="handleSelectionChange"
      >
        <el-table-column
          v-if="isMultiple"
          type="selection"
          width="48"
          reserve-selection
          :selectable="(row: T) => !isRowDisabled(row)"
        />

        <el-table-column
          v-for="column in columns"
          :key="column.prop"
          :prop="column.prop"
          :label="column.label"
          :width="column.width"
          :min-width="column.minWidth"
          :align="column.align"
          :fixed="column.fixed"
          :show-overflow-tooltip="column.showOverflowTooltip ?? true"
          :sortable="column.sortable"
        >
          <template #default="scope">
            <slot
              :name="`cell-${column.prop}`"
              :row="scope.row"
              :column="column"
              :index="scope.$index"
            >
              {{ column.formatter?.(scope.row, column) ?? scope.row[column.prop] ?? '—' }}
            </slot>
          </template>
        </el-table-column>

        <template #empty>
          <slot name="empty" :error="errorMessage" :reload="loadData">
            <el-empty :description="errorMessage || emptyText" :image-size="72" />
          </slot>
        </template>
      </el-table>

      <div class="select-table__bottom">
        <div class="select-table__selected">
          <slot name="selected" :keys="draftKeys" :rows="[...draftRows.values()]" :clear="clearDraft">
            <span>已选 {{ selectedCount }} 项</span>
            <el-button v-if="selectedCount" type="primary" link @click="clearDraft">清空</el-button>
          </slot>
        </div>

        <el-pagination
          v-model:current-page="pageIndex"
          v-model:page-size="currentPageSize"
          :page-sizes="pageSizes"
          :total="total"
          layout="total, sizes, prev, pager, next"
          @current-change="handlePageChange"
          @size-change="handlePageSizeChange"
        />
      </div>
    </div>

    <template #footer>
      <slot
        name="footer"
        :confirm="handleConfirm"
        :cancel="handleCancel"
        :selected-count="selectedCount"
      >
        <el-button @click="handleCancel">取消</el-button>
        <el-button type="primary" :disabled="selectedCount === 0" @click="handleConfirm">
          确认选择
        </el-button>
      </slot>
    </template>
  </el-dialog>
</template>

<style scoped>
.select-table {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 12px;
}

.select-table__search {
  display: flex;
  flex-wrap: wrap;
  align-items: flex-start;
  gap: 8px;
}

.select-table__search :deep(.el-form-item) {
  margin-right: 0;
  margin-bottom: 0;
}

.select-table__search :deep(.el-input) {
  width: 260px;
}

.select-table__error {
  margin-bottom: 2px;
}

.select-table__bottom {
  display: flex;
  min-width: 0;
  flex-wrap: wrap;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}

.select-table__selected {
  display: flex;
  align-items: center;
  gap: 8px;
  color: var(--el-text-color-regular);
  font-size: 13px;
}

@media (max-width: 720px) {
  .select-table__bottom {
    align-items: flex-start;
    flex-direction: column;
  }

  .select-table__search :deep(.el-input) {
    width: min(100%, 260px);
  }
}
</style>
