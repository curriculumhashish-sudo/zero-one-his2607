# TreeFilterPanel

## 组件定位

`TreeFilterPanel` 是带搜索过滤功能的树形选择面板组件，基于 Element Plus `el-tree` 封装。

它支持关键词搜索、懒加载、错误重试、自定义节点渲染、祖先层级保留等功能，适用于左侧导航、分类筛选、组织架构选择等场景。

## 适用场景

- 左侧树形导航菜单
- 分类筛选面板
- 组织架构选择
- 带搜索的树形数据展示
- 懒加载树形数据

## 基本用法

```vue
<script setup lang="ts">
import TreeFilterPanel from '@/components/common/TreeFilterPanel.vue'
import type { TreeNodeData } from '@/components/common/TreeFilterPanel.vue'

const treeData = ref<TreeNodeData[]>([
  {
    id: '1',
    label: '一级分类',
    children: [
      { id: '1-1', label: '二级分类A' },
      { id: '1-2', label: '二级分类B' },
    ],
  },
  { id: '2', label: '一级分类2' },
])

const currentKey = ref<string | number | null>(null)

function handleSelect(data: TreeNodeData, node: any) {
  console.log('选中节点:', data)
}
</script>

<template>
  <TreeFilterPanel
    :data="treeData"
    v-model:current-key="currentKey"
    node-key="id"
    @select="handleSelect"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `data` | `TreeNodeData[]` | `[]` | 树数据 |
| `nodeKey` | `string` | `'id'` | 节点唯一标识字段名 |
| `props` | `TreeProps` | `{ label: 'label', children: 'children' }` | 字段映射配置 |
| `currentKey` | `string \| number \| null` | `null` | 当前选中节点的 key（受控） |
| `filterText` | `string` | `''` | 搜索关键词（受控） |
| `defaultExpandedKeys` | `(string \| number)[]` | `[]` | 默认展开的节点 keys |
| `defaultExpandAll` | `boolean` | `false` | 是否默认展开所有节点 |
| `highlightCurrent` | `boolean` | `true` | 是否高亮当前选中节点 |
| `expandOnClickNode` | `boolean` | `true` | 点击节点时是否展开/折叠 |
| `lazy` | `boolean` | `false` | 是否启用懒加载 |
| `loadNode` | `(node, resolve) => void` | `undefined` | 懒加载函数 |
| `loading` | `boolean` | `false` | 是否加载中 |
| `error` | `Error \| null` | `null` | 错误对象 |
| `emptyText` | `string` | `'暂无数据'` | 空状态文字 |
| `searchPlaceholder` | `string` | `'请输入关键词搜索'` | 搜索占位文字 |
| `showSearch` | `boolean` | `true` | 是否显示搜索框 |
| `showClear` | `boolean` | `true` | 是否显示清空按钮 |
| `preserveAncestors` | `boolean` | `true` | 筛选时是否保留祖先层级 |

## TreeProps

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `label` | `string` | 节点标签字段名 |
| `children` | `string` | 子节点字段名 |
| `isLeaf` | `string` | 是否叶子节点字段名 |
| `disabled` | `string` | 是否禁用字段名 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:currentKey` | `value` | 当前选中节点变化时触发 |
| `update:filterText` | `value` | 搜索关键词变化时触发 |
| `select` | `data, node` | 点击节点时触发 |
| `nodeExpand` | `data, node` | 节点展开时触发 |
| `nodeCollapse` | `data, node` | 节点折叠时触发 |
| `retry` | - | 点击重试按钮时触发 |
| `loadError` | `error` | 懒加载出错时触发 |

## 插槽

| 插槽 | 说明 | 作用域参数 |
| --- | --- | --- |
| `header` | 自定义头部区域 | - |
| `node` | 自定义节点内容 | `node`, `data` |
| `error` | 自定义错误状态 | `error`, `retry` |
| `empty` | 自定义空状态 | - |
| `footer` | 自定义底部区域 | - |

## 暴露方法

| 方法 | 参数 | 返回值 | 说明 |
| --- | --- | --- | --- |
| `reload` | - | `void` | 重新加载（触发 retry 事件） |
| `expandAll` | - | `void` | 展开所有节点 |
| `collapseAll` | - | `void` | 折叠所有节点 |
| `expand` | `keys` | `void` | 展开指定节点 |
| `collapse` | `keys` | `void` | 折叠指定节点 |
| `setCurrentKey` | `key` | `void` | 设置当前选中节点 |
| `getCurrentKey` | - | `string \| number \| null` | 获取当前选中节点 key |
| `getCurrentNode` | - | `TreeNodeData \| null` | 获取当前选中节点数据 |
| `getTreeInstance` | - | `ElTree` | 获取树组件实例 |
| `filter` | `text` | `void` | 过滤树（搜索） |

## 职责边界

不要在组件内写死接口调用或业务数据。页面负责提供 `data` 或 `loadNode` 函数，组件只负责渲染和交互。

对于复杂的节点渲染，使用 `node` 插槽；对于复杂的头部/底部操作，使用 `header` / `footer` 插槽。