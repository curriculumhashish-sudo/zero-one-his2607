<template>
  <div class="description-list" :class="sizeClass">
    <el-descriptions
      :column="column"
      :border="border"
      :direction="direction"
      :label-width="labelWidth"
      :size="size"
      :title="title"
      class="description-list__inner"
    >
      <el-descriptions-item
        v-for="item in visibleItems"
        :key="item.prop"
        :span="resolveSpan(item)"
        :label="item.label"
        :label-class-name="item.labelClassName"
        :class-name="item.className"
        :width="item.width"
        :min-width="item.minWidth"
        :align="item.align || 'left'"
        :label-align="item.labelAlign || 'right'"
      >
        <!-- 
          插槽优先级：
          1. value-{prop} 自定义整个值区域
          2. label-{prop} 自定义标签（值保留默认渲染）
          3. 默认渲染
        -->
        <slot
          :name="`value-${item.prop}`"
          :value="getValue(item)"
          :raw="rawValue(item)"
          :item="item"
        >
          <span class="description-list__value">
            {{ formattedValue(item) }}
          </span>
        </slot>
      </el-descriptions-item>

      <!-- 空状态 -->
      <template v-if="isEmpty">
        <el-descriptions-item :span="column">
          <slot name="empty">
            <span class="description-list__empty">
              {{ emptyText }}
            </span>
          </slot>
        </el-descriptions-item>
      </template>
    </el-descriptions>

    <!-- 额外内容 -->
    <slot name="extra" />
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { ElDescriptions, ElDescriptionsItem } from 'element-plus'

// ---------- 类型定义 ----------
export interface DescriptionItem {
  /** 字段键名，从 data 中取值 */
  prop: string
  /** 显示标签 */
  label: string
  /** 列跨度，默认 1 */
  span?: number | ((data: Record<string, any>) => number)
  /** 是否隐藏，支持 boolean 或函数 */
  hidden?: boolean | ((data: Record<string, any>) => boolean)
  /** 格式化函数 */
  formatter?: (value: any, data: Record<string, any>) => string
  /** 自定义 CSS 类名 */
  className?: string
  /** 标签 CSS 类名 */
  labelClassName?: string
  /** 列宽度 */
  width?: string | number
  /** 列最小宽度 */
  minWidth?: string | number
  /** 内容对齐方式 */
  align?: 'left' | 'center' | 'right'
  /** 标签对齐方式 */
  labelAlign?: 'left' | 'center' | 'right'
}

// ---------- Props ----------
const props = withDefaults(
  defineProps<{
    /** 数据对象 */
    data?: Record<string, any> | null
    /** 字段配置列表 */
    items?: DescriptionItem[]
    /** 列数，默认 2 */
    column?: number
    /** 是否显示边框，默认 true */
    border?: boolean
    /** 布局方向 */
    direction?: 'horizontal' | 'vertical'
    /** 标签宽度，如 '100px' */
    labelWidth?: string
    /** 空值占位符，默认 '-' */
    emptyText?: string
    /** 尺寸 */
    size?: 'large' | 'default' | 'small'
    /** 标题 */
    title?: string
    /** 紧凑模式（减小间距） */
    dense?: boolean
  }>(),
  {
    data: () => ({}),
    items: () => [],
    column: 2,
    border: true,
    direction: 'horizontal',
    labelWidth: '100px',
    emptyText: '-',
    size: 'default',
    title: '',
    dense: false,
  }
)

// ---------- 计算属性 ----------
/** 数据对象（空值保护） */
const safeData = computed(() => props.data || {})

/** 可见的字段列表（过滤 hidden） */
const visibleItems = computed(() => {
  return props.items.filter((item) => {
    if (typeof item.hidden === 'function') {
      return !item.hidden(safeData.value)
    }
    return !item.hidden
  })
})

/** 是否为空（无可见字段或所有字段值为空） */
const isEmpty = computed(() => {
  if (visibleItems.value.length === 0) return true
  return visibleItems.value.every((item) => {
    const val = rawValue(item)
    return val === null || val === undefined || val === ''
  })
})

/** 尺寸对应的 CSS 类 */
const sizeClass = computed(() => {
  const classes: string[] = []
  if (props.dense) classes.push('description-list--dense')
  if (props.size === 'small') classes.push('description-list--small')
  if (props.size === 'large') classes.push('description-list--large')
  return classes.join(' ')
})

// ---------- 方法 ----------
/** 获取原始值 */
function rawValue(item: DescriptionItem): any {
  return safeData.value[item.prop]
}

/** 获取值（空值转为 undefined） */
function getValue(item: DescriptionItem): any {
  const val = rawValue(item)
  if (val === null || val === undefined || val === '') {
    return undefined
  }
  return val
}

/** 格式化值 */
function formattedValue(item: DescriptionItem): string {
  const val = rawValue(item)
  if (val === null || val === undefined || val === '') {
    return props.emptyText
  }
  if (item.formatter) {
    return item.formatter(val, safeData.value)
  }
  return String(val)
}

/** 解析 span（支持数字和函数） */
function resolveSpan(item: DescriptionItem): number {
  if (typeof item.span === 'function') {
    return item.span(safeData.value)
  }
  return item.span || 1
}

// ---------- 暴露 ----------
defineExpose({
  getData: () => safeData.value,
  getVisibleItems: () => visibleItems.value,
  isEmpty: () => isEmpty.value,
})
</script>

<style scoped>
.description-list {
  width: 100%;
}

.description-list__inner {
  width: 100%;
}

/* 紧凑模式 */
.description-list--dense :deep(.el-descriptions__label) {
  padding: 4px 8px !important;
}

.description-list--dense :deep(.el-descriptions__content) {
  padding: 4px 8px !important;
}

.description-list--dense :deep(.el-descriptions__cell) {
  padding: 4px 8px !important;
}

.description-list--dense :deep(.el-descriptions__header) {
  margin-bottom: 8px;
}

/* small 尺寸加强紧凑 */
.description-list--small :deep(.el-descriptions__label) {
  font-size: 12px;
}

.description-list--small :deep(.el-descriptions__content) {
  font-size: 12px;
}

/* large 尺寸 */
.description-list--large :deep(.el-descriptions__label) {
  font-size: 15px;
  padding: 12px 16px !important;
}

.description-list--large :deep(.el-descriptions__content) {
  font-size: 15px;
  padding: 12px 16px !important;
}

/* 空值样式 */
.description-list__empty {
  color: #b0b0b0;
  font-style: italic;
}

/* 值样式 */
.description-list__value {
  word-break: break-all;
}

/* 无边框时的样式调整 */
.description-list :deep(.el-descriptions:not(.is-bordered) .el-descriptions__cell) {
  padding-bottom: 8px;
}

.description-list--dense :deep(.el-descriptions:not(.is-bordered) .el-descriptions__cell) {
  padding-bottom: 4px;
}

/* 垂直模式下的调整 */
.description-list :deep(.el-descriptions--vertical .el-descriptions__cell) {
  padding: 6px 0;
}

.description-list--dense :deep(.el-descriptions--vertical .el-descriptions__cell) {
  padding: 3px 0;
}
</style>