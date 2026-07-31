/** 通用下拉选项 */
export interface SelectOption {
  label: string
  value: string
}

/** 药品管理查询参数 */
export interface MedicationQuery {
  /** 关键词（名称或编码） */
  keyword?: string
  /** 药品类别 */
  category?: string
  /** 启用状态 */
  enabled?: boolean | null
  /** 页码 */
  pageIndex: number
  /** 每页条数 */
  pageSize: number
}

/** 药品记录（列表展示用） */
export interface MedicationRecord {
  id: string
  /** 药品编码（如 MED001） */
  code: string
  /** 药品名称 */
  name: string
  /** 通用名 */
  genericName: string
  /** 药品类别（WM/TCM/HM/BP） */
  category: string
  /** 药品类别名称（展示用） */
  categoryName: string
  /** 规格 */
  specification: string
  /** 剂型（TABLET/CAPSULE/INJECTION/ORAL_LIQUID/GRANULE/OINTMENT） */
  dosageForm: string
  /** 剂型名称（展示用） */
  dosageFormName: string
  /** 单位 */
  unit: string
  /** 生产厂家 */
  manufacturer: string
  /** 零售价 */
  retailPrice: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
  /** 更新时间 */
  updatedAt: string
}

/** 药品详情（编辑表单用，含全部字段） */
export interface MedicationDetail {
  id?: string
  /** 药品编码 */
  code: string
  /** 药品名称 */
  name: string
  /** 通用名 */
  genericName: string
  /** 药品类别 */
  category: string
  /** 规格 */
  specification: string
  /** 剂型 */
  dosageForm: string
  /** 单位 */
  unit: string
  /** 生产厂家 */
  manufacturer: string
  /** 零售价 */
  retailPrice: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 新增/保存参数 */
export interface MedicationSavePayload {
  id?: string
  /** 药品编码 */
  code: string
  /** 药品名称 */
  name: string
  /** 通用名 */
  genericName: string
  /** 药品类别 */
  category: string
  /** 规格 */
  specification: string
  /** 剂型 */
  dosageForm: string
  /** 单位 */
  unit: string
  /** 生产厂家 */
  manufacturer: string
  /** 零售价 */
  retailPrice: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 分页结果 */
export interface MedicationPageResult {
  rows: MedicationRecord[]
  total: number
}

/** 表单选项数据 */
export interface MedicationFormOptions {
  categoryOptions: SelectOption[]
  dosageFormOptions: SelectOption[]
}
