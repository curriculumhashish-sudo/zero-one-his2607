# StatusDisplay

## 组件定位

`StatusDisplay` 是统一状态展示组件，用于把状态值映射为稳定的文案、颜色、标签样式和说明。

状态字典由页面传入，组件不写死业务状态码。

## 适用场景

- 表格状态列
- 详情摘要状态字段
- 工具栏或侧栏中的状态标识

## 基本用法

```vue
<script setup lang="ts">
import type { StatusDisplayOption } from '@/components/common/type'

const statusOptions: StatusDisplayOption[] = [
  { value: 'pending', label: '待处理', type: 'warning', description: '等待处理' },
  { value: 'done', label: '已完成', type: 'success', description: '已闭环' }
]
</script>

<template>
  <StatusDisplay value="done" :options="statusOptions" show-description />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `value` | `string \| number \| null` | `null` | 当前状态值 |
| `options` | `StatusDisplayOption[]` | `[]` | 状态映射 |
| `fallbackLabel` | `string` | `'未知状态'` | 未匹配状态文案 |
| `size` | `'large' \| 'default' \| 'small'` | `'small'` | 标签尺寸 |
| `effect` | `'dark' \| 'light' \| 'plain'` | `'plain'` | 默认标签风格 |
| `type` | Element Plus Tag 类型 | `'info'` | 默认标签类型 |
| `round` | `boolean` | `false` | 是否圆角标签 |
| `showDescription` | `boolean` | `false` | 是否显示说明 |

## StatusDisplayOption

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `value` | `string \| number` | 状态值 |
| `label` | `string` | 展示文案 |
| `type` | Element Plus Tag 类型 | 标签颜色 |
| `effect` | `'dark' \| 'light' \| 'plain'` | 标签风格 |
| `description` | `string` | 附加说明 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `default` | 自定义展示内容，作用域参数：`option` |

## 职责边界

不要在组件里写审批流转规则、固定后端枚举或接口逻辑。页面负责提供状态字典。
