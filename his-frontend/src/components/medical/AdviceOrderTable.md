# AdviceOrderTable（C54 医嘱表格）

门诊/住院通用的医嘱列表。接收页面转换好的 `AdviceOrder` view model，基于 Element Plus `el-table` 渲染，保持与项目其它表格一致的行为。**组件不请求医嘱接口、不写死列，也不提交/停止/执行医嘱。**

## Props

| 名称 | 类型 | 默认 | 说明 |
| --- | --- | --- | --- |
| `rows` | `AdviceOrder[]` | `[]` | 医嘱行数据 |
| `rowKey` | `keyof AdviceOrder` | `'key'` | 行主键字段 |
| `columns` | `AdviceColumn[]` | 医嘱内容 + 状态 | 列定义；`prop==='order'` 列默认用 `AdviceOrderDisplayCell` |
| `loading` | `boolean` | `false` | 加载中 |
| `readonly` | `boolean` | `false` | 只读，禁用勾选 |
| `selectable` | `boolean` | `false` | 是否显示勾选列 |
| `groupBy` | `keyof AdviceOrder \| (row) => string` | — | 行分组标识（供页面样式使用） |
| `currentRowKey` | `MedicalKey \| null` | `null` | 当前高亮行 key（受控） |
| `disabledRow` | `(row) => boolean` | `() => false` | 判断某行是否禁用勾选 |
| `emptyText` | `string` | `暂无医嘱` | 空数据文案 |

## Emits

| 事件 | 载荷 | 触发时机 |
| --- | --- | --- |
| `selectionChange` | `rows: AdviceOrder[]` | 勾选变化 |
| `currentChange` | `row?: AdviceOrder` | 当前高亮行变化 |
| `rowAction` | `{ row, action }` | 行操作插槽内调用 `action(name)` |

## Slots

`toolbar`、`order`（`{ row }`，医嘱内容列）、`cell-{prop}`（`{ row, column }`，任意列自定义）、`actions`（`{ row, action }`）、`empty`、`summary`。

## 数据模型

```ts
type MedicalKey = string | number

interface AdviceOrder {
  key: MedicalKey
  name: string
  specification?: string
  dose?: string | number
  unit?: string
  frequency?: string
  route?: string
  days?: string | number
  pharmacy?: string
  remark?: string
  status?: string
  group?: string
  groupItems?: AdviceOrder[]   // 医嘱内容列内嵌 C55 时用于成组括线
  raw?: unknown
}

interface AdviceColumn {
  prop: string
  label: string
  width?: string | number
  minWidth?: string | number
  formatter?: (row: AdviceOrder) => string
  align?: 'left' | 'center' | 'right'
}
```

## 备注

- 排序、分页、批量操作由页面负责；组件仅按 `rows` 顺序渲染当前页数据。
- `groupBy` 只生成 `advice-group-*` 行类名，不改变行顺序。
- 空值单元格统一显示 `-`。
- `prop==='order'` 列内嵌 [AdviceOrderDisplayCell](./AdviceOrderDisplayCell.md)，行数据带 `groupItems` 时自动呈现成组括线版式。
