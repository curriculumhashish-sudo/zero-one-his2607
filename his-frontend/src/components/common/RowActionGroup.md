# RowActionGroup

## 组件定位

`RowActionGroup` 是表格行操作组，用于统一查看、编辑、打印、删除等行内操作的样式、间距、折叠和确认提示。

它不执行路由跳转或删除接口，只把 action 和 row 抛给页面。

## 适用场景

- 表格操作列
- 明细行操作
- 配置项列表的编辑、删除、启停

## 基本用法

```vue
<script setup lang="ts">
import type { RowActionItem } from '@/components/common/type'

const rowActions: RowActionItem[] = [
  { key: 'view', label: '查看', type: 'primary' },
  { key: 'edit', label: '编辑' },
  { key: 'delete', label: '删除', type: 'danger', confirmText: '确认删除？' }
]
</script>

<template>
  <el-table-column label="操作" width="160">
    <template #default="{ row }">
      <RowActionGroup :actions="rowActions" :row="row" @action="handleRowAction" />
    </template>
  </el-table-column>
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `actions` | `RowActionItem[]` | `[]` | 行操作配置 |
| `row` | `unknown` | `undefined` | 当前行数据 |
| `maxVisible` | `number` | `3` | 直接展示的操作数，超出进入更多 |
| `disabled` | `boolean` | `false` | 整体禁用 |

## RowActionItem

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `key` | `string \| number` | 操作唯一标识 |
| `label` | `string` | 操作文案 |
| `type` | Element Plus 按钮类型 | 操作颜色 |
| `disabled` | `boolean` | 是否禁用 |
| `divided` | `boolean` | 下拉菜单中是否分割 |
| `confirmText` | `string` | 点击前确认文案 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `action` | `action, row` | 点击有效操作 |
| `click` | `key, row, action` | 同时返回 action key |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `default` | 完全自定义行操作内容 |

## 职责边界

不要在组件内写删除接口、详情路由或权限计算。权限应由页面计算后体现在 `actions` 配置里。
