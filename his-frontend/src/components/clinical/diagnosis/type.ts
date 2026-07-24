import type { SelectTableLoadPage } from '@/components/common/select-type'

/** 诊断组件能够识别的稳定行标识。 */
export type ClinicalDiagnosisKey = string | number

/**
 * 门诊、住院共同使用的诊断展示模型。
 *
 * 这里故意不出现 encounterId、admissionId 等场景字段，
 * 业务页面需要通过 adapter 把自己的接口 VO 转换成这个模型。
 */
export interface ClinicalDiagnosisItem {
  /** 前端表格稳定行标识；未保存数据也必须有值 */
  key: ClinicalDiagnosisKey
  /** 后端诊断记录 ID；新添加但未保存时为空 */
  recordId?: string
  /** 诊断字典项目 ID */
  catalogId: string
  /** ICD 编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 诊断分类名称，例如西医诊断 */
  typeName: string
  /** 是否为主诊断 */
  primary: boolean
  /** 诊断时间，格式由业务页面决定 */
  diagnosedAt?: string
}

/**
 * 诊断字典候选项。
 * extends Record 是为了满足公共 SelectTable 的通用行类型约束。
 */
export interface ClinicalDiagnosisCandidate extends Record<string, unknown> {
  /** 诊断字典项目 ID */
  id: string
  /** ICD 编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 诊断分类名称 */
  typeName: string
}

/** 由门诊或住院页面注入的诊断字典分页加载函数。 */
export type ClinicalDiagnosisDictionaryLoader = SelectTableLoadPage<ClinicalDiagnosisCandidate>
