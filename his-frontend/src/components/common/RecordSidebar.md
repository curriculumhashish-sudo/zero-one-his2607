# RecordSidebar

## 组件定位

`RecordSidebar` 是记录选择侧栏，用于在主从详情页面中展示可选记录列表。

它组合了 `SidePanelShell`，负责搜索区、列表样式、选中状态和事件输出，不读取固定业务字段。

## 适用场景

- 左侧就诊记录，右侧详情
- 左侧历史单据，右侧单据详情
- 左侧任务记录，右侧处理内容

## 基本用法

```vue
<script setup lang="ts">
import type { RecordSidebarItem } from '@/components/common/type'

const records: RecordSidebarItem[] = [
  { key: 1, title: '2026-07-19 门诊', subtitle: '呼吸内科', status: '诊疗中' }
]
</script>

<template>
  <RecordSidebar
    v-model="selectedRecordKey"
    v-model:keyword="keyword"
    title="就诊记录"
    :items="records"
    collapsible
    @select="handleSelect"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `modelValue` | `string \| number \| null` | `null` | 当前选中记录 key |
| `items` | `RecordSidebarItem[]` | `[]` | 记录列表 |
| `title` | `string` | `'记录列表'` | 侧栏标题 |
| `width` | `string \| number` | `300` | 侧栏宽度 |
| `loading` | `boolean` | `false` | 加载状态 |
| `emptyDescription` | `string` | `'暂无记录'` | 空状态说明 |
| `collapsible` | `boolean` | `false` | 是否可折叠 |
| `collapsed` | `boolean` | `false` | 当前折叠状态 |
| `searchable` | `boolean` | `true` | 是否显示搜索框 |
| `keyword` | `string` | `''` | 搜索关键字 |
| `searchPlaceholder` | `string` | `'搜索记录'` | 搜索框占位 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:modelValue` | `key` | 选中记录变化 |
| `update:collapsed` | `value` | 折叠状态变化 |
| `update:keyword` | `keyword` | 搜索关键字变化 |
| `select` | `item` | 点击记录 |
| `search` | `keyword` | 回车搜索或清空 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `title` | 自定义标题 |
| `search` | 自定义搜索区 |
| `item` | 自定义整条记录 |
| `item-extra` | 自定义记录右侧内容 |
| `footer` | 自定义底部区，作用域参数：`count` |

## 职责边界

不要在组件内写历史查询接口、患者字段、单据字段或详情加载逻辑。
