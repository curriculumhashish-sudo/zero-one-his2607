<template>
  <SidePanelShell
    :title="title"
    :width="width"
    :loading="loading"
    :empty="items.length === 0"
    :empty-description="emptyDescription"
    :collapsible="collapsible"
    :collapsed="collapsed"
    @update:collapsed="emit('update:collapsed', $event)"
  >
    <template #title>
      <slot name="title">{{ title }}</slot>
    </template>

    <template v-if="searchable || $slots.search" #search>
      <slot name="search">
        <el-input
          :model-value="keyword"
          :placeholder="searchPlaceholder"
          clearable
          @update:model-value="emit('update:keyword', $event)"
          @keyup.enter="emit('search', keyword)"
          @clear="emit('search', '')"
        />
      </slot>
    </template>

    <div class="record-sidebar__list" role="listbox" :aria-label="title || '记录列表'">
      <button
        v-for="item in items"
        :key="item.key"
        type="button"
        class="record-sidebar__item"
        :class="{ 'is-active': item.key === modelValue }"
        :disabled="item.disabled"
        :aria-selected="item.key === modelValue"
        @click="selectItem(item)"
      >
        <slot name="item" :item="item" :active="item.key === modelValue">
          <span class="record-sidebar__item-main">
            <strong class="record-sidebar__item-title">{{ item.title }}</strong>
            <span v-if="item.subtitle" class="record-sidebar__item-subtitle">
              {{ item.subtitle }}
            </span>
            <span v-if="item.description" class="record-sidebar__item-description">
              {{ item.description }}
            </span>
          </span>
          <slot name="item-extra" :item="item">
            <el-tag v-if="item.status" size="small" effect="plain">{{ item.status }}</el-tag>
          </slot>
        </slot>
      </button>
    </div>

    <template #footer>
      <slot name="footer" :count="items.length">共 {{ items.length }} 条记录</slot>
    </template>
  </SidePanelShell>
</template>

<script setup lang="ts">
import SidePanelShell from './SidePanelShell.vue'
import type { CommonComponentKey, RecordSidebarItem } from './type.ts'

interface RecordSidebarProps {
  modelValue?: CommonComponentKey | null
  items?: RecordSidebarItem[]
  title?: string
  width?: string | number
  loading?: boolean
  emptyDescription?: string
  collapsible?: boolean
  collapsed?: boolean
  searchable?: boolean
  keyword?: string
  searchPlaceholder?: string
}

withDefaults(defineProps<RecordSidebarProps>(), {
  modelValue: null,
  items: () => [],
  title: '记录列表',
  width: 300,
  loading: false,
  emptyDescription: '暂无记录',
  collapsible: false,
  collapsed: false,
  searchable: true,
  keyword: '',
  searchPlaceholder: '搜索记录'
})

const emit = defineEmits<{
  'update:modelValue': [value: CommonComponentKey]
  'update:collapsed': [value: boolean]
  'update:keyword': [value: string]
  select: [item: RecordSidebarItem]
  search: [keyword: string]
}>()

function selectItem(item: RecordSidebarItem) {
  if (item.disabled) return
  emit('update:modelValue', item.key)
  emit('select', item)
}
</script>

<style scoped>
.record-sidebar__list {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.record-sidebar__item {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: 12px;
  width: 100%;
  padding: 10px 12px;
  border: 1px solid transparent;
  border-radius: var(--el-border-radius-base);
  background: transparent;
  color: var(--el-text-color-regular);
  font: inherit;
  text-align: left;
  cursor: pointer;
  transition:
    color 0.2s,
    border-color 0.2s,
    background-color 0.2s;
}

.record-sidebar__item:hover:not(:disabled),
.record-sidebar__item.is-active {
  border-color: var(--el-color-primary-light-7);
  background: var(--el-color-primary-light-9);
  color: var(--el-color-primary);
}

.record-sidebar__item:focus-visible {
  outline: 2px solid var(--el-color-primary-light-5);
  outline-offset: 1px;
}

.record-sidebar__item:disabled {
  color: var(--el-text-color-disabled);
  cursor: not-allowed;
}

.record-sidebar__item-main {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 4px;
}

.record-sidebar__item-title,
.record-sidebar__item-subtitle,
.record-sidebar__item-description {
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.record-sidebar__item-title {
  font-size: 14px;
}

.record-sidebar__item-subtitle {
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.record-sidebar__item-description {
  color: var(--el-text-color-placeholder);
  font-size: 12px;
}
</style>
