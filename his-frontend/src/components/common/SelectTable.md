# SelectTable

搜索表格选择组件。适用于患者、药品、诊疗项目和库存物资等需要通过多列信息判断的单选或多选场景。

组件内部维护选择草稿：取消不会修改外部值，确认后才更新 `v-model`。多选模式支持跨页保留选择。

## 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import SelectTable from '@/components/common/SelectTable.vue'
import type { SelectTableColumn } from '@/components/common/select-type'

interface PatientRow extends Record<string, unknown> {
  id: string
  name: string
  gender: string
  recordNo: string
}

const visible = ref(false)
const patientId = ref<string | null>(null)
const selectedPatients = ref<PatientRow[]>([])

const columns: SelectTableColumn<PatientRow>[] = [
  { prop: 'recordNo', label: '病历号', width: 140 },
  { prop: 'name', label: '姓名', minWidth: 120 },
  { prop: 'gender', label: '性别', width: 80 }
]

async function loadPatients(
  query: { keyword: string; pageIndex: number; pageSize: number },
  context: { signal: AbortSignal }
) {
  const response = await fetch('/api/patients', {
    method: 'POST',
    body: JSON.stringify(query),
    signal: context.signal
  })
  return (await response.json()) as { rows: PatientRow[]; total: number }
}

function handleConfirm(value: string | number | null | Array<string | number>, rows: PatientRow[]) {
  selectedPatients.value = rows
}
</script>

<template>
  <el-button @click="visible = true">选择患者</el-button>

  <SelectTable
    v-model="patientId"
    v-model:visible="visible"
    title="选择患者"
    row-key="id"
    :columns="columns"
    :load-page="loadPatients"
    :selected-rows="selectedPatients"
    @confirm="handleConfirm"
  />
</template>
```

## 多选模式

```vue
<SelectTable
  v-model="medicationIds"
  v-model:visible="medicationVisible"
  selection-mode="multiple"
  row-key="id"
  :columns="medicationColumns"
  :load-page="loadMedications"
  :disabled-row="(row) => row.stock <= 0"
>
  <template #cell-stock="{ row }">
    <el-tag :type="row.stock > 0 ? 'success' : 'danger'">
      {{ row.stock }}
    </el-tag>
  </template>
</SelectTable>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
|---|---|---|---|
| `modelValue` | `SelectValue \| SelectValue[] \| null` | `null` | 确认后的选择值 |
| `visible` | `boolean` | `false` | 对话框显示状态，使用 `v-model:visible` |
| `columns` | `SelectTableColumn<T>[]` | 必填 | 表格列配置 |
| `rowKey` | `string` | `id` | 稳定行键字段 |
| `selectionMode` | `single \| multiple` | `single` | 选择模式 |
| `loadPage` | `(query, { signal }) => Promise<{ rows, total }>` | - | 远程分页函数 |
| `data` | `T[]` | `[]` | 不提供 `loadPage` 时使用的本地数据 |
| `selectedRows` | `T[]` | `[]` | 已选对象，用于重新打开时完整回显 |
| `disabledRow` | `(row: T) => boolean` | - | 判断行是否不可选择 |
| `title` | `string` | `请选择` | 对话框标题 |
| `width` | `string \| number` | `900px` | 对话框宽度 |
| `pageSize` | `number` | `10` | 初始每页数量 |
| `pageSizes` | `number[]` | `[10, 20, 50]` | 每页数量选项 |
| `keywordPlaceholder` | `string` | `请输入关键字` | 查询占位文字 |
| `searchLabel` | `string` | `关键字` | 查询字段标题 |
| `emptyText` | `string` | `暂无匹配数据` | 空态文字 |

## Events

| 事件 | 参数 | 说明 |
|---|---|---|
| `update:modelValue` | `value` | 点击确认后更新 |
| `update:visible` | `visible` | 对话框状态变化 |
| `confirm` | `value, rows` | 确认选择 |
| `cancel` | - | 取消、关闭图标或外部关闭 |
| `search` | `keyword` | 执行查询 |
| `pageChange` | `pageIndex, pageSize` | 分页变化 |
| `loadError` | `error` | 加载失败 |

## Slots

| 插槽 | 参数 | 说明 |
|---|---|---|
| `search` | `{ keyword, setKeyword, search, loading }` | 替换默认搜索区 |
| `cell-{prop}` | `{ row, column, index }` | 自定义指定列 |
| `selected` | `{ keys, rows, clear }` | 自定义已选区域 |
| `empty` | `{ error, reload }` | 自定义空态/失败状态 |
| `footer` | `{ confirm, cancel, selectedCount }` | 自定义底部按钮 |

## 暴露方法

- `open()` / `close()`：打开或关闭。
- `reload()`：重新加载当前页。
- `clear()`：清空选择草稿。

## 边界说明

- 不要使用数组下标作为 `rowKey`。
- 只有 `modelValue` 而没有 `selectedRows` 时，组件仍能保存键，但无法凭空恢复不在当前页的完整对象。
- 组件不引入 `vxe-table`，也不写死患者、药品和项目列。

