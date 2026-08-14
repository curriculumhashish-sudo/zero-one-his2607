/** 通用下拉选项 */
export interface SelectOption {
  label: string
  value: string
}

/** 诊疗类别枚举 */
export type TreatmentCategory = 'EXAM' | 'LAB' | 'THERAPY' | 'SURGERY'

/** 诊疗项目查询参数 */
export interface TreatmentQuery {
  /** 关键词（名称或编码） */
  keyword?: string
  /** 诊疗类别 */
  category?: string
  /** 启用状态 */
  enabled?: boolean | null
  /** 页码 */
  pageIndex: number
  /** 每页条数 */
  pageSize: number
}

/** 诊疗项目记录（列表展示用） */
export interface TreatmentRecord {
  id: string
  /** 诊疗项目编码（如 EXAM001） */
  code: string
  /** 诊疗项目名称（如 血常规检查） */
  name: string
  /** 拼音码（如 XCGJC） */
  pyCode: string
  /** 诊疗类别编码 */
  category: string
  /** 诊疗类别名称（展示用） */
  categoryName: string
  /** 计价单位（如 次, 项） */
  unit: string
  /** 价格（如 25.00） */
  price: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 更新时间 */
  updatedAt: string
}

/** 诊疗项目详情（编辑表单用，含全部字段） */
export interface TreatmentDetail {
  id?: string
  /** 诊疗项目编码 */
  code: string
  /** 诊疗项目名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 诊疗类别编码 */
  category: string
  /** 计价单位 */
  unit: string
  /** 价格 */
  price: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark?: string
}

/** 新增/保存参数 */
export interface TreatmentSavePayload {
  id?: string
  /** 诊疗项目编码 */
  code: string
  /** 诊疗项目名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 诊疗类别编码 */
  category: string
  /** 计价单位 */
  unit: string
  /** 价格 */
  price: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark?: string
}

/** 分页结果 */
export interface TreatmentPageResult {
  rows: TreatmentRecord[]
  total: number
}

/** 表单选项数据 */
export interface TreatmentFormOptions {
  categoryOptions: SelectOption[]
  unitOptions: SelectOption[]
}
