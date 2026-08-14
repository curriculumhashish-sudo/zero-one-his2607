# CalendarSchedule（C28 排班/预约日历）

受控的**月视图 / 周视图**排班日历。纯展示与交互组件：事件、资源均由页面注入；组件只在切换范围时发出 `rangeChange`，由页面决定何时加载数据。**不请求排班接口、不写死状态码、不做 UTC 转换。**

## 时间约定

- 所有日期使用本地日历字符串 `YYYY-MM-DD`。
- 事件的 `start` / `end` 为当天时间 `HH:mm`（可选），与 `date` 字段分离。
- 组件内部使用原生 `Date` 计算范围与网格，不依赖 dayjs。

## Props

| 名称 | 类型 | 默认 | 说明 |
| --- | --- | --- | --- |
| `modelValue` | `string` | — | 当前锚点日期（受控），`YYYY-MM-DD` |
| `view` | `'month' \| 'week'` | `'week'` | 视图模式（受控） |
| `events` | `ScheduleEvent[]` | `[]` | 事件列表 |
| `resources` | `ScheduleResource[]` | `[]` | 资源列表，用于事件左侧色条着色 |
| `loading` | `boolean` | `false` | 加载中显示骨架屏 |
| `firstDayOfWeek` | `0 \| 1` | `1` | 每周第一天：0=周日，1=周一 |
| `statusMap` | `ScheduleStatusMap` | `{}` | 状态标识 → 文案+语义色，组件不写死状态码 |
| `readonly` | `boolean` | `false` | 只读，禁用日期/时段交互 |
| `maxEventsPerDay` | `number` | `3` | 单元格最多展示的事件数，超出折叠为「更多」 |
| `emptyText` | `string` | `当前范围暂无排班` | 空数据文案 |

## Emits

| 事件 | 载荷 | 触发时机 |
| --- | --- | --- |
| `update:modelValue` | `date: string` | 上/下一周期、点「今天」 |
| `update:view` | `view: CalendarView` | 切换月/周 |
| `rangeChange` | `range: CalendarDateRange` | 挂载后及范围变化时（含首帧），通知页面加载区间数据 |
| `dateClick` | `date, cell` | 点击单元格空白区、或点「更多」 |
| `slotClick` | `date, cell` | 点击日期数字（新建入口） |
| `eventClick` | `event` | 点击单条事件 |
| `eventAction` | `action, event` | 事件上的自定义动作（由插槽触发） |

## Slots

`toolbar`（`{ range, view, title }`）、`dayHeader`（`{ cell }`）、`event`（`{ event, statusText }`）、`more`（`{ cell, count }`）、`empty`。

`events` 为空时，`empty` 作为网格上方的提示区域显示，日期网格仍然保留，因此页面仍可通过 `dateClick` / `slotClick` 创建排班。

## 数据模型

```ts
type CalendarView = 'month' | 'week'

interface ScheduleEvent {
  key: string | number
  date: string          // YYYY-MM-DD
  title: string
  start?: string        // HH:mm
  end?: string          // HH:mm
  status?: string       // 由 statusMap 决定文案/色
  resourceKey?: string | number
  raw?: unknown
}

interface ScheduleResource {
  key: string | number
  name: string
  color?: string        // 事件左侧色条
  raw?: unknown
}

interface ScheduleStatusMeta { text: string; type?: 'primary' | 'success' | 'warning' | 'danger' | 'info' | 'default' }
type ScheduleStatusMap = Record<string, ScheduleStatusMeta>

interface CalendarDateRange { start: string; end: string }
```

## 备注

- `rangeChange` 使用 `immediate` + `deep`，首帧即上报，切换视图/周期会再次上报，页面据此增量加载。
- 事件着色优先取 `resources[].color`，否则回退 `statusMap[status].type` 对应的 `--el-color-*`。
- 只读模式下 `dateClick` / `slotClick` 不发出。
- 锚点日（`modelValue`）在网格中以 `primary-light-9` 底色高亮，「今天」另有顶部主题色条。
