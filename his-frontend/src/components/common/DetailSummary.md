# DetailSummary

## 组件定位

`DetailSummary` 用于以多列 label-value 形式展示摘要信息，例如患者基础信息、单据头信息、报表基础信息。

字段由页面配置，组件只统一展示结构、空值、加载和空状态。

## 适用场景

- 详情页顶部摘要
- 单据基础信息
- 报表基本信息
- 主从详情右侧的概要区

## 基本用法

```vue
<script setup lang="ts">
import type { DetailSummaryItem } from '@/components/common/type'

const items: DetailSummaryItem[] = [
  { key: 'name', label: '患者姓名', value: '张明' },
  { key: 'status', label: '状态', value: '诊疗中' },
  { key: 'remark', label: '备注', value: '摘要项支持跨列', span: 2 }
]
</script>

<template>
  <DetailSummary title="患者基本信息" :items="items" :column="3" />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `title` | `string` | `''` | 标题 |
| `items` | `DetailSummaryItem[]` | `[]` | 摘要项 |
| `column` | `number` | `4` | 展示列数 |
| `border` | `boolean` | `true` | 是否显示边框 |
| `direction` | `'horizontal' \| 'vertical'` | `'horizontal'` | 描述列表方向 |
| `labelWidth` | `string \| number` | `''` | 标签宽度 |
| `loading` | `boolean` | `false` | 加载状态 |
| `emptyDescription` | `string` | `'暂无摘要信息'` | 空状态文案 |
| `emptyValue` | `string` | `'—'` | 空值占位 |

## DetailSummaryItem

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `key` | `string \| number` | 字段唯一标识 |
| `label` | `string` | 字段名称 |
| `value` | `string \| number \| null` | 字段值 |
| `span` | `number` | 跨列数量 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `title` | 自定义标题 |
| `extra` | 标题右侧内容 |
| `value` | 统一自定义值展示 |
| `value-{key}` | 单个字段自定义值展示 |

## 职责边界

不要在组件里计算金额、拼接业务字段或写死状态枚举。页面负责准备 `items`。
