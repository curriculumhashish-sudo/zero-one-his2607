/** 通用下拉选项 */
export interface SelectOption {
  label: string
  value: string
}

/** 频次类型枚举 */
export type FrequencyType = 'DAILY' | 'WEEKLY' | 'HOURLY' | 'AS_NEEDED'

/** 频次查询参数 */
export interface FrequencyQuery {
  /** 关键词（名称或编码） */
  keyword?: string
  /** 频次类型 */
  type?: string
  /** 启用状态 */
  enabled?: boolean | null
  /** 页码 */
  pageIndex: number
  /** 每页条数 */
  pageSize: number
}

/** 频次记录（列表展示用） */
export interface FrequencyRecord {
  id: string
  /** 频次编码（如 QD, BID, TID） */
  code: string
  /** 频次名称（如 每日一次, 每日两次） */
  name: string
  /** 频次类型 */
  type: string
  /** 频次类型名称（展示用） */
  typeName: string
  /** 每日执行次数 */
  timesPerDay: number
  /** 间隔时间(小时) */
  intervalHours: number
  /** 执行时间点（如 08:00,12:00,20:00） */
  timePoints: string
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 更新时间 */
  updatedAt: string
}

/** 频次详情（编辑表单用，含全部字段） */
export interface FrequencyDetail {
  id?: string
  /** 频次编码（如 QD, BID, TID） */
  code: string
  /** 频次名称（如 每日一次, 每日两次） */
  name: string
  /** 频次类型 */
  type: string
  /** 每日执行次数 */
  timesPerDay: number
  /** 间隔时间(小时) */
  intervalHours: number
  /** 执行时间点（如 08:00,12:00,20:00） */
  timePoints: string
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 新增/保存参数 */
export interface FrequencySavePayload {
  id?: string
  /** 频次编码（如 QD, BID, TID） */
  code: string
  /** 频次名称（如 每日一次, 每日两次） */
  name: string
  /** 频次类型 */
  type: string
  /** 每日执行次数 */
  timesPerDay: number
  /** 间隔时间(小时) */
  intervalHours: number
  /** 执行时间点（如 08:00,12:00,20:00） */
  timePoints: string
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 分页结果 */
export interface FrequencyPageResult {
  rows: FrequencyRecord[]
  total: number
}

/** 表单选项数据 */
export interface FrequencyFormOptions {
  typeOptions: SelectOption[]
}
