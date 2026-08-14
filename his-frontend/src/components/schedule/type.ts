/**
 * 排班/预约类组件（C28 CalendarSchedule、C29 TimeSlotGrid）共用数据模型。
 *
 * 时区约定：所有日期统一使用「本地日历日期」字符串，格式 `YYYY-MM-DD`；
 * 时间使用 24 小时制 `HH:mm`。组件内部不做 UTC 转换，页面注入前需自行归一。
 * 组件只负责展示与交互，不请求排班接口。
 */

/** 稳定 key 类型 */
export type ScheduleKey = string | number

/** 日历视图类型 */
export type CalendarView = 'month' | 'week'

/**
 * 日历资源模型（排班对象，如医生、诊室、体检项目/设备等）
 */
export interface ScheduleResource {
  /** 资源唯一标识 */
  key: ScheduleKey
  /** 资源名称 */
  name: string
  /** 资源主题色，用于事件着色，可选 */
  color?: string
  /** 原始业务数据，透传使用 */
  raw?: unknown
}

/**
 * 日历事件模型（一条排班/预约记录）
 *
 * 至少包含 key、日期、标题；开始/结束时间、状态、资源 key 均为可选。
 */
export interface ScheduleEvent {
  /** 事件唯一标识 */
  key: ScheduleKey
  /** 事件所属日期，格式 `YYYY-MM-DD` */
  date: string
  /** 事件标题 */
  title: string
  /** 开始时间 `HH:mm`，可选 */
  start?: string
  /** 结束时间 `HH:mm`，可选 */
  end?: string
  /** 事件状态标识，具体文案与色彩由外部 statusMap 决定 */
  status?: string
  /** 关联资源 key，可选 */
  resourceKey?: ScheduleKey
  /** 原始业务数据，透传使用 */
  raw?: unknown
}

/** 事件状态展示映射（文案 + 语义色），由页面注入，组件不写死状态码 */
export interface ScheduleStatusMeta {
  /** 状态文案 */
  text: string
  /** 语义类型，用于着色 */
  type?: 'default' | 'primary' | 'success' | 'warning' | 'danger' | 'info'
}

/** 状态映射表：状态标识 -> 展示元数据 */
export type ScheduleStatusMap = Record<string, ScheduleStatusMeta>

/** 日历可见日期范围（用于通知页面加载对应区间数据） */
export interface CalendarDateRange {
  /** 起始日期 `YYYY-MM-DD`（含） */
  start: string
  /** 结束日期 `YYYY-MM-DD`（含） */
  end: string
}

/** 日历单元格模型（组件内部计算生成，供 dayHeader/event 插槽使用） */
export interface CalendarDayCell {
  /** 日期 `YYYY-MM-DD` */
  date: string
  /** 当月第几天 */
  day: number
  /** 是否属于当前展示月份（月视图用于区分补位日期） */
  isCurrentMonth: boolean
  /** 是否今天 */
  isToday: boolean
  /** 是否周末 */
  isWeekend: boolean
  /** 该日期下的事件列表 */
  events: ScheduleEvent[]
}

/** 号源/时段状态 */
export type TimeSlotStatus = 'available' | 'full' | 'disabled' | 'expired' | 'stopped' | string

/** 号源状态展示映射元数据 */
export interface TimeSlotStatusMeta {
  /** 状态文案 */
  text: string
  /** 语义类型 */
  type?: 'default' | 'primary' | 'success' | 'warning' | 'danger' | 'info'
  /** 是否可选，缺省按内置规则：available 可选，其余不可选 */
  selectable?: boolean
}

/** 号源状态映射表 */
export type TimeSlotStatusMap = Record<string, TimeSlotStatusMeta>

/**
 * 时段/号源模型（C29 TimeSlotGrid）
 */
export interface TimeSlot {
  /** 号源唯一标识 */
  key: ScheduleKey
  /** 开始时间 `HH:mm`，可选 */
  start?: string
  /** 结束时间 `HH:mm`，可选 */
  end?: string
  /** 展示文本，缺省时由 start/end 拼接 */
  label?: string
  /** 总容量（总号数），可选 */
  capacity?: number
  /** 剩余容量（剩余号数），可选 */
  remaining?: number
  /** 号源状态，缺省视为 available */
  status?: TimeSlotStatus
  /** 分组标识，如 上午/下午，用于 groupBy='group' 分组 */
  group?: string
  /** 原始业务数据，透传使用 */
  raw?: unknown
}

/** 内置号源状态默认文案（页面可通过 statusMap 覆盖） */
export const DEFAULT_TIME_SLOT_STATUS_MAP: TimeSlotStatusMap = {
  available: { text: '可选', type: 'success', selectable: true },
  full: { text: '已满', type: 'danger', selectable: false },
  disabled: { text: '不可选', type: 'info', selectable: false },
  expired: { text: '已过号', type: 'info', selectable: false },
  stopped: { text: '停诊', type: 'warning', selectable: false }
}
