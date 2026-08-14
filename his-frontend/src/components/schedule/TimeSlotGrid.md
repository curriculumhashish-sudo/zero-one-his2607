# TimeSlotGrid（C29 号源/时段选择网格）

受控的号源单选网格。号源由页面注入，可选性由 `statusMap` 或剩余量决定。**不请求预约接口、不写死业务号源。**

## Props

| 名称 | 类型 | 默认 | 说明 |
| --- | --- | --- | --- |
| `slots` | `TimeSlot[]` | `[]` | 号源列表（`key/start/end/label/capacity/remaining/status/group`） |
| `modelValue` | `ScheduleKey \| null` | `null` | 当前选中号源 key（受控） |
| `groupBy` | `string \| (slot) => string` | — | 分组：字段名或取值函数；不传则不分组 |
| `statusMap` | `TimeSlotStatusMap` | `{}` | 覆盖内置状态（文案/语义色/`selectable`） |
| `loading` | `boolean` | `false` | 加载中显示骨架屏 |
| `readonly` | `boolean` | `false` | 只读，任何号源都不可选 |
| `showCapacity` | `boolean` | `true` | 是否展示容量/剩余信息 |
| `emptyText` | `string` | `暂无可选时段` | 空数据文案 |

## 可选性规则

按优先级判断某号源是否可选：

1. `remaining !== undefined && remaining <= 0` → 不可选；
2. 命中 `statusMap[status].selectable` → 以该值为准；
3. 否则仅 `status === 'available'` 可选。

内置默认状态（可被 `statusMap` 覆盖）：`available` 可选，`full` / `disabled` / `expired` / `stopped` 不可选。

## Emits

| 事件 | 载荷 | 触发时机 |
| --- | --- | --- |
| `update:modelValue` | `key` | 选中一个可选号源 |
| `select` | `slot` | 同上，携带完整号源对象 |
| `disabledClick` | `slot` | 点击不可选号源或只读态下点击 |

## Slots

`item`（`{ data, selectable, statusText }`，自定义单个号源）、`groupHeader`（`{ group, slots }`）、`empty`、`footer`。

## 数据模型

```ts
interface TimeSlot {
  key: string | number
  start?: string        // HH:mm
  end?: string          // HH:mm
  label?: string        // 缺省时由 start/end 拼接
  capacity?: number     // 总号数
  remaining?: number    // 剩余号数
  status?: 'available' | 'full' | 'disabled' | 'expired' | 'stopped' | string
  group?: string        // 供 groupBy='group' 分组
  raw?: unknown
}

interface TimeSlotStatusMeta { text: string; type?: 'primary' | 'success' | 'warning' | 'danger' | 'info' | 'default'; selectable?: boolean }
type TimeSlotStatusMap = Record<string, TimeSlotStatusMeta>
```

## 备注

- 不可选号源不使用原生 `disabled`，以便仍能发出 `disabledClick`，页面可据此提示「已满/停诊」。
- 状态色通过 `is-{type}` class 映射到 `--el-color-*`。
- 卡片 hover 有阴影过渡，选中态为 `primary` 描边 + `primary-light-9` 底色，剩余量固定压在卡片底部。
