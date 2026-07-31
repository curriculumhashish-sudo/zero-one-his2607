/** 通用下拉选项 */
export interface SelectOption {
  label: string
  value: string
}

/** 样本类型枚举 */
export type SampleType = 'BLOOD' | 'URINE' | 'TISSUE' | 'STOOL' | 'SPUTUM' | 'OTHER'

/** 样本管理查询参数 */
export interface SampleQuery {
  /** 关键词（名称、编码或拼音码） */
  keyword?: string
  /** 样本类型编码 */
  type?: string
  /** 启用状态 */
  enabled?: boolean | null
  /** 页码 */
  pageIndex: number
  /** 每页条数 */
  pageSize: number
}

/** 样本记录（列表展示用） */
export interface SampleRecord {
  id: string
  /** 样本编码（如 SAM001） */
  code: string
  /** 样本名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 样本类型编码 */
  type: string
  /** 样本类型名称（展示用） */
  typeName: string
  /** 采集方法 */
  collectionMethod: string
  /** 保存条件 */
  storageCondition: string
  /** 有效时长（小时） */
  validHours: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 更新时间 */
  updatedAt: string
}

/** 样本详情（编辑表单用，含全部字段） */
export interface SampleDetail {
  id?: string
  /** 样本编码 */
  code: string
  /** 样本名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 样本类型编码 */
  type: string
  /** 采集方法 */
  collectionMethod: string
  /** 保存条件 */
  storageCondition: string
  /** 有效时长（小时） */
  validHours: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 新增/保存参数 */
export interface SampleSavePayload {
  id?: string
  /** 样本编码 */
  code: string
  /** 样本名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 样本类型编码 */
  type: string
  /** 采集方法 */
  collectionMethod: string
  /** 保存条件 */
  storageCondition: string
  /** 有效时长（小时） */
  validHours: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 分页结果 */
export interface SamplePageResult {
  rows: SampleRecord[]
  total: number
}

/** 表单选项数据 */
export interface SampleFormOptions {
  typeOptions: SelectOption[]
}
