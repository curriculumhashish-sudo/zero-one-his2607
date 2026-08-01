<template>
  <section class="doc-head-panel" :class="{ 'is-readonly': readonly }">
    <!-- ==================== Header：标题 + 状态下拉 + 操作 ==================== -->
    <header class="doc-head-panel__header">
      <slot name="title">
        <div class="doc-head-panel__title">{{ title }}</div>
      </slot>
      <div class="doc-head-panel__header-right">
        <!-- 状态下拉切换 -->
        <el-select
          v-if="statusMap.length > 0"
          :model-value="status"
          :disabled="readonly || loading"
          size="small"
          class="doc-head-panel__status-select"
          @update:model-value="emit('update:status', $event)"
        >
          <el-option
            v-for="opt in statusMap"
            :key="String(opt.value)"
            :label="opt.label"
            :value="opt.value"
          />
        </el-select>
        <slot name="status">
          <StatusDisplay
            v-if="status !== undefined && status !== null"
            :value="status"
            :options="statusMap"
            size="small"
          />
        </slot>
        <slot name="actions" />
      </div>
    </header>

    <!-- ==================== 查询区：集成 SearchPanel ==================== -->
    <div v-if="$slots.searchFields" class="doc-head-panel__search">
      <SearchPanel
        :keyword="keyword"
        :dropdown-value="dropdownValue"
        :keyword-label="keywordLabel"
        :keyword-placeholder="keywordPlaceholder"
        :dropdown-label="dropdownLabel"
        :dropdown-options="dropdownOptions"
        :dropdown-placeholder="dropdownPlaceholder"
        :loading="searchLoading"
        :field-count="searchFieldCount"
        :show-search="showSearch"
        :show-reset="showReset"
        @update:keyword="emit('update:keyword', $event)"
        @update:dropdown-value="emit('update:dropdownValue', $event)"
        @search="emit('search')"
        @reset="emit('reset')"
      >
        <template #header>
          <slot name="searchFields" />
        </template>
      </SearchPanel>
    </div>

    <!-- ==================== Body：表单/表格区 ==================== -->
    <div class="doc-head-panel__body">
      <slot name="body">
        <el-empty description="暂无内容" />
      </slot>
    </div>

    <!-- ==================== Footer：页面配置的操作按钮 ==================== -->
    <footer v-if="visibleActions.length || $slots.auditInfo || $slots.extra" class="doc-head-panel__footer">
      <div class="doc-head-panel__footer-btns">
        <template v-for="btn in visibleActions" :key="btn.key">
          <el-button
            :type="btn.type"
            :plain="btn.plain"
            size="small"
            :loading="btn.loading"
            :disabled="isDisabled(btn)"
            @click="emit('action', btn.key, btn)"
          >
            {{ btn.label }}
          </el-button>
        </template>
        <slot name="auditInfo" />
      </div>
      <slot name="extra" />
    </footer>
  </section>
</template>

<script lang="ts" setup>
import { computed } from 'vue'

import type { CommonComponentKey, StatusDisplayOption } from '@/components/common/type'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'

// ==================== 类型 ====================

export interface DocumentHeadActionContext {
  status: CommonComponentKey | null
  readonly: boolean
  loading: boolean
}

export interface DocumentHeadAction {
  key: string
  label: string
  type?: 'primary' | 'success' | 'warning' | 'danger' | 'info' | 'default'
  plain?: boolean
  loading?: boolean
  visible?: boolean | ((context: DocumentHeadActionContext) => boolean)
  disabled?: boolean | ((context: DocumentHeadActionContext) => boolean)
}

// ==================== Props ====================

const props = withDefaults(
  defineProps<{
    /** 单据标题 */
    title?: string
    /** 单据状态值（支持 v-model:status） */
    status?: CommonComponentKey | null
    /** 状态映射表（下拉选项 + StatusDisplay 共用） */
    statusMap?: StatusDisplayOption[]
    /** 是否只读 */
    readonly?: boolean
    /** 加载状态 */
    loading?: boolean
    /** 底部操作配置；业务状态判断由页面提供 */
    actions?: DocumentHeadAction[]
    /** 搜索关键词 v-model */
    keyword?: string
    /** 关键词字段标签 */
    keywordLabel?: string
    /** 关键词占位文本 */
    keywordPlaceholder?: string
    /** 搜索下拉选项 */
    dropdownOptions?: { label: string; value: string | number }[]
    /** 搜索下拉字段标签 */
    dropdownLabel?: string
    /** 搜索下拉选中值 v-model */
    dropdownValue?: string | number | null
    /** 下拉占位文本 */
    dropdownPlaceholder?: string
    /** 查询按钮 loading */
    searchLoading?: boolean
    /** 是否显示查询按钮 */
    showSearch?: boolean
    /** 是否显示重置按钮 */
    showReset?: boolean
    /** #searchFields 插槽中的字段数量（用于展开/收起判断） */
    searchFieldCount?: number
  }>(),
  {
    title: '单据信息',
    status: null,
    statusMap: () => [],
    readonly: false,
    loading: false,
    actions: () => [],
    keyword: '',
    keywordLabel: '关键词',
    keywordPlaceholder: '请输入关键词',
    dropdownLabel: '类别',
    dropdownOptions: () => [],
    dropdownValue: null,
    dropdownPlaceholder: '请选择',
    showSearch: true,
    showReset: true,
    searchLoading: false,
    searchFieldCount: 0
  }
)

// ==================== Emits ====================

const emit = defineEmits<{
  action: [action: string, item: DocumentHeadAction]
  'update:status': [value: CommonComponentKey]
  'update:keyword': [value: string]
  'update:dropdownValue': [value: string | number | null]
  search: []
  reset: []
}>()

// ==================== Slots ====================

defineSlots<{
  title?: () => unknown
  status?: () => unknown
  actions?: () => unknown
  searchFields?: () => unknown
  body?: () => unknown
  auditInfo?: () => unknown
  extra?: () => unknown
}>()

const actionContext = computed<DocumentHeadActionContext>(() => ({
  status: props.status,
  readonly: props.readonly,
  loading: props.loading
}))

const visibleActions = computed(() =>
  props.actions.filter((item) =>
    typeof item.visible === 'function'
      ? item.visible(actionContext.value)
      : item.visible !== false
  )
)

function isDisabled(item: DocumentHeadAction): boolean {
  if (props.readonly || props.loading || item.loading) return true
  return typeof item.disabled === 'function'
    ? item.disabled(actionContext.value)
    : Boolean(item.disabled)
}
</script>

<style scoped>
/* ==================== 根容器 ==================== */
.doc-head-panel {
  display: flex;
  flex-direction: column;
  gap: 8px;
  padding: 8px 12px;
  border: 1px solid var(--el-border-color-extra-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-fill-color-blank);
}

.doc-head-panel.is-readonly {
  opacity: 0.7;
}

/* ==================== Header ==================== */
.doc-head-panel__header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  flex-shrink: 0;
}

.doc-head-panel__title {
  font-size: 16px;
  font-weight: 600;
  color: var(--el-text-color-primary);
  position: relative;
  padding-left: 10px;
}

.doc-head-panel__title::before {
  position: absolute;
  top: 50%;
  left: 0;
  width: 3px;
  height: 14px;
  background: var(--el-color-primary);
  border-radius: 2px;
  content: '';
  transform: translateY(-50%);
}

.doc-head-panel__header-right {
  display: flex;
  align-items: center;
  gap: 8px;
  flex-shrink: 0;
}

.doc-head-panel__status-select {
  width: 120px;
}

/* ==================== Search 区 ==================== */
.doc-head-panel__search {
  flex-shrink: 0;
}

/* ==================== Body ==================== */
.doc-head-panel__body {
  flex: 1;
  min-height: 0;
}

/* ==================== Footer ==================== */
.doc-head-panel__footer {
  display: flex;
  flex-direction: column;
  gap: 6px;
  flex-shrink: 0;
  padding-top: 8px;
  border-top: 1px dashed var(--el-border-color-extra-light);
}

.doc-head-panel__footer-btns {
  display: flex;
  align-items: center;
  flex-wrap: wrap;
  gap: 8px;
}
</style>
