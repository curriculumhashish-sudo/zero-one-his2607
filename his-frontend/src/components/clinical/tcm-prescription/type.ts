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
  /** Optional method dictionary supplied by the doctor-station page. */
  methodOptions?: Array<{ code: string; name: string }>
  /** Optional frequency dictionary supplied by the doctor-station page. */
  rateOptions?: Array<{ code: string; name: string }>
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
  /** Backend group key used for both drafts and signed records. */
  prescriptionGroupKey: string
  /** True after the record has been accepted by the backend. */
  saved: boolean
  /** Backend prescription number, available after signing. */
  id?: string
  prescriptionNo?: string
  version?: number
  encounterDiagnosisId?: string
  usage?: string
  frequency?: string
  therapyEnum?: string
  tcmSyndromeGroup?: string
  methodCode?: string
  methodName?: string
  rateCode?: string
  rateName?: string
  syndromeName?: string
  doseCount: number
  days: number
  decoctionMethod?: string
  remark?: string
  status: TcmPrescriptionStatus
  statusName: string
  lines: ClinicalTcmPrescriptionLine[]
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
