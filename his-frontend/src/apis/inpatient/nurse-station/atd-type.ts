/** 床位看板业务面板。 */
export type NurseStationBedBoardPanel = 'ADMISSION' | 'DISCHARGE'

/** 床位运行状态。 */
export type NurseStationBedOperationalStatus = 'AVAILABLE' | 'OCCUPIED' | 'DISABLED'

/** 床位看板查询条件。 */
export interface NurseStationBedBoardQuery {
  /** 入科或出院面板。 */
  panel: NurseStationBedBoardPanel
  /** 多个病区 ID，以英文逗号分隔。 */
  wardIds?: string
  /** 多个病房 ID，以英文逗号分隔。 */
  houseIds?: string
}

/** 床位位置块。 */
export interface NurseStationBedLocationVO {
  bedId: string
  bedName: string
  wardId: string
  wardName: string
  houseId: string
  houseName: string
  displayOrder: number
  operationalEnum: NurseStationBedOperationalStatus
  /** 并发写入校验版本。 */
  version: number
}

/** 床位占用患者摘要。 */
export interface NurseStationBedOccupantVO {
  encounterId: string
  patientId: string
  patientName: string
  genderEnumText?: string
  ageText?: string
  diagnosisSummary?: string
  statusEnum?: string
  statusEnumText?: string
  priorityEnum?: string
}

/** 床位看板单项。 */
export interface NurseStationBedBoardItemVO {
  bed: NurseStationBedLocationVO
  occupant?: NurseStationBedOccupantVO | null
}

/** 床位看板响应。 */
export interface NurseStationBedBoardVO {
  items: NurseStationBedBoardItemVO[]
}

/** 床位操作类型。 */
export type NurseStationBedOperationType = 'ASSIGN' | 'TRANSFER' | 'SWAP'

/** 入科体征。 */
export interface NurseStationAdmissionVitalSignsDTO {
  temperature?: string
  pulse?: string
  heartRate?: string
  bloodPressureHigh?: string
  bloodPressureLow?: string
  heightCm?: string
  weightKg?: string
}

/** 入科责任医护。 */
export interface NurseStationAdmissionParticipantDTO {
  practitionerId: string
  roleCode: 'ATTENDING_DOCTOR' | 'PRIMARY_NURSE'
}

/** 分床、换床和床位互换请求。 */
export interface NurseStationExecuteBedOperationDTO {
  operationType: NurseStationBedOperationType
  encounterId: string
  targetBedId: string
  targetHouseId: string
  swapEncounterId?: string
  expectedBedVersion?: number
  idempotencyKey: string
  priorityEnum?: string
  participants?: NurseStationAdmissionParticipantDTO[]
  admissionVitalSigns?: NurseStationAdmissionVitalSignsDTO
}

/** 床位操作结果。 */
export interface NurseStationBedOperationResultVO {
  encounterId: string
  operationType: NurseStationBedOperationType
  succeeded: boolean
}

/** 出院前待执行医嘱。 */
export interface NurseStationDischargePendingExecutionVO {
  requestId: string
  therapyEnum?: 'LONG_TERM' | 'TEMPORARY'
  reqAuthoredTime?: string
  displayName?: string
  doseText?: string
  rateCode?: string
  blockingReason?: string
}

/** 出院前待发或待退药。 */
export interface NurseStationDischargePendingMedicationVO {
  dispenseId: string
  medicationName?: string
  statusEnum?: string
  statusEnumText?: string
  quantity?: number
  unitCode?: string
  performLocationName?: string
  plannedDispenseTime?: string
}

/** 住院状态转换请求。 */
export interface NurseStationEncounterTransitionDTO {
  encounterId: string
}

/** 住院状态转换结果。 */
export interface NurseStationEncounterTransitionResultVO {
  encounterId: string
  succeeded: boolean
  previousStatusEnum?: string
  targetStatusEnum?: string
  bedReleased?: boolean
  requiresBedAssignment?: boolean
}
