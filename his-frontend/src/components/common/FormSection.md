# FormSection

## 组件定位

`FormSection` 是表单/表格分区容器，用于统一筛选区和内容区的上下结构。

它只负责分区布局、最小高度和 loading，不定义具体表单字段或表格列。

## 适用场景

- 详情页里的一个明细分区
- 表单上方有筛选，下面有表格
- 右侧详情区域中需要局部滚动的内容块

## 基本用法

```vue
<template>
  <FormSection :min-table-height="280">
    <template #filter>
      <el-input v-model="keyword" placeholder="筛选明细" />
    </template>

    <template #table>
      <el-table :data="rows" height="100%">
        <el-table-column prop="name" label="名称" />
      </el-table>
    </template>
  </FormSection>
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `loading` | `boolean` | `false` | 内容区加载状态 |
| `minTableHeight` | `string \| number` | `240` | 内容区最小高度 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `filter` | 上方筛选或工具区 |
| `table` | 下方主体内容 |
| `default` | 未提供 `table` 插槽时作为主体内容 |

## 职责边界

不要在组件内写固定字段、校验规则或业务查询。页面负责传入筛选区和主体内容。
