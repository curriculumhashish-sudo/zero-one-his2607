# DataTablePanel

## 组件定位

`DataTablePanel` 是表格外壳组件，用于统一表格容器、loading、序号列、选择列、空状态和分页区域。

它不定义任何业务列，所有 `el-table-column` 都由页面通过默认插槽传入。

## 适用场景

- 查询结果列表
- 配置项列表
- 明细数据表格
- 需要统一表格外观和分页的页面

## 基本用法

```vue
<template>
  <DataTablePanel
    v-model:page="page"
    v-model:page-size="pageSize"
    :data="tableData"
    :total="total"
    show-index
    selectable
    @selectable="selection = $event"
  >
    <el-table-column prop="name" label="姓名" />
    <el-table-column prop="department" label="科室" />
  </DataTablePanel>
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `data` | `any[]` | `[]` | 表格数据 |
| `loading` | `boolean` | `false` | 表格加载状态 |
| `total` | `number` | `0` | 总条数，大于 0 时显示分页 |
| `page` | `number` | `1` | 当前页码 |
| `pageSize` | `number` | `10` | 每页条数 |
| `minHeight` | `string` | `'400px'` | 容器最小高度 |
| `selectable` | `boolean` | `false` | 是否显示选择列 |
| `showIndex` | `boolean` | `false` | 是否显示序号列 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:page` | `page` | 页码变化 |
| `update:pageSize` | `pageSize` | 每页条数变化 |
| `selectable` | `selection` | 表格选择变化 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `default` | 传入业务表格列 |

## 对外暴露

| 名称 | 说明 |
| --- | --- |
| `tableRef` | Element Plus `el-table` 实例 |

## 职责边界

不要把患者列、药品列、操作接口、页面类型判断写进组件。业务列和行操作应由页面声明。
