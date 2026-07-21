<template>
  <div class="document-queue-list">
    <!-- ==================== 头部 ==================== -->
    <header v-if="$slots.filters || showCreate" class="document-queue-list__header">
      <div v-if="$slots.filters" class="document-queue-list__filters">
        <slot name="filters" />
      </div>
      <div class="document-queue-list__header-actions">
        <el-button link title="刷新列表" @click="emit('refresh')">
          <el-icon><Refresh /></el-icon>
        </el-button>
        <el-button v-if="showCreate" type="primary" size="small" @click="emit('create')">
          新增
        </el-button>
      </div>
    </header>

    <!-- ==================== 主体 ==================== -->
    <div class="document-queue-list__body">
      <!-- 加载状态 -->
      <div v-if="loading" class="document-queue-list__status">
        <el-icon class="is-loading"><Loading /></el-icon>
        <span>加载中...</span>
      </div>

      <!-- 错误状态 -->
      <div
        v-else-if="hasError"
        class="document-queue-list__status document-queue-list__status--error"
      >
        <el-result icon="error" :title="errorMessage">
          <template #extra>
            <el-button type="primary" @click="emit('retry')">重试</el-button>
          </template>
        </el-result>
      </div>

      <!-- 空状态 -->
      <div v-else-if="isEmpty" class="document-queue-list__status">
        <slot name="empty">
          <el-empty :description="emptyText" />
        </slot>
      </div>

      <!-- 单据列表 -->
      <div
        v-else
        ref="listRef"
        class="document-queue-list__list"
        role="listbox"
        :aria-label="'单据列表'"
        @keydown="handleListKeydown"
      >
        <div
          v-for="(order, index) in orders"
          :key="resolveRowKey(order)"
          :ref="(el: unknown) => setItemRef(el as HTMLElement | null, index)"
          class="document-queue-list__item"
          :class="{ 'is-active': isActive(order) }"
          role="option"
          :aria-selected="isActive(order)"
          :tabindex="isActive(order) ? 0 : -1"
          @click="handleSelect(order)"
        >
          <slot name="item" :order="order" :active="isActive(order)" :index="index">
            <!-- ===== 默认卡片渲染 ===== -->
            <div class="document-queue-list__item-card">
              <!-- 顶行：主标识 + 状态标签 -->
              <div class="document-queue-list__item-header">
                <span class="document-queue-list__doc-no">
                  {{ resolveField(order, primaryFieldKey) }}
                </span>
                <StatusDisplay
                  v-if="
                    statusMap && statusMap.length > 0 && resolveField(order, statusFieldKey) != null
                  "
                  :value="resolveField(order, statusFieldKey)"
                  :options="statusMap"
                  size="small"
                  effect="plain"
                />
              </div>

              <!-- 底行：其他已配置字段 -->
              <div v-if="displayFields.length > 0" class="document-queue-list__item-body">
                <template v-for="field in displayFields" :key="field.key">
                  <span v-if="hasFieldValue(order, field)" class="document-queue-list__field">
                    <span class="document-queue-list__field-label">{{ field.label }}</span>
                    <span class="document-queue-list__field-separator">：</span>
                    <span class="document-queue-list__field-value">{{
                      resolveField(order, field.key)
                    }}</span>
                  </span>
                </template>
              </div>
            </div>
          </slot>

          <!-- 每个条目的额外内容 -->
          <div v-if="$slots.itemExtra" class="document-queue-list__item-extra">
            <slot name="itemExtra" :order="order" :active="isActive(order)" :index="index" />
          </div>
        </div>
      </div>
    </div>

    <!-- ==================== 底部 ==================== -->
    <footer v-if="$slots.footer" class="document-queue-list__footer">
      <slot name="footer" :count="orders?.length ?? 0" />
    </footer>
  </div>
</template>

<script setup lang="ts" generic="T extends Record<string, any> = Record<string, any>">
import { computed, ref } from 'vue'
import { Refresh, Loading } from '@element-plus/icons-vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import type { StatusDisplayOption } from '@/components/common/type'

// ==================== 类型定义 ====================

/** 卡片主体渲染的单字段描述符 */
export interface QueueField<T = any> {
  /** 字段标识符；当未提供 resolve 时用于从单据解析值 */
  key: string
  /** 显示标签 */
  label: string
  /** 自定义解析器，默认取 order[key] */
  resolve?: (order: T) => any
}

// ==================== 属性 ====================

const props = withDefaults(
  defineProps<{
    /** 单据数组 */
    orders?: T[] | null
    /**
     * 稳定行键。
     * - string：单据上的属性名（如 'id'）
     * - function：(order) => 唯一键
     */
    rowKey: string | ((order: T) => string | number)
    /** 当前选中键（受控） */
    selectedKey?: string | number | null
    /**
     * 字段配置。
     * 第一个字段（索引 0）作为卡片头部主标识渲染；
     * 其余字段在卡片主体中以键值对行展示。
     * 值为 null / undefined / '' 的字段自动隐藏，不显示空标签。
     */
    fields?: QueueField<T>[]
    /**
     * StatusDisplay 组件的状态映射。
     * 默认以 key 为 `'status'` 的字段解析后匹配此映射。
     * 传入 `[]` 或不传则完全隐藏状态标签。
     */
    statusMap?: StatusDisplayOption[] | null
    /** 驱动状态标签的字段键，默认 'status' */
    statusFieldKey?: string
    /** 列表是否处于加载状态 */
    loading?: boolean
    /**
     * 错误状态。
     * - `true` 或非空字符串 → 显示错误面板
     * - 字符串值将作为错误标题
     */
    error?: boolean | string
    /** 空状态描述文本 */
    emptyText?: string
    /** 是否在头部显示"新增"按钮 */
    showCreate?: boolean
  }>(),
  {
    orders: () => [],
    selectedKey: null,
    fields: () =>
      [
        { key: 'docNo', label: '单据号' },
        { key: 'date', label: '日期' },
        { key: 'handler', label: '经办人' },
        { key: 'counterparty', label: '往来单位' }
      ] as QueueField<T>[],
    statusMap: null,
    statusFieldKey: 'status',
    loading: false,
    error: false,
    emptyText: '暂无单据',
    showCreate: false
  }
)

// ==================== 事件 ====================

const emit = defineEmits<{
  'update:selectedKey': [key: string | number]
  select: [order: T]
  create: []
  refresh: []
  retry: []
  /** 通用操作事件；参数：(actionName, order) */
  action: [action: string, order: T]
}>()

// ==================== 插槽 ====================

defineSlots<{
  filters?: () => unknown
  item?: (props: { order: T; active: boolean; index: number }) => unknown
  itemExtra?: (props: { order: T; active: boolean; index: number }) => unknown
  empty?: () => unknown
  footer?: (props: { count: number }) => unknown
}>()

// ==================== 响应式引用 ====================

const listRef = ref<HTMLElement | null>(null)
const itemRefs = ref<(HTMLElement | null)[]>([])

function setItemRef(el: HTMLElement | null, index: number) {
  itemRefs.value[index] = el
}

// ==================== 计算属性 ====================

const hasError = computed(() => {
  if (typeof props.error === 'string') return props.error.length > 0
  return !!props.error
})

const errorMessage = computed(() => {
  if (typeof props.error === 'string') return props.error
  return '加载失败'
})

const isEmpty = computed(() => !props.orders || props.orders.length === 0)

/** The first field — used as the card header's primary identifier */
const primaryFieldKey = computed(() => props.fields[0]?.key ?? 'docNo')

/** Remaining fields displayed in the card body (exclude the primary & status field keys) */
const displayFields = computed(() =>
  props.fields.slice(1).filter((f) => f.key !== props.statusFieldKey)
)

// ==================== 方法 ====================

function resolveRowKey(order: T): string | number {
  if (typeof props.rowKey === 'function') return props.rowKey(order)
  return order[props.rowKey] as string | number
}

function isActive(order: T): boolean {
  return resolveRowKey(order) === props.selectedKey
}

/** Resolve a field value from an order */
function resolveField(order: T, key: string): any {
  const field = props.fields.find((f) => f.key === key)
  if (field?.resolve) return field.resolve(order)
  return order[key]
}

/** Whether a field has a displayable value */
function hasFieldValue(order: T, field: QueueField<T>): boolean {
  const value = resolveField(order, field.key)
  return value != null && value !== ''
}

function handleSelect(order: T) {
  const key = resolveRowKey(order)
  if (key === props.selectedKey) return
  emit('update:selectedKey', key)
  emit('select', order)
}

// ==================== 键盘导航 ====================

/** Current orders array for index lookup (avoid computed dependency issues) */
function getCurrentOrders(): T[] {
  return props.orders ?? []
}

/** Navigate selection by relative offset. Clamps to valid range. */
function navigateSelection(delta: 1 | -1) {
  const orders = getCurrentOrders()
  if (orders.length === 0) return

  const currentIndex = orders.findIndex((o) => resolveRowKey(o) === props.selectedKey)
  const nextIndex =
    currentIndex === -1 ? 0 : Math.max(0, Math.min(orders.length - 1, currentIndex + delta))

  const nextOrder = orders[nextIndex]
  if (!nextOrder) return

  const key = resolveRowKey(nextOrder)
  emit('update:selectedKey', key)
  emit('select', nextOrder)

  // Focus the newly selected item after render
  requestAnimationFrame(() => {
    itemRefs.value[nextIndex]?.focus()
  })
}

function handleListKeydown(e: KeyboardEvent) {
  // 仅处理导航键
  if (!['ArrowUp', 'ArrowDown', 'Enter', ' '].includes(e.key)) return

  // 焦点在输入框/文本域/下拉框内时不拦截
  const target = e.target as HTMLElement
  if (['INPUT', 'TEXTAREA', 'SELECT'].includes(target.tagName)) return

  e.preventDefault()

  switch (e.key) {
    case 'ArrowUp':
      navigateSelection(-1)
      break
    case 'ArrowDown':
      navigateSelection(1)
      break
    case 'Enter':
    case ' ': {
      const orders = getCurrentOrders()
      const current = orders.find((o) => resolveRowKey(o) === props.selectedKey)
      if (current) {
        emit('select', current)
      }
      break
    }
  }
}
</script>

<style scoped>
/* ==================== Root ==================== */
.document-queue-list {
  display: flex;
  flex-direction: column;
  height: 100%;
  min-height: 0;
}

/* ==================== Header ==================== */
.document-queue-list__header {
  display: flex;
  flex-shrink: 0;
  align-items: flex-start;
  gap: 8px;
  padding: 10px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.document-queue-list__filters {
  flex: 1;
  min-width: 0;
}

.document-queue-list__header-actions {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  gap: 6px;
}

/* ==================== Body ==================== */
.document-queue-list__body {
  flex: 1;
  min-height: 0;
  overflow: hidden auto;
}

/* ---------- Status overlay (loading / error / empty) ---------- */
.document-queue-list__status {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  min-height: 200px;
  padding: 32px 16px;
  color: var(--el-text-color-secondary);
  font-size: 14px;
  gap: 8px;
}

.document-queue-list__status--error {
  min-height: 240px;
}

/* ==================== List ==================== */
.document-queue-list__list {
  display: flex;
  flex-direction: column;
  gap: 4px;
  padding: 6px 8px;
}

/* ==================== Item ==================== */
.document-queue-list__item {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: 8px;
  padding: 10px 12px;
  border: 1px solid transparent;
  border-radius: var(--el-border-radius-base, 4px);
  background: var(--el-bg-color, #fff);
  cursor: pointer;
  outline: none;
  transition:
    border-color 0.2s,
    background-color 0.2s,
    box-shadow 0.2s;
}

.document-queue-list__item:hover {
  border-color: var(--el-color-primary-light-7);
  background: var(--el-color-primary-light-9);
}

.document-queue-list__item.is-active {
  border-color: var(--el-color-primary-light-5);
  background: var(--el-color-primary-light-9);
  box-shadow: 0 0 0 1px var(--el-color-primary-light-5) inset;
}

.document-queue-list__item:focus-visible {
  outline: 2px solid var(--el-color-primary);
  outline-offset: 1px;
}

/* ==================== Item Card (default slot) ==================== */
.document-queue-list__item-card {
  flex: 1;
  min-width: 0;
}

.document-queue-list__item-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  margin-bottom: 4px;
}

.document-queue-list__doc-no {
  overflow: hidden;
  font-size: 14px;
  font-weight: 600;
  color: var(--el-text-color-primary);
  text-overflow: ellipsis;
  white-space: nowrap;
}

.document-queue-list__item-body {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  gap: 12px;
  font-size: 12px;
  color: var(--el-text-color-secondary);
}

.document-queue-list__field {
  display: inline-flex;
  align-items: baseline;
  min-width: 0;
  max-width: 100%;
}

.document-queue-list__field-label {
  flex-shrink: 0;
  color: var(--el-text-color-placeholder);
}

.document-queue-list__field-separator {
  flex-shrink: 0;
  color: var(--el-text-color-placeholder);
}

.document-queue-list__field-value {
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

/* ==================== Item Extra ==================== */
.document-queue-list__item-extra {
  flex-shrink: 0;
}

/* ==================== Footer ==================== */
.document-queue-list__footer {
  flex-shrink: 0;
  padding: 8px 12px;
  border-top: 1px solid var(--el-border-color-lighter);
  font-size: 12px;
  color: var(--el-text-color-placeholder);
}
</style>
