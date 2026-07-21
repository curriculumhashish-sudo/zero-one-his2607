# DescriptionList

## 组件定位

`DescriptionList` 是基于 Element Plus `el-descriptions` 的增强描述列表组件，用于展示实体详情信息。

它支持动态字段配置、条件隐藏、自定义格式化、插槽扩展和紧凑模式，自动处理空值显示和列跨度计算。

## 适用场景

- 详情页的字段展示区域
- 表单提交后的确认信息展示
- 数据卡片中的属性列表
- 需要根据条件动态显示字段的场景

## 基本用法

```vue
<script setup lang="ts">
import DescriptionList from '@/components/common/DescriptionList.vue'
import type { DescriptionItem } from '@/components/common/DescriptionList.vue'

const data = ref({
  name: '张三',
  age: 30,
  phone: '13800138000',
  address: '北京市朝阳区',
  status: 'active',
  createdAt: '2024-01-15',
})

const items: DescriptionItem[] = [
  { prop: 'name', label: '姓名' },
  { prop: 'age', label: '年龄' },
  { prop: 'phone', label: '手机号' },
  { prop: 'address', label: '地址', span: 2 },
  { 
    prop: 'status', 
    label: '状态',
    formatter: (val) => ({ active: '活跃', inactive: '停用' }[val] || val)
  },
  { prop: 'createdAt', label: '创建时间' },
]
</script>

<template>
  <DescriptionList :data="data" :items="items" title="用户信息" />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `data` | `Record<string, any> \| null` | `{}` | 数据对象 |
| `items` | `DescriptionItem[]` | `[]` | 字段配置列表 |
| `column` | `number` | `2` | 列数 |
| `border` | `boolean` | `true` | 是否显示边框 |
| `direction` | `'horizontal' \| 'vertical'` | `'horizontal'` | 布局方向 |
| `labelWidth` | `string` | `'100px'` | 标签宽度 |
| `emptyText` | `string` | `'-'` | 空值占位符 |
| `size` | `'large' \| 'default' \| 'small'` | `'default'` | 尺寸 |
| `title` | `string` | `''` | 标题 |
| `dense` | `boolean` | `false` | 紧凑模式（减小间距） |

## DescriptionItem

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `prop` | `string` | 字段键名，从 data 中取值 |
| `label` | `string` | 显示标签 |
| `span` | `number \| ((data) => number)` | 列跨度，默认 1，支持函数动态计算 |
| `hidden` | `boolean \| ((data) => boolean)` | 是否隐藏，支持函数动态判断 |
| `formatter` | `(value, data) => string` | 格式化函数 |
| `className` | `string` | 自定义 CSS 类名 |
| `labelClassName` | `string` | 标签 CSS 类名 |
| `width` | `string \| number` | 列宽度 |
| `minWidth` | `string \| number` | 列最小宽度 |
| `align` | `'left' \| 'center' \| 'right'` | 内容对齐方式，默认 `'left'` |
| `labelAlign` | `'left' \| 'center' \| 'right'` | 标签对齐方式，默认 `'right'` |

## 插槽

| 插槽 | 说明 | 作用域参数 |
| --- | --- | --- |
| `value-{prop}` | 自定义指定字段的值渲染 | `value`, `raw`, `item` |
| `empty` | 自定义空状态内容 | - |
| `extra` | 额外内容区域（在列表下方） | - |

## 暴露方法

| 方法 | 返回值 | 说明 |
| --- | --- | --- |
| `getData` | `Record<string, any>` | 获取当前数据对象 |
| `getVisibleItems` | `DescriptionItem[]` | 获取可见字段列表 |
| `isEmpty` | `boolean` | 判断是否为空（无可见字段或所有字段值为空） |

## 职责边界

不要在组件内写死业务字段配置和格式化逻辑。页面负责提供 `data` 和 `items`，组件只负责渲染和基础处理。

对于复杂的条件隐藏逻辑，使用 `hidden` 函数；对于复杂的值渲染，使用 `value-{prop}` 插槽。