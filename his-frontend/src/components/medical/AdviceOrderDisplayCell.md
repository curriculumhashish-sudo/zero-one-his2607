# AdviceOrderDisplayCell（C55 医嘱展示单元）

紧凑展示单条医嘱 view model，常用于 [AdviceOrderTable](./AdviceOrderTable.md) 的医嘱内容列，也可独立使用。**纯展示，不含编辑与业务提交。**

## Props

| 名称 | 类型 | 默认 | 说明 |
| --- | --- | --- | --- |
| `order` | `AdviceOrder` | — | 医嘱数据，至少含 `key/name` |
| `fields` | `Partial<Record<字段, keyof AdviceOrder>>` | `{}` | 字段映射：把不同 DTO 的字段名映射到内置语义字段（`name/status/remark/规格/剂量…`） |
| `compact` | `boolean` | `false` | 紧凑模式，收紧行距并隐藏备注 |
| `maxLines` | `number` | `2` | 预留的最大展示行数 |
| `showRemark` | `boolean` | `true` | 是否展示备注 |
| `showStatus` | `boolean` | `false` | 是否展示状态标签 |
| `statusMap` | `Record<string, string>` | `{}` | 状态码 → 文案 |

## 明细拼接

`details` 由「规格 · 剂量+单位 · 频次 · 途径 · 天数」按顺序拼接，**缺失字段自动过滤**，不产生空括号或多余分隔符，也不会出现 `undefined`。所有字段取值均经过 `fields` 映射。

## 成组括线版式

当 `order.groupItems` 存在且非空时，自动切换为「成组括线」版式（贴合真实 HIS 医嘱单）：左侧一条竖括线拢住组内多味药，每味药一行，底部一行组内共用信息。

- **每行题名**：`名称【规格】（药房）`，缺省片段自动省略。
- **每行剂量**：`单次剂量+单位/次`，居右。
- **共用行**：`途径，频次（N天）`，取组内首项，缺省片段省略。
- `groupItems` 为空数组或缺省时，回退单条医嘱版式（向后兼容）。

## 数据模型

```ts
interface AdviceOrder {
  key: string | number
  name: string
  specification?: string
  dose?: string | number
  unit?: string
  frequency?: string
  route?: string
  days?: string | number
  pharmacy?: string        // 药房/发药点，用于题名括注
  remark?: string
  status?: string
  group?: string
  groupItems?: AdviceOrder[] // 非空则启用成组括线版式，子项复用同结构
  raw?: unknown
}
```

## Emits

| 事件 | 载荷 | 触发时机 |
| --- | --- | --- |
| `click` | `order` | 点击整个单元 |
| `action` | `order, action` | 供插槽内自定义动作使用 |

## Slots

`name`、`details`、`tags`、`remark`、`extra`（均带 `{ order }` 作用域）；成组版式下另有 `line`（`{ item, index }`）、`shared`（`{ order }`）。

## 备注

- 主字段名、状态、备注均通过 `fields` 映射解析，便于直接接入门诊/住院不同 DTO。
- `compact` 模式下备注块通过 CSS 隐藏（DOM 保留），供表格内密集展示。
- 单条 / 成组两种版式共用同一套 `fields` 映射与状态展示逻辑。
