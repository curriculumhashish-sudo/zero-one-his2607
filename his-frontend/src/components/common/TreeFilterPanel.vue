<template>
  <div class="tree-filter-panel" :class="{ 'tree-filter-panel--loading': loading }">
    <!-- 头部：搜索框 + 工具栏 -->
    <div v-if="$slots.header || showSearch" class="tree-filter-panel__header">
      <slot name="header">
        <div class="tree-filter-panel__search">
          <el-input
            :model-value="filterText"
            :placeholder="searchPlaceholder"
            :disabled="loading"
            clearable
            size="default"
            @update:model-value="handleFilterChange"
          >
            <template #prefix>
              <el-icon><Search /></el-icon>
            </template>
          </el-input>
          <el-button
            v-if="showClear"
            :disabled="!filterText || loading"
            size="default"
            @click="handleClearFilter"
          >
            清空
          </el-button>
        </div>
      </slot>
    </div>

    <!-- 错误状态 -->
    <div v-if="error" class="tree-filter-panel__error">
      <slot name="error" :error="error" :retry="handleRetry">
        <el-icon class="tree-filter-panel__error-icon"><WarningFilled /></el-icon>
        <span class="tree-filter-panel__error-text">{{ errorMessage }}</span>
        <el-button type="primary" size="small" @click="handleRetry">重试</el-button>
      </slot>
    </div>

    <!-- 树主体 -->
    <el-tree
      v-else
      ref="treeRef"
      :data="safeData"
      :props="treeProps"
      :node-key="nodeKey"
      :default-expanded-keys="resolvedExpandedKeys"
      :default-expand-all="defaultExpandAll"
      :filter-node-method="filterNodeMethod"
      :highlight-current="highlightCurrent"
      :expand-on-click-node="expandOnClickNode"
      :lazy="lazy"
      :load="handleLazyLoad"
      :loading="loading"
      :empty-text="emptyText"
      class="tree-filter-panel__tree"
      @node-click="handleNodeClick"
      @node-expand="handleNodeExpand"
      @node-collapse="handleNodeCollapse"
      @current-change="handleCurrentChange"
    >
      <!-- 自定义节点内容 -->
      <template #default="{ node, data }">
        <slot name="node" :node="node" :data="data">
          <span class="tree-filter-panel__node-label">
            {{ getNodeLabel(data) }}
          </span>
        </slot>
      </template>

      <!-- 空状态 -->
      <template #empty>
        <slot name="empty">
          <div class="tree-filter-panel__empty">
            <el-icon><Document /></el-icon>
            <span>{{ emptyText || '暂无数据' }}</span>
          </div>
        </slot>
      </template>
    </el-tree>

    <!-- 底部 -->
    <div v-if="$slots.footer" class="tree-filter-panel__footer">
      <slot name="footer" />
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, watch, nextTick, onBeforeUnmount } from 'vue'
import {
  ElInput,
  ElButton,
  ElTree,
  ElIcon,
} from 'element-plus'
import { Search, WarningFilled, Document } from '@element-plus/icons-vue'

// ---------- 类型定义 ----------
export interface TreeNodeData {
  [key: string]: any
}

export interface TreeProps {
  label: string
  children: string
  isLeaf?: string
  disabled?: string
}

// ---------- Props ----------
const props = withDefaults(
  defineProps<{
    /** 树数据 */
    data?: TreeNodeData[]
    /** 节点唯一标识字段名 */
    nodeKey: string
    /** 字段映射：label、children、isLeaf、disabled */
    props?: TreeProps
    /** 当前选中节点的 key（受控） */
    currentKey?: string | number | null
    /** 搜索关键词（受控） */
    filterText?: string
    /** 默认展开的节点 keys */
    defaultExpandedKeys?: (string | number)[]
    /** 是否默认展开所有节点 */
    defaultExpandAll?: boolean
    /** 是否高亮当前选中节点 */
    highlightCurrent?: boolean
    /** 点击节点时是否展开/折叠 */
    expandOnClickNode?: boolean
    /** 是否启用懒加载 */
    lazy?: boolean
    /** 懒加载函数，接收 (node, resolve) */
    loadNode?: (node: TreeNodeData, resolve: (data: TreeNodeData[]) => void) => void
    /** 是否加载中 */
    loading?: boolean
    /** 错误对象 */
    error?: Error | null
    /** 空状态文字 */
    emptyText?: string
    /** 搜索占位文字 */
    searchPlaceholder?: string
    /** 是否显示搜索框 */
    showSearch?: boolean
    /** 是否显示清空按钮 */
    showClear?: boolean
    /** 筛选时是否保留祖先层级（展开匹配节点的父级） */
    preserveAncestors?: boolean
  }>(),
  {
    data: () => [],
    nodeKey: 'id',
    props: () => ({ label: 'label', children: 'children' }),
    currentKey: null,
    filterText: '',
    defaultExpandedKeys: () => [],
    defaultExpandAll: false,
    highlightCurrent: true,
    expandOnClickNode: true,
    lazy: false,
    loadNode: undefined,
    loading: false,
    error: null,
    emptyText: '暂无数据',
    searchPlaceholder: '请输入关键词搜索',
    showSearch: true,
    showClear: true,
    preserveAncestors: true,
  }
)

// ---------- Emits ----------
const emit = defineEmits<{
  (e: 'update:currentKey', value: string | number | null): void
  (e: 'update:filterText', value: string): void
  (e: 'select', data: TreeNodeData, node: any): void
  (e: 'nodeExpand', data: TreeNodeData, node: any): void
  (e: 'nodeCollapse', data: TreeNodeData, node: any): void
  (e: 'retry'): void
  (e: 'loadError', error: Error): void
}>()

// ---------- 引用 ----------
const treeRef = ref<InstanceType<typeof ElTree>>()

// ---------- 辅助函数：获取树实例（绕过类型检查） ----------
function getTree() {
  return treeRef.value as any
}

// ---------- 内部状态 ----------
const localFilterText = ref(props.filterText || '')
const localCurrentKey = ref<string | number | null>(props.currentKey)

/** 正在加载的节点集合（防重复触发） */
const loadingNodes = new Set<string>()
/** 组件是否已卸载 */
let isUnmounted = false

// ---------- 计算属性 ----------
/** 安全的树数据 */
const safeData = computed(() => props.data || [])

/** 合并树 props */
const treeProps = computed(() => ({
  label: props.props?.label || 'label',
  children: props.props?.children || 'children',
  isLeaf: props.props?.isLeaf || 'isLeaf',
  disabled: props.props?.disabled || 'disabled',
}))

/** 解析默认展开的 keys */
const resolvedExpandedKeys = computed(() => {
  if (props.defaultExpandAll) return []
  return props.defaultExpandedKeys || []
})

/** 错误信息 */
const errorMessage = computed(() => {
  if (props.error) {
    return props.error.message || '加载失败，请重试'
  }
  return '加载失败，请重试'
})

// ---------- 方法 ----------
/** 获取节点标签 */
function getNodeLabel(data: TreeNodeData): string {
  const labelField = props.props?.label || 'label'
  return data[labelField] || data[props.nodeKey] || ''
}

/** 过滤节点方法（搜索） */
function filterNodeMethod(value: string, data: TreeNodeData): boolean {
  if (!value) return true
  const labelField = props.props?.label || 'label'
  const label = String(data[labelField] || '').toLowerCase()
  const keyword = String(value).toLowerCase()
  return label.includes(keyword)
}

/** 处理搜索关键词变化 */
function handleFilterChange(value: string) {
  localFilterText.value = value
  emit('update:filterText', value)

  const tree = getTree()
  if (tree?.filter) {
    tree.filter(value)
  }

  if (value && props.preserveAncestors) {
    nextTick(() => {
      expandMatchedAncestors(value)
    })
  }
}

/** 展开匹配节点的父级（保留祖先层级） */
function expandMatchedAncestors(keyword: string) {
  const tree = getTree()
  if (!tree) return

  const labelField = props.props?.label || 'label'
  const childrenField = props.props?.children || 'children'
  const matchedKeys: (string | number)[] = []

  function findMatches(nodes: TreeNodeData[], parentKeys: (string | number)[] = []) {
    for (const node of nodes) {
      const label = String(node[labelField] || '').toLowerCase()
      const key = node[props.nodeKey]

      if (label.includes(keyword.toLowerCase())) {
        matchedKeys.push(...parentKeys)
      }

      const children = node[childrenField]
      if (Array.isArray(children) && children.length > 0) {
        findMatches(children, [...parentKeys, key])
      }
    }
  }

  findMatches(safeData.value)

  if (matchedKeys.length > 0) {
    const uniqueKeys = [...new Set(matchedKeys)]
    // 使用 any 调用 expand
    if (tree.expand) {
      tree.expand(uniqueKeys)
    }
  }
}

/** 清空搜索 */
function handleClearFilter() {
  localFilterText.value = ''
  emit('update:filterText', '')
  const tree = getTree()
  if (tree?.filter) {
    tree.filter('')
  }
}

/** 处理节点点击 */
function handleNodeClick(data: TreeNodeData, node: any) {
  emit('select', data, node)

  const key = data[props.nodeKey]
  if (key !== undefined && key !== null) {
    localCurrentKey.value = key
    emit('update:currentKey', key)
  }
}

/** 处理当前节点变化 */
function handleCurrentChange(data: TreeNodeData | null) {
  if (data) {
    const key = data[props.nodeKey]
    if (key !== undefined && key !== null) {
      localCurrentKey.value = key
      emit('update:currentKey', key)
    }
  }
}

/** 处理节点展开 */
function handleNodeExpand(data: TreeNodeData, node: any) {
  emit('nodeExpand', data, node)
}

/** 处理节点折叠 */
function handleNodeCollapse(data: TreeNodeData, node: any) {
  emit('nodeCollapse', data, node)
}

/** 处理懒加载 */
function handleLazyLoad(node: any, resolve: (data: TreeNodeData[]) => void) {
  if (!props.loadNode) {
    resolve([])
    return
  }

  const nodeKey = node.data?.[props.nodeKey]
  if (nodeKey && loadingNodes.has(String(nodeKey))) {
    resolve([])
    return
  }

  if (nodeKey) {
    loadingNodes.add(String(nodeKey))
  }

  try {
    props.loadNode(node.data, (children) => {
      if (!isUnmounted) {
        loadingNodes.delete(String(nodeKey))
        resolve(children || [])
      }
    })
  } catch (error) {
    if (!isUnmounted) {
      loadingNodes.delete(String(nodeKey))
      const err = error instanceof Error ? error : new Error(String(error))
      emit('loadError', err)
      resolve([])
    }
  }
}

/** 处理重试 */
function handleRetry() {
  emit('retry')
  if (localFilterText.value) {
    handleClearFilter()
  }
}

// ---------- 监听器 ----------
watch(
  () => props.filterText,
  (newVal) => {
    if (newVal !== localFilterText.value) {
      localFilterText.value = newVal || ''
      const tree = getTree()
      if (tree?.filter) {
        tree.filter(newVal || '')
      }
      if (newVal && props.preserveAncestors) {
        nextTick(() => {
          expandMatchedAncestors(newVal)
        })
      }
    }
  }
)

watch(
  () => props.currentKey,
  (newVal) => {
    if (newVal !== localCurrentKey.value) {
      localCurrentKey.value = newVal
      const tree = getTree()
      if (tree && newVal !== undefined && newVal !== null) {
        if (tree.setCurrentKey) {
          tree.setCurrentKey(newVal)
        }
      }
    }
  }
)

watch(
  () => props.data,
  () => {
    const tree = getTree()
    if (props.defaultExpandAll) {
      nextTick(() => {
        if (tree?.expandAll) {
          tree.expandAll()
        }
      })
    } else if (props.defaultExpandedKeys && props.defaultExpandedKeys.length > 0) {
      nextTick(() => {
        if (tree?.expand) {
          tree.expand(props.defaultExpandedKeys)
        }
      })
    }
  },
  { deep: false }
)

// ---------- 组件卸载 ----------
onBeforeUnmount(() => {
  isUnmounted = true
  loadingNodes.clear()
})

// ---------- 暴露方法 ----------
defineExpose({
  reload: () => {
    emit('retry')
  },
  /** 展开所有节点 */
  expandAll: () => {
    const tree = getTree()
    if (tree?.expandAll) {
      tree.expandAll()
    }
  },
  /** 折叠所有节点 */
  collapseAll: () => {
    const tree = getTree()
    if (tree?.collapseAll) {
      tree.collapseAll()
    }
  },
  /** 展开指定节点 */
  expand: (keys: (string | number)[]) => {
    const tree = getTree()
    if (tree?.expand) {
      tree.expand(keys)
    }
  },
  /** 折叠指定节点 */
  collapse: (keys: (string | number)[]) => {
    const tree = getTree()
    if (tree?.collapse) {
      tree.collapse(keys)
    }
  },
  /** 设置当前选中节点 */
  setCurrentKey: (key: string | number) => {
    const tree = getTree()
    if (tree?.setCurrentKey) {
      tree.setCurrentKey(key)
    }
  },
  /** 获取当前选中节点 key */
  getCurrentKey: () => {
    const tree = getTree()
    return tree?.getCurrentKey?.() ?? null
  },
  /** 获取当前选中节点数据 */
  getCurrentNode: () => {
    const tree = getTree()
    return tree?.getCurrentNode?.() ?? null
  },
  /** 获取树组件实例 */
  getTreeInstance: () => treeRef.value,
  /** 过滤树（搜索） */
  filter: (text: string) => {
    const tree = getTree()
    if (tree?.filter) {
      tree.filter(text)
    }
  },
})
</script>

<style scoped>
.tree-filter-panel {
  display: flex;
  flex-direction: column;
  height: 100%;
  min-height: 0;
  background: var(--el-bg-color, #ffffff);
  border: 1px solid var(--el-border-color-light, #e5e7eb);
  border-radius: 8px;
  overflow: hidden;
}

.tree-filter-panel--loading {
  opacity: 0.7;
  pointer-events: none;
}

/* 头部 */
.tree-filter-panel__header {
  flex-shrink: 0;
  padding: 12px 16px;
  border-bottom: 1px solid var(--el-border-color-light, #e5e7eb);
}

.tree-filter-panel__search {
  display: flex;
  gap: 8px;
  align-items: center;
}

.tree-filter-panel__search .el-input {
  flex: 1;
}

/* 错误状态 */
.tree-filter-panel__error {
  display: flex;
  flex: 1;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 12px;
  padding: 32px 16px;
  color: var(--el-text-color-secondary, #6b7280);
}

.tree-filter-panel__error-icon {
  font-size: 40px;
  color: var(--el-color-danger, #f56c6c);
}

.tree-filter-panel__error-text {
  font-size: 14px;
  text-align: center;
}

/* 树主体 */
.tree-filter-panel__tree {
  flex: 1;
  padding: 8px 0;
  overflow: auto;
  min-height: 120px;
}

.tree-filter-panel__tree :deep(.el-tree-node__content) {
  height: 32px;
  padding: 0 8px;
  border-radius: 4px;
  transition: background 0.15s;
}

.tree-filter-panel__tree :deep(.el-tree-node__content:hover) {
  background: var(--el-fill-color-light, #f3f4f6);
}

.tree-filter-panel__tree :deep(.el-tree-node.is-current > .el-tree-node__content) {
  background: var(--el-color-primary-light-9, #e6f1ff);
  color: var(--el-color-primary, #409eff);
}

.tree-filter-panel__tree :deep(.el-tree-node.is-current > .el-tree-node__content .el-tree-node__label) {
  font-weight: 600;
}

/* 节点标签 */
.tree-filter-panel__node-label {
  font-size: 13px;
  color: var(--el-text-color-primary, #1f2937);
}

/* 空状态 */
.tree-filter-panel__empty {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 8px;
  padding: 32px 16px;
  color: var(--el-text-color-secondary, #6b7280);
  font-size: 14px;
}

.tree-filter-panel__empty .el-icon {
  font-size: 40px;
}

/* 底部 */
.tree-filter-panel__footer {
  flex-shrink: 0;
  padding: 8px 16px;
  border-top: 1px solid var(--el-border-color-light, #e5e7eb);
}

/* 搜索框适配 */
.tree-filter-panel__search .el-input :deep(.el-input__wrapper) {
  border-radius: 6px;
}

.tree-filter-panel__search .el-button {
  flex-shrink: 0;
}

/* 紧凑模式 - 可通过外部 class 覆盖 */
.tree-filter-panel--compact .tree-filter-panel__header {
  padding: 6px 12px;
}

.tree-filter-panel--compact .tree-filter-panel__tree {
  padding: 4px 0;
}

.tree-filter-panel--compact .tree-filter-panel__tree :deep(.el-tree-node__content) {
  height: 28px;
  font-size: 12px;
}
</style>