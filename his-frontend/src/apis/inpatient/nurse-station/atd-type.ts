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
  operationalEnum: NurseStationBedOperationalStatus
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

/** 床位预约请求。 */
export interface NurseStationAssignBedDTO {
  encounterId: string
  patientId: string
  intendedAdmissionDate?: string
  targetBedId: string
}

/** 床位预约结果。 */
export interface NurseStationAssignBedResultVO {
  encounterId: string
  reservationId: string
  succeeded: boolean
}

/** 住院状态转换请求。 */
export interface NurseStationEncounterTransitionDTO {
  encounterId: string
}

/** 住院状态转换结果。 */
export interface NurseStationEncounterTransitionResultVO {
  encounterId: string
  succeeded: boolean
}
