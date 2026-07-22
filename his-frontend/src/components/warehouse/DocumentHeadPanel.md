# DocumentHeadPanel

## 组件定位

单据头部、查询区、业务主体和操作区的通用容器。组件不写死入库、出库、盘点或审批流程，底部按钮由页面通过 `actions` 配置。

## 基本用法

```vue
<script setup lang="ts">
import type { DocumentHeadAction } from '@/components/warehouse/DocumentHeadPanel.vue'

const actions: DocumentHeadAction[] = [
  {
    key: 'save',
    label: '保存',
    type: 'primary',
    disabled: ({ status }) => status !== 'draft'
  },
  {
    key: 'approve',
    label: '审核通过',
    type: 'success',
    visible: ({ status }) => status === 'pending'
  }
]
</script>

<template>
  <DocumentHeadPanel
    v-model:status="status"
    title="采购入库单"
    :status-map="statusMap"
    :actions="actions"
    @action="handleAction"
  >
    <template #body>单据表单或明细表格</template>
  </DocumentHeadPanel>
</template>
```

## Props

- `title`：单据标题。
- `status`、`statusMap`：受控状态及展示映射。
- `readonly`、`loading`：禁用操作状态。
- `actions`：`DocumentHeadAction[]`，页面配置的操作按钮。
- `keyword`、`dropdownValue` 及搜索相关属性：传递给 `SearchPanel`。

`DocumentHeadAction` 支持 `key`、`label`、`type`、`plain`、`loading`，以及布尔值或函数形式的 `visible`、`disabled`。判断函数接收 `{ status, readonly, loading }`。

## 事件

- `action(key, item)`：点击页面配置的操作按钮。
- `update:status`、`update:keyword`、`update:dropdownValue`：受控值变化。
- `search`、`reset`：查询操作。

## 插槽

- `title`、`status`、`actions`：头部扩展。
- `searchFields`：查询字段。
- `body`：单据表单或明细主体。
- `auditInfo`、`extra`：底部审核信息和扩展内容。

## 职责边界

组件只负责结构和操作状态。保存、提交、审核、出库、盘点等动作名称、可见性、禁用条件及实际业务调用均由页面决定。
