<script setup lang="ts">
import { computed } from 'vue'

/** 布局项：控制分页栏各区块是否显示、以及显示顺序 */
type LayoutItem = 'total' | 'sizes' | 'prev' | 'pager' | 'next' | 'jumper'

/**
 * PaginationBar —— 纯受控分页栏
 * 所有分页状态由父组件持有并通过 props 传入，变更通过事件传出，
 * 组件内部不请求数据、不保存任何业务状态。
 */
const props = withDefaults(
  defineProps<{
    page: number // 当前页码，从 1 开始
    pageSize: number // 当前每页条数
    total: number // 数据总条数
    pageSizes?: number[] // 每页条数下拉可选项
    layout?: LayoutItem[] // 各区块显示与顺序
    disabled?: boolean // 整体禁用（加载中常用）
    small?: boolean // 精简小尺寸模式，适合侧栏等窄区域
    /** 当前页附近展示的页码按钮数量（超出用省略号折叠） */
    pagerCount?: number
  }>(),
  {
    pageSizes: () => [10, 20, 50, 100],
    layout: () => ['total', 'sizes', 'prev', 'pager', 'next', 'jumper'],
    disabled: false,
    small: false,
    pagerCount: 7,
  },
)

const emit = defineEmits<{
  'update:page': [page: number] // 配合 v-model:page
  'update:pageSize': [pageSize: number] // 配合 v-model:page-size
  change: [payload: { page: number; pageSize: number }] // 页码或每页条数变化时统一触发
}>()

// 总页数：至少 1 页（total 为 0 时也保持一致显示）
const pageCount = computed(() => Math.max(1, Math.ceil(props.total / Math.max(1, props.pageSize))))

// 判断某个布局区块是否需要渲染
const has = (item: LayoutItem) => props.layout.includes(item)

// 把目标页码收敛到 [1, pageCount] 合法区间
function clampPage(page: number) {
  return Math.min(Math.max(1, page), pageCount.value)
}

// 跳转到指定页；禁用时或目标就是当前页时不触发任何事件（保证事件只触发一次）
function goToPage(target: number) {
  if (props.disabled) return
  const next = clampPage(target)
  if (next === props.page) return
  emit('update:page', next)
  emit('change', { page: next, pageSize: props.pageSize })
}

// 每页条数变化：按新条数重算页数，并把当前页回落到合法范围，避免越界
function onSizeChange(event: Event) {
  if (props.disabled) return
  const nextSize = Number((event.target as HTMLSelectElement).value)
  if (nextSize === props.pageSize) return
  // Keep the current page within the legal range for the new size.
  const nextPageCount = Math.max(1, Math.ceil(props.total / Math.max(1, nextSize)))
  const nextPage = Math.min(Math.max(1, props.page), nextPageCount)
  emit('update:pageSize', nextSize)
  if (nextPage !== props.page) emit('update:page', nextPage)
  emit('change', { page: nextPage, pageSize: nextSize })
}

// 跳页输入框：回车/失焦时取整跳转，非法值忽略，最后把输入框值复位为当前页
function onJump(event: Event) {
  const input = event.target as HTMLInputElement
  const value = Number(input.value)
  if (Number.isFinite(value) && value >= 1) {
    goToPage(Math.floor(value))
  }
  input.value = String(props.page)
}

/**
 * 计算可见页码序列，`-1` 作为省略号占位。
 * 页数不超过按钮上限时全部展示；否则首尾固定 + 当前页居中，两侧用省略号折叠。
 */
const pagers = computed<number[]>(() => {
  const count = pageCount.value
  const current = clampPage(props.page)
  const maxButtons = Math.max(5, props.pagerCount) // 至少 5 个按钮才能容纳「首 … 中 … 尾」

  // 页数较少：直接顺序展开 1..count
  if (count <= maxButtons) {
    return Array.from({ length: count }, (_, i) => i + 1)
  }

  const side = Math.floor((maxButtons - 3) / 2) // 当前页两侧各展示的数量
  const result: number[] = [1] // 首页固定

  // 以当前页为中心取窗口，并在贴近首/尾时修正窗口边界
  let start = current - side
  let end = current + side
  if (start <= 2) {
    start = 2
    end = start + (maxButtons - 3)
  }
  if (end >= count - 1) {
    end = count - 1
    start = end - (maxButtons - 3)
  }

  if (start > 2) result.push(-1) // 左侧省略号
  for (let i = start; i <= end; i++) result.push(i)
  if (end < count - 1) result.push(-1) // 右侧省略号
  result.push(count) // 尾页固定

  return result
})

// 是否处于首页 / 末页（用于禁用上一页 / 下一页按钮）
const atFirst = computed(() => props.disabled || props.page <= 1)
const atLast = computed(() => props.disabled || props.page >= pageCount.value)
</script>

<template>
  <div class="pagination-bar" :class="{ 'is-small': small, 'is-disabled': disabled }">
    <!-- 总数文案 -->
    <span v-if="has('total')" class="pagination-bar__total">共 {{ total }} 条</span>

    <!-- 每页条数下拉 -->
    <select
      v-if="has('sizes')"
      class="pagination-bar__sizes"
      :value="pageSize"
      :disabled="disabled"
      aria-label="每页条数"
      @change="onSizeChange"
    >
      <option v-for="size in pageSizes" :key="size" :value="size">{{ size }} 条/页</option>
    </select>

    <!-- 上一页 -->
    <button
      v-if="has('prev')"
      type="button"
      class="pagination-bar__btn"
      :disabled="atFirst"
      aria-label="上一页"
      @click="goToPage(page - 1)"
    >
      ‹
    </button>

    <!-- 页码按钮区（item 为 -1 时渲染为省略号，不可点击） -->
    <template v-if="has('pager')">
      <button
        v-for="(item, index) in pagers"
        :key="item === -1 ? `gap-${index}` : item"
        type="button"
        class="pagination-bar__btn"
        :class="{
          'is-active': item === page,
          'pagination-bar__ellipsis': item === -1,
        }"
        :disabled="disabled || item === -1"
        :aria-current="item === page ? 'page' : undefined"
        @click="item !== -1 && goToPage(item)"
      >
        {{ item === -1 ? '…' : item }}
      </button>
    </template>

    <!-- 下一页 -->
    <button
      v-if="has('next')"
      type="button"
      class="pagination-bar__btn"
      :disabled="atLast"
      aria-label="下一页"
      @click="goToPage(page + 1)"
    >
      ›
    </button>

    <!-- 跳页输入框 -->
    <span v-if="has('jumper')" class="pagination-bar__jumper">
      前往
      <input
        class="pagination-bar__jumper-input"
        type="number"
        min="1"
        :max="pageCount"
        :value="page"
        :disabled="disabled"
        aria-label="跳转到指定页"
        @change="onJump"
      />
      页
    </span>
  </div>
</template>

<style scoped>
.pagination-bar {
  display: flex;
  align-items: center;
  flex-wrap: wrap;
  gap: 8px;
  font-size: 14px;
  color: #606266;
}

/* 精简模式：更小的间距与字号 */
.pagination-bar.is-small {
  gap: 4px;
  font-size: 12px;
}

/* 禁用态整体置灰 */
.pagination-bar.is-disabled {
  color: #c0c4cc;
}

.pagination-bar__total {
  white-space: nowrap;
}

.pagination-bar__sizes {
  height: 28px;
  padding: 0 6px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  background: #fff;
  color: inherit;
  cursor: pointer;
}

.pagination-bar.is-small .pagination-bar__sizes {
  height: 24px;
}

.pagination-bar__sizes:disabled {
  cursor: not-allowed;
  background: #f5f7fa;
}

/* 页码 / 翻页按钮基础样式 */
.pagination-bar__btn {
  min-width: 28px;
  height: 28px;
  padding: 0 6px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  background: #fff;
  color: inherit;
  cursor: pointer;
  line-height: 1;
}

.pagination-bar.is-small .pagination-bar__btn {
  min-width: 24px;
  height: 24px;
}

/* 悬停高亮（排除禁用与当前页） */
.pagination-bar__btn:hover:not(:disabled):not(.is-active) {
  color: #409eff;
  border-color: #c6e2ff;
}

/* 当前页高亮 */
.pagination-bar__btn.is-active {
  color: #fff;
  background: #409eff;
  border-color: #409eff;
  cursor: default;
}

.pagination-bar__btn:disabled {
  cursor: not-allowed;
  color: #c0c4cc;
  background: #f5f7fa;
}

/* 省略号占位：无边框、无背景、不可点击 */
.pagination-bar__ellipsis {
  border-color: transparent;
  background: transparent;
  cursor: default;
}

.pagination-bar__jumper {
  display: inline-flex;
  align-items: center;
  gap: 4px;
  white-space: nowrap;
}

.pagination-bar__jumper-input {
  width: 48px;
  height: 28px;
  padding: 0 4px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  text-align: center;
  color: inherit;
}

.pagination-bar.is-small .pagination-bar__jumper-input {
  height: 24px;
  width: 40px;
}

.pagination-bar__jumper-input:disabled {
  cursor: not-allowed;
  background: #f5f7fa;
}
</style>
