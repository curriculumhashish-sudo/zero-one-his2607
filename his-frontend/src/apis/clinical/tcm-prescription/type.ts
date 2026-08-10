import type { PageQuery } from '../../type'

/** 使用中医处方工作区的医生站场景。 */
export type TcmPrescriptionStationScope = 'outpatient' | 'inpatient'

/** 中医处方状态。 */
export type TcmPrescriptionStatus = 'DRAFT' | 'SIGNED'

/** 中药材目录项目。 */
export interface TcmHerbCatalogItemVO extends Record<string, unknown> {
  /** 药品定义 ID。 */
  id: string
  /** 药品编码。 */
  code: string
  /** 药品名称。 */
  name: string
  /** 规格。 */
  specification: string
  /** 默认剂量。 */
  defaultDose: number
  /** 剂量单位。 */
  doseUnit: string
  /** 零售价。 */
  retailPrice: number
}

/** 中药材目录查询条件。 */
export interface TcmHerbCatalogQuery extends PageQuery {
  /** 医生站场景。 */
  scope: TcmPrescriptionStationScope
  /** 药品编码或名称关键词。 */
  keyword?: string
}

/** 中医处方中的单味药材。 */
export interface TcmPrescriptionLineVO {
  /** 处方明细 ID。 */
  id: string
  /** 药品目录 ID。 */
  catalogId: string
  /** 药品编码。 */
  code: string
  /** 药品名称。 */
  name: string
  /** 规格。 */
  specification: string
  /** 单剂用量。 */
  dose: number
  /** 剂量单位。 */
  doseUnit: string
  /** 零售价。 */
  retailPrice: number
  /** 排序号。 */
  orderNo: number
}

/** 中医处方方头及药材明细。 */
export interface TcmPrescriptionVO {
  /** 处方记录 ID。 */
  id: string
  /** 医生站场景。 */
  scope: TcmPrescriptionStationScope
  /** 就诊记录 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 处方号；草稿阶段为空。 */
  prescriptionNo?: string
  /** 本次就诊诊断记录 ID。 */
  encounterDiagnosisId: string
  /** 关联中医病证。 */
  syndromeName: string
  /** 付数。 */
  doseCount: number
  /** 用法。 */
  method: string
  /** 频次。 */
  frequency: string
  /** 疗程天数。 */
  days: number
  /** 煎药方式。 */
  decoctionMethod?: string
  /** 处方备注。 */
  remark?: string
  /** 处方状态。 */
  status: TcmPrescriptionStatus
  /** 处方状态名称。 */
  statusName: string
  /** 药材明细。 */
  lines: TcmPrescriptionLineVO[]
  /** 开立时间。 */
  authoredAt: string
  /** 最近更新时间。 */
  updatedAt: string
  /** 处方版本；保存/删除时由 API 层自动携带，页面无需感知。 */
  version?: number
}

/** 查询当前就诊中医处方。 */
export interface TcmPrescriptionQuery {
  /** 医生站场景。 */
  scope: TcmPrescriptionStationScope
  /** 就诊记录 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
}

/** 中医处方历史摘要。 */
export interface TcmPrescriptionHistoryVO {
  /** 历史记录 ID。 */
  id: string
  /** 处方号。 */
  prescriptionNo: string
  /** 就诊记录 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 开立时间。 */
  authoredAt: string
  /** 处方摘要。 */
  summary: string
  /** 药材名称。 */
  herbNames: string[]
}

/** 查询患者历史中医处方。 */
export interface TcmPrescriptionHistoryQuery {
  /** 医生站场景。 */
  scope: TcmPrescriptionStationScope
  /** 患者档案 ID。 */
  patientId: string
  /** 当前就诊 ID，用于排除当前处方。 */
  currentEncounterId: string
  /** 处方号、摘要或药材关键词。 */
  keyword?: string
}

/** 保存时提交的单味药材。 */
export interface SaveTcmPrescriptionLineDTO {
  /** 药品定义 ID。 */
  medicineId: string
  /** 单剂数量。 */
  quantity: number
  /** 数量单位。 */
  unit?: string
  /** 单次剂量。 */
  dose?: number
  /** 剂量单位。 */
  doseUnit?: string
  /** 排序号。 */
  sortNo: number
}

/** 新增或保存中医处方草稿。 */
export interface SaveTcmPrescriptionDTO {
  /** 医生站场景。 */
  scope: TcmPrescriptionStationScope
  /** 就诊记录 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 已保存处方 ID；新增处方为空。 */
  id?: string
  /** 处方版本；修改已有处方时 API 层自动携带，页面无需感知。 */
  version?: number
  /** 本次就诊诊断记录 ID。 */
  encounterDiagnosisId: string
  /** 诊断名称，仅用于前端草稿和 Mock 展示，不提交真实接口。 */
  diagnosisName: string
  /** 付数。 */
  copies: number
  /** 用法。 */
  usage: string
  /** 频次。 */
  frequency: string
  /** 疗程天数。 */
  days: number
  /** 是否代煎。 */
  decoction?: number
  /** 处方备注。 */
  remark?: string
  /** 药材明细。 */
  details: SaveTcmPrescriptionLineDTO[]
}

/** 中医处方生命周期操作参数。 */
export interface TcmPrescriptionActionDTO {
  /** 医生站场景。 */
  scope: TcmPrescriptionStationScope
  /** 就诊记录 ID。 */
  encounterId: string
  /** 处方 ID。 */
  id: string
  /** 处方版本；API 层自动携带，页面无需感知。 */
  version?: number
}
