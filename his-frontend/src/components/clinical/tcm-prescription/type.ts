import type { SelectTableLoadPage } from '@/components/common/select-type'
import type {
  TcmHerbCatalogItemVO,
  TcmPrescriptionLineVO,
  TcmPrescriptionStationScope,
  TcmPrescriptionStatus
} from '@/apis/clinical/tcm-prescription'

/** 中医处方可关联的已保存诊断。 */
export interface ClinicalTcmPrescriptionDiagnosisOption {
  /** 本次就诊诊断记录 ID。 */
  id: string
  /** 诊断名称。 */
  name: string
}

/** 公共中医处方组件使用的患者上下文。 */
export interface ClinicalTcmPrescriptionContext {
  /** 医生站场景。 */
  scope: TcmPrescriptionStationScope
  /** 就诊记录 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 当前就诊中可关联的已保存诊断。 */
  diagnoses?: ClinicalTcmPrescriptionDiagnosisOption[]
}

/** 可编辑的中药材明细。 */
export interface ClinicalTcmPrescriptionLine extends Omit<TcmPrescriptionLineVO, 'id'> {
  /** 前端稳定行标识。 */
  key: string
  /** 已保存明细 ID。 */
  id?: string
}

/** 可编辑的中医处方。 */
export interface ClinicalTcmPrescriptionDraft {
  /** 前端稳定处方标识。 */
  key: string
  /** 已保存处方 ID。 */
  id?: string
  /** 处方号。 */
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
  lines: ClinicalTcmPrescriptionLine[]
  /** 最近更新时间。 */
  updatedAt?: string
  /** 乐观锁版本。 */
  version?: number
}

/** 中药材目录加载器。 */
export type TcmHerbCatalogLoader = SelectTableLoadPage<TcmHerbCatalogItemVO>

/** 父页面调用公共中医处方工作区的方法。 */
export interface ClinicalTcmPrescriptionWorkspaceExpose {
  /** 保存全部有修改的草稿。 */
  save: (showMessage?: boolean) => Promise<boolean>
  /** 重新加载当前患者处方。 */
  reload: () => Promise<void>
  /** 当前是否存在未保存修改。 */
  isDirty: () => boolean
  /** 校验全部有修改的草稿，成功时返回空字符串。 */
  validate: () => string
}
