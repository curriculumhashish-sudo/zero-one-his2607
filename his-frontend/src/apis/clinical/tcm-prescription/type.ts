import type { PageQuery } from '../../type'

/** Doctor-station context used by the shared mock and HTTP adapters. */
export type TcmPrescriptionStationScope = 'outpatient' | 'inpatient'

/** Frontend status projection of the backend prescription states. */
export type TcmPrescriptionStatus =
  | 'DRAFT'
  | 'UNSIGNED'
  | 'ACTIVE'
  | 'SIGNED'
  | 'COMPLETED'
  | 'STOPPED'

/** Minimal herb row required to create a prescription line. */
export interface TcmHerbCatalogItemVO extends Record<string, unknown> {
  id: string
  name: string
  defaultDose: number
  doseUnit: string
  retailUnitPrice?: number
  doseUnitCode?: string
  /** Legacy mock-only display fields. */
  code?: string
  specification?: string
  retailPrice?: number
}

export interface TcmHerbCatalogQuery extends PageQuery {
  scope: TcmPrescriptionStationScope
  keyword?: string
}

/** Medication-request detail returned by the current-prescription endpoint. */
export interface TcmPrescriptionLineVO {
  id?: string
  medicationRequestId?: string
  medicationDefinitionId?: string
  catalogId?: string
  name: string
  dose: number
  doseUnit: string
  retailUnitPrice?: number
  /** Legacy mock-only display fields. */
  code?: string
  specification?: string
  retailPrice?: number
  orderNo?: number
}

/** Prescription header and details used by the inpatient workspace. */
export interface TcmPrescriptionVO {
  /** Backend prescription number; drafts use prescriptionGroupKey instead. */
  id?: string
  scope: TcmPrescriptionStationScope
  encounterId: string
  patientId: string
  prescriptionGroupKey?: string
  prescriptionNo?: string
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
  lines: TcmPrescriptionLineVO[]
  /** Legacy mock-only fields retained for local fixtures. */
  encounterDiagnosisId?: string
  method?: string
  frequency?: string
  authoredAt?: string
  updatedAt?: string
  version?: number
}

export interface TcmPrescriptionQuery {
  scope: TcmPrescriptionStationScope
  encounterId: string
  patientId: string
}

export interface TcmPrescriptionHistoryVO {
  id: string
  visitId?: string
  visitTime?: string
  visitType?: string
  mainDiagnosis?: string
  doctorName?: string
  drugCount?: number
  examCount?: number
  materialCount?: number
  encounterId: string
  patientId: string
  authoredAt?: string
  prescriptionNo: string
  summary: string
  herbNames: string[]
}

export interface TcmPrescriptionHistoryDetailVO {
  id: string
  name: string
  quantity: number
  executedCount: number
  createTime?: string
  diagnosis?: string
  doctorName?: string
}

export interface TcmPrescriptionHistoryQuery {
  scope: TcmPrescriptionStationScope
  patientId: string
  currentEncounterId: string
  keyword?: string
}

/** One line accepted by the backend prescription-add DTO. */
export interface SaveTcmPrescriptionLineDTO {
  medicineId: string
  quantity: number
  unit?: string
  dose?: number
  doseUnit?: string
  doseUnitCode?: string
  /** Legacy mock-only field. */
  sortNo?: number
}

/** New prescription payload. Existing-record updates are not supported by the MVP backend. */
export interface SaveTcmPrescriptionDTO {
  scope: TcmPrescriptionStationScope
  encounterId: string
  patientId: string
  prescriptionGroupKey?: string
  therapyEnum?: string
  tcmSyndromeGroup?: string
  methodCode?: string
  rateCode?: string
  chineseHerbsDoseQuantity?: number
  sufferingFlag?: boolean
  days: number
  remark?: string
  details: SaveTcmPrescriptionLineDTO[]
  /** Backend identity/version fields used by the shared mock adapter. */
  id?: string
  version?: number
  encounterDiagnosisId?: string
  copies?: number
  usage?: string
  frequency?: string
  decoction?: number
  /** Legacy inpatient/mock fields; outpatient real requests omit them. */
  diagnosisName?: string
}

/** Request fields for delete, sign, and withdraw operations. */
export interface TcmPrescriptionActionDTO {
  scope: TcmPrescriptionStationScope
  encounterId: string
  patientId?: string
  tenantId?: string
  id?: string
  prescriptionNo?: string
  prescriptionGroupKey?: string
  medicationRequestIds?: string[]
  prescriptionNos?: string[]
  stopTime?: string
  /** Legacy mock-only field. */
  version?: number
}
