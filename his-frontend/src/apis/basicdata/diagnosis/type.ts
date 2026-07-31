import type { SelectOption } from '../type'

/** 诊断类型枚举 */
export type DiagnosisType = 'WESTERN' | 'TCM'

/** 诊断分类枚举 */
export type DiagnosisCategory = 'OUTPATIENT' | 'INPATIENT' | 'SURGERY' | 'ETIOLOGY'

/** 诊断管理查询参数 */
export interface DiagnosisQuery {
  /** 关键词（名称或编码） */
  keyword?: string
  /** 诊断类型 */
  type?: string
  /** 启用状态 */
  enabled?: boolean | null
  /** 页码 */
  pageIndex: number
  /** 每页条数 */
  pageSize: number
}

/** 诊断记录（列表展示用） */
export interface DiagnosisRecord {
  id: string
  /** 诊断编码（ICD-10，如 J00） */
  code: string
  /** 诊断名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 诊断类型 */
  type: string
  /** 诊断类型名称（展示用） */
  typeName: string
  /** 诊断分类 */
  category: string
  /** 诊断分类名称（展示用） */
  categoryName: string
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
  /** 更新时间 */
  updatedAt: string
}

/** 诊断详情（编辑表单用，含全部字段） */
export interface DiagnosisDetail {
  id?: string
  /** 诊断编码（ICD-10，如 J00） */
  code: string
  /** 诊断名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 诊断类型 */
  type: DiagnosisType
  /** 诊断分类 */
  category: DiagnosisCategory
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 新增/保存参数 */
export interface DiagnosisSavePayload {
  id?: string
  /** 诊断编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 拼音码 */
  pyCode: string
  /** 诊断类型 */
  type: DiagnosisType
  /** 诊断分类 */
  category: DiagnosisCategory
  /** 启用状态 */
  enabled: boolean
  /** 排序号 */
  sort: number
  /** 备注 */
  remark: string
}

/** 分页结果 */
export interface DiagnosisPageResult {
  rows: DiagnosisRecord[]
  total: number
}

/** 表单选项数据 */
export interface DiagnosisFormOptions {
  typeOptions: SelectOption[]
  categoryOptions: SelectOption[]
}
