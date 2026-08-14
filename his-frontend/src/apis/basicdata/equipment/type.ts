import type { SelectOption } from '../type'

export type { SelectOption }

/** 器材分类枚举 */
export type EquipmentCategory = 'CONSUMABLE' | 'INSTRUMENT' | 'EQUIPMENT'

/** 器材管理查询参数 */
export interface EquipmentQuery {
  /** 关键词（名称或编码） */
  keyword?: string
  /** 器材分类 */
  category?: string
  /** 启用状态 */
  enabled?: boolean | null
  /** 页码 */
  pageIndex: number
  /** 每页条数 */
  pageSize: number
}

/** 器材记录（列表展示用） */
export interface EquipmentRecord {
  id: string
  /** 器材编码（如 EQ001） */
  code: string
  /** 器材名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 器材分类 */
  category: string
  /** 器材分类名称（展示用） */
  categoryName: string
  /** 规格 */
  specification: string
  /** 型号 */
  model: string
  /** 计量单位 */
  unit: string
  /** 单价 */
  price: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
  /** 更新时间 */
  updatedAt: string
}

/** 器材详情（编辑表单用，含全部字段） */
export interface EquipmentDetail {
  id?: string
  /** 器材编码 */
  code: string
  /** 器材名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 器材分类 */
  category: EquipmentCategory
  /** 规格 */
  specification: string
  /** 型号 */
  model: string
  /** 计量单位 */
  unit: string
  /** 单价 */
  price: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 新增/保存参数 */
export interface EquipmentSavePayload {
  id?: string
  /** 器材编码 */
  code: string
  /** 器材名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 器材分类 */
  category: EquipmentCategory
  /** 规格 */
  specification: string
  /** 型号 */
  model: string
  /** 计量单位 */
  unit: string
  /** 单价 */
  price: number
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 分页结果 */
export interface EquipmentPageResult {
  rows: EquipmentRecord[]
  total: number
}

/** 表单选项数据 */
export interface EquipmentFormOptions {
  categoryOptions: SelectOption[]
  unitOptions: SelectOption[]
}
