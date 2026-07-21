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
          @update:model-value="updateKeyword"
        />
      </slot>
    </template>

    <el-tree
      ref="treeRef"
      class="side-navigator__tree"
      node-key="key"
      :data="items"
      :props="treeProps"
      :current-node-key="modelValue ?? undefined"
      :highlight-current="highlightCurrent"
      :default-expand-all="defaultExpandAll"
      :accordion="accordion"
      :expand-on-click-node="false"
      :filter-node-method="filterNode"
      @node-click="selectNode"
    >
      <template #default="scope">
        <slot name="node" v-bind="scope">
          <span class="side-navigator__node-label">{{ scope.data.label }}</span>
        </slot>
      </template>
    </el-tree>

    <template #footer>
      <slot name="footer" :count="nodeCount">共 {{ nodeCount }} 个分类</slot>
    </template>
  </SidePanelShell>
</template>

<script setup lang="ts">
import { computed, nextTick, ref, watch } from 'vue'
import type { TreeInstance, TreeNodeData } from 'element-plus'
import SidePanelShell from './SidePanelShell.vue'
import type { CommonComponentKey, SideNavigatorNode } from './type.ts'

interface SideNavigatorProps {
  modelValue?: CommonComponentKey | null
  items?: SideNavigatorNode[]
  title?: string
  width?: string | number
  loading?: boolean
  emptyDescription?: string
  collapsible?: boolean
  collapsed?: boolean
  searchable?: boolean
  keyword?: string
  searchPlaceholder?: string
  defaultExpandAll?: boolean
  accordion?: boolean
  highlightCurrent?: boolean
}

const props = withDefaults(defineProps<SideNavigatorProps>(), {
  modelValue: null,
  items: () => [],
  title: '分类导航',
  width: 260,
  loading: false,
  emptyDescription: '暂无分类',
  collapsible: false,
  collapsed: false,
  searchable: true,
  keyword: '',
  searchPlaceholder: '搜索分类',
  defaultExpandAll: false,
  accordion: false,
  highlightCurrent: true
})

const emit = defineEmits<{
  'update:modelValue': [value: CommonComponentKey]
  'update:collapsed': [value: boolean]
  'update:keyword': [value: string]
  select: [node: SideNavigatorNode]
}>()

const treeRef = ref<TreeInstance>()
const treeProps = { label: 'label', children: 'children', disabled: 'disabled' }

const nodeCount = computed(() => {
  const count = (nodes: SideNavigatorNode[]): number =>
    nodes.reduce((total, node) => total + 1 + count(node.children ?? []), 0)
  return count(props.items)
})

function filterNode(value: string, data: TreeNodeData) {
  const label = String(data.label ?? '')
  return !value || label.toLocaleLowerCase().includes(value.trim().toLocaleLowerCase())
}

function updateKeyword(value: string) {
  emit('update:keyword', value)
  treeRef.value?.filter(value)
}

function selectNode(node: SideNavigatorNode) {
  if (node.disabled) return
  emit('update:modelValue', node.key)
  emit('select', node)
}

watch(
  () => props.keyword,
  (value) => treeRef.value?.filter(value),
  { immediate: true, flush: 'post' }
)

watch(
  () => [props.modelValue, props.collapsed, props.items] as const,
  async () => {
    await nextTick()
    treeRef.value?.setCurrentKey(props.modelValue ?? undefined)
  },
  { immediate: true }
)
</script>

<style scoped>
.side-navigator__tree {
  --el-tree-node-hover-bg-color: var(--el-color-primary-light-9);

  background: transparent;
}

.side-navigator__tree :deep(.el-tree-node__content) {
  min-height: 36px;
  border-radius: var(--el-border-radius-base);
}

.side-navigator__tree :deep(.el-tree-node.is-current > .el-tree-node__content) {
  color: var(--el-color-primary);
  font-weight: 600;
}

.side-navigator__node-label {
  min-width: 0;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}
</style>
