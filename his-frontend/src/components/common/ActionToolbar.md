# ActionToolbar

## 组件定位

`ActionToolbar` 是区域级操作栏，用于列表页、详情页或配置页顶部的新增、刷新、导入、导出、批量处理等操作。

它只负责按钮展示、禁用、加载、更多菜单和确认提示，不直接执行接口。

## 适用场景

- 列表页表格上方操作区
- 详情页顶部操作区
- 配置页面的新增、导入、导出区
- 有批量选择数量提示的操作区

## 基本用法

```vue
<script setup lang="ts">
import { Plus, Refresh } from '@element-plus/icons-vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import type { ActionToolbarItem } from '@/components/common/type'

const actions: ActionToolbarItem[] = [
  { key: 'create', label: '新增', type: 'primary', icon: Plus },
  { key: 'refresh', label: '刷新', icon: Refresh },
  { key: 'delete', label: '删除', type: 'danger', confirmText: '确认删除选中数据？' }
]

function handleAction(action: ActionToolbarItem) {
  console.log(action.key)
}
</script>

<template>
  <ActionToolbar
    title="门诊记录"
    description="区域级操作由页面处理"
    :actions="actions"
    :selection-count="2"
    @action="handleAction"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `title` | `string` | `''` | 左侧标题 |
| `description` | `string` | `''` | 左侧说明 |
| `actions` | `ActionToolbarItem[]` | `[]` | 操作按钮配置 |
| `selectionCount` | `number` | `0` | 已选数量，大于 0 时显示 |
| `maxVisible` | `number` | `4` | 直接展示的按钮数，超出进入更多菜单 |
| `loading` | `boolean` | `false` | 整体加载状态 |
| `disabled` | `boolean` | `false` | 整体禁用状态 |
| `compact` | `boolean` | `false` | 紧凑模式 |

## ActionToolbarItem

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `key` | `string \| number` | 操作唯一标识 |
| `label` | `string` | 按钮文案 |
| `type` | Element Plus 按钮类型 | 按钮颜色 |
| `icon` | `Component \| string` | 按钮图标 |
| `disabled` | `boolean` | 单个操作禁用 |
| `loading` | `boolean` | 单个操作加载 |
| `plain` | `boolean` | 朴素按钮 |
| `text` | `boolean` | 文本按钮 |
| `confirmText` | `string` | 点击前确认文案 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `action` | `action` | 点击有效操作后触发 |
| `click` | `key, action` | 同时返回 action key |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `left` | 替换左侧标题和说明区域 |
| `actions` | 完全自定义右侧操作区域 |

## 职责边界

不要在组件内部写保存、删除、审批、导入导出接口。页面收到 `action.key` 后自行决定业务处理。
