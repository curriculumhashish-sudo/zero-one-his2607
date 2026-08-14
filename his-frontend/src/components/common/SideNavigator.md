# SideNavigator

## 组件定位

`SideNavigator` 是侧栏分类导航组件，用于树形分类、目录、模板分类等左侧导航场景。

它组合 `SidePanelShell` 和 `el-tree`，负责选中、搜索过滤、折叠和节点数量展示。

## 适用场景

- 左侧病历分类
- 模板分类树
- 科室、库房、执行地点等分类导航

## 基本用法

```vue
<script setup lang="ts">
import type { SideNavigatorNode } from '@/components/common/type'

const nodes: SideNavigatorNode[] = [
  { key: 'outpatient', label: '门诊病历' },
  { key: 'inpatient', label: '住院记录' }
]
</script>

<template>
  <SideNavigator
    v-model="selectedKey"
    v-model:keyword="keyword"
    title="病历分类"
    :items="nodes"
    default-expand-all
    collapsible
    @select="handleSelect"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `modelValue` | `string \| number \| null` | `null` | 当前选中节点 key |
| `items` | `SideNavigatorNode[]` | `[]` | 节点数据 |
| `title` | `string` | `'分类导航'` | 标题 |
| `width` | `string \| number` | `260` | 侧栏宽度 |
| `loading` | `boolean` | `false` | 加载状态 |
| `emptyDescription` | `string` | `'暂无分类'` | 空状态说明 |
| `collapsible` | `boolean` | `false` | 是否可折叠 |
| `collapsed` | `boolean` | `false` | 当前折叠状态 |
| `searchable` | `boolean` | `true` | 是否显示搜索 |
| `keyword` | `string` | `''` | 搜索关键字 |
| `searchPlaceholder` | `string` | `'搜索分类'` | 搜索占位 |
| `defaultExpandAll` | `boolean` | `false` | 默认展开全部 |
| `accordion` | `boolean` | `false` | 是否手风琴模式 |
| `highlightCurrent` | `boolean` | `true` | 是否高亮当前节点 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:modelValue` | `key` | 选中节点变化 |
| `update:collapsed` | `value` | 折叠状态变化 |
| `update:keyword` | `keyword` | 搜索关键字变化 |
| `select` | `node` | 点击节点 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `title` | 自定义标题 |
| `search` | 自定义搜索区 |
| `node` | 自定义树节点 |
| `footer` | 自定义底部区，作用域参数：`count` |

## 职责边界

不要在组件内写固定分类结构、节点增删改接口或页面名称判断。节点数据由页面传入。
