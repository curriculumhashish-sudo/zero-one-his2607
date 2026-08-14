# PaginationBar

## 组件定位

`PaginationBar` 是纯受控分页栏，用于统一页码、每页条数、总数、跳页和窄区域分页布局。

组件内部不请求数据，也不保存业务分页状态。

## 适用场景

- 主表分页
- 侧栏列表分页
- 不需要完整 `el-pagination` 外观的轻量分页区域

## 基本用法

```vue
<template>
  <PaginationBar
    v-model:page="page"
    v-model:page-size="pageSize"
    :total="total"
    :page-sizes="[10, 20, 50]"
    @change="loadList"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `page` | `number` | 必填 | 当前页码，从 1 开始 |
| `pageSize` | `number` | 必填 | 每页条数 |
| `total` | `number` | 必填 | 总条数 |
| `pageSizes` | `number[]` | `[10, 20, 50, 100]` | 每页条数选项 |
| `layout` | `LayoutItem[]` | `['total', 'sizes', 'prev', 'pager', 'next', 'jumper']` | 布局项 |
| `disabled` | `boolean` | `false` | 是否禁用 |
| `small` | `boolean` | `false` | 小尺寸模式 |
| `pagerCount` | `number` | `7` | 页码按钮数量 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:page` | `page` | 页码变化 |
| `update:pageSize` | `pageSize` | 每页条数变化 |
| `change` | `{ page, pageSize }` | 任一分页状态变化 |

## 职责边界

不要在组件内请求列表、保存查询条件或绑定业务状态。页面收到分页事件后自行加载数据。
