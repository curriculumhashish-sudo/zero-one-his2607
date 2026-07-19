# FormSection（C10）

`FormSection` 是“上方筛选、下方表格”的通用布局组件。上方通过 `filter` 插槽放入筛选组件，下方通过 `table` 插槽放入表格组件。

它只负责两个区域的排列、表格区域滚动和加载状态，不负责定义筛选字段、表格列或接口请求。

组件路径：

```text
src/components/common/FormSection.vue
```

## 1. 页面结构

```text
┌────────────────────────────┐
│ filter 插槽：筛选组件       │
├────────────────────────────┤
│ table 插槽：表格组件        │
│                            │
└────────────────────────────┘
```

## 2. 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import FormSection from '@/components/common/FormSection.vue'

const loading = ref(false)
</script>

<template>
  <FormSection :loading="loading" :min-table-height="360">
    <template #filter>
      <FilterComponent />
    </template>

    <template #table>
      <TableComponent />
    </template>
  </FormSection>
</template>
```

## 3. Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `loading` | `boolean` | `false` | 是否在表格区域显示加载遮罩，不影响筛选区域 |
| `minTableHeight` | `string \| number` | `240` | 表格区域最小高度；数字会自动转换为 `px` |

示例：

```vue
<FormSection min-table-height="50vh" />
<FormSection :min-table-height="400" />
```

## 4. 插槽

| 插槽 | 说明 |
| --- | --- |
| `filter` | 上方筛选区域 |
| `table` | 下方表格区域 |
| `default` | `table` 插槽的简写后备；没有提供 `table` 插槽时使用 |

以下两种写法效果相同：

```vue
<FormSection>
  <template #table>
    <MyTable />
  </template>
</FormSection>
```

```vue
<FormSection>
  <MyTable />
</FormSection>
```

## 5. 与 MySearch、MyTable 组合

```vue
<FormSection :loading="loading" :min-table-height="360">
  <template #filter>
    <MySearch
      :model="searchModel"
      :items="searchItems"
      @model-change="(prop, value) => (searchModel[prop] = value)"
      @do-search="loadTable"
    >
      <template #header>
        <h3>插槽使用示例</h3>
      </template>

      <template #footer>
        <el-text type="info">这里可以放筛选条件说明。</el-text>
      </template>
    </MySearch>
  </template>

  <template #table>
    <MyTable
      tabtitle="操作栏表格"
      :tabdatacolumns="tableColumns"
      :tabdata="tableData"
      :taboperbtns="operationButtons"
      @taboper-click="handleOperation"
    />
  </template>
</FormSection>
```

相关组件导入：

```ts
import FormSection from '@/components/common/FormSection.vue'
import MySearch from '@/components/mysearch/MySearch.vue'
import MyTable from '@/components/mytable/MyTable.vue'
```

## 6. 表格操作栏示例

表格列中加入 `operate` 操作列：

```ts
import {
  createMyTableOperationsColumn,
  type MyTableOperationsBtn,
  type MyTableOperationsColumn
} from '@/components/mytable/type'

const tableColumns: MyTableOperationsColumn[] = [
  createMyTableOperationsColumn({
    'min-width': 200,
    align: 'center'
  }),
  { prop: 'name', label: '姓名' },
  { prop: 'status', label: '状态' }
]

const operationButtons: MyTableOperationsBtn[] = [
  {
    evtname: 'info',
    text: '详情',
    attr: { type: 'primary', link: true }
  },
  {
    evtname: 'edit',
    text: '编辑',
    attr: { type: 'success', link: true }
  },
  {
    evtname: 'delete',
    text: '删除',
    attr: { type: 'danger', link: true }
  }
]

function handleOperation(index: number, row: unknown, eventName: string) {
  console.log(index, row, eventName)
}
```

## 7. 布局要求

如果希望表格区域占满页面剩余空间，C10 的父容器需要具有明确高度：

```css
.content {
  display: flex;
  height: 100%;
  min-height: 0;
}
```

组件内部使用纵向 Flex 布局：

- 筛选区高度由内容决定。
- 表格区占据剩余空间。
- 表格内容过高时，表格区独立滚动。

## 8. 职责边界

适合放入：

- `MySearch` 或其他筛选组件
- `MyTable`、`el-table` 或其他表格组件
- 表格区域的加载状态

不应放入组件内部：

- 固定筛选字段
- 固定表格列
- 分页和查询接口
- 删除、保存等业务逻辑

