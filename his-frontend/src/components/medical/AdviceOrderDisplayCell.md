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

## Emits

| 事件 | 载荷 | 触发时机 |
| --- | --- | --- |
| `click` | `order` | 点击整个单元 |
| `action` | `order, action` | 供插槽内自定义动作使用 |

## Slots

`name`、`details`、`tags`、`remark`、`extra`（均带 `{ order }` 作用域）。

## 备注

- 主字段名、状态、备注均通过 `fields` 映射解析，便于直接接入门诊/住院不同 DTO。
- `compact` 模式下备注块通过 CSS 隐藏（DOM 保留），供表格内密集展示。
