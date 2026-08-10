import type { SelectTableLoadPage } from '@/components/common/select-type'
import type { AdviceOrder, MedicalKey } from '@/components/medical/type'

/** 当前基础版支持的医嘱项目分类。 */
export type ClinicalOrderCategory = 'DRUG' | 'LAB' | 'EXAM' | 'TREATMENT'

/** 医嘱生命周期状态：撤回后保留记录，不再恢复为草稿。 */
export type ClinicalOrderStatus = 'DRAFT' | 'SIGNED' | 'WITHDRAWN'

/**
 * 门诊、住院共同使用的医嘱草稿。
 *
 * 继承 AdviceOrder 后可以直接交给现有 AdviceOrderTable 展示；
 * catalogId、category 等字段则用于编辑、校验和保存。
 */
export interface ClinicalOrderDraft extends AdviceOrder {
  /** 前端稳定行标识 */
  key: MedicalKey
  /** 已保存医嘱记录 ID；新草稿为空 */
  recordId?: string
  /** 医嘱项目目录 ID */
  catalogId: string
  /** 医嘱项目编码 */
  code: string
  /** 医嘱分类 */
  category: ClinicalOrderCategory
  /** 医嘱分类名称 */
  categoryName: string
  /** 开立数量 */
  quantity?: number
  /** 开立数量单位 */
  quantityUnit?: string
  /** 医嘱状态码 */
  statusCode?: string
  /** 场景侧状态显示名称；为空时由公共编辑器根据 status 映射。 */
  statusName?: string
  /** 疗程类型 */
  therapyType?: string
  /** 疗程类型名称 */
  therapyTypeName?: string
  /** 医嘱状态；只有草稿允许编辑和改变分组。 */
  status: ClinicalOrderStatus
  /** 医嘱分组标识；相同标识的医嘱属于同一组。 */
  groupId?: string
  /** 签发后生成的处方号。 */
  prescriptionNo?: string
  /** 非药品医嘱签发后生成的服务申请 ID。 */
  serviceRequestId?: string
  /** 一个开立数量单位所含的剂量，用于计算建议数量。 */
  dosePerQuantityUnit?: number
}

/** 医嘱项目选择弹窗中的候选项。 */
export interface ClinicalOrderCandidate extends Record<string, unknown> {
  /** 项目目录 ID */
  id: string
  /** 项目编码 */
  code: string
  /** 项目名称 */
  name: string
  /** 医嘱分类 */
  category: ClinicalOrderCategory
  /** 医嘱分类名称 */
  categoryName: string
  /** 规格 */
  specification?: string
  /** 默认单次剂量 */
  defaultDose?: number
  /** 默认剂量单位 */
  defaultUnit?: string
  /** 默认频次 */
  defaultFrequency?: string
  /** 默认用法 */
  defaultRoute?: string
  /** 默认疗程天数 */
  defaultDays?: number
  /** 默认开立数量 */
  defaultQuantity?: number
  /** 数量单位 */
  quantityUnit?: string
  /** 默认疗程类型 */
  defaultTherapyType?: string
  /** 一个数量单位所含剂量，例如每片 5mg。 */
  dosePerQuantityUnit?: number
}

/** 公共医嘱工作区使用的最小诊疗上下文。 */
export interface ClinicalOrderContext {
  /** 场景标识 */
  scope: 'outpatient' | 'inpatient'
  /** 患者 ID */
  patientId: string
  /** 本次门诊就诊或住院记录的统一键 */
  encounterKey: string
  /** 当前科室名称 */
  departmentName?: string
}

/** 单条医嘱校验错误。 */
export interface ClinicalOrderValidationError {
  /** 出错医嘱行标识 */
  key: MedicalKey
  /** 出错字段 */
  field: keyof ClinicalOrderDraft | 'order'
  /** 给医生看的错误说明 */
  message: string
}

/** 医嘱校验结果。 */
export interface ClinicalOrderValidationResult {
  valid: boolean
  errors: ClinicalOrderValidationError[]
}

/** 由门诊或住院页面注入的医嘱项目分页加载函数。 */
export type ClinicalOrderCatalogLoader = SelectTableLoadPage<ClinicalOrderCandidate>
