/** Query conditions supported by the current medical-order execution endpoint. */
export interface NurseStationOrderExecuteQueryDTO {
  /** Selected inpatient encounter IDs. */
  encounterIds: string[]
  /** Page size used for each encounter query. */
  pageSize?: number
}

/** A service request returned by the current execution-list endpoint. */
export interface NurseStationOrderExecuteRowVO {
  /** Stable table row key. */
  listRowKey: string
  /** Service request primary key. */
  id: string
  /** Business number. */
  busNo?: string
  /** Prescription number. */
  prescriptionNo?: string
  /** Current workflow status. */
  statusEnum: string
  /** Service request category code. */
  categoryEnum?: string
  /** Current intent code returned by the backend. */
  intentEnum?: string
  /** Priority code. */
  priorityEnum?: string
  /** Patient primary key. */
  patientId?: string
  /** Inpatient encounter primary key. */
  encounterId: string
  /** Order authored time. */
  authoredTime?: string
  /** Ordered quantity. */
  quantity?: number
  /** Quantity unit code. */
  unitCode?: string
  /** Requesting practitioner primary key. */
  requesterId?: string
  /** Current performer primary key. */
  performerId?: string
  /** Execution location primary key. */
  locationId?: string
  /** Number of execution starts recorded on the request. */
  executionCount?: number
  /** Order remark. */
  remark?: string
  /** Instructions for nursing staff. */
  entrustContent?: string
  /** Status reason returned by the backend. */
  reasonText?: string
}

/** Service requests grouped by inpatient encounter. */
export interface NurseStationOrderExecutePatientGroupVO {
  encounterId: string
  orders: NurseStationOrderExecuteRowVO[]
}

/** Execution-list result used by the nurse-station page. */
export interface NurseStationOrderExecuteListVO {
  groups: NurseStationOrderExecutePatientGroupVO[]
}

/** Batch facade for the backend single-order execution endpoint. */
export interface NurseStationExecuteOrdersDTO {
  /** Logged-in operator ID required by the current backend contract. */
  performerId: string
  /** Selected service request IDs. */
  serviceRequestIds: string[]
}

/** Result for one backend execution request. */
export interface NurseStationOrderExecuteWriteResultVO {
  serviceRequestId: string
  success: boolean
  message: string
}

/** Aggregated result of multiple single-order execution requests. */
export interface NurseStationOrderExecuteBatchResultVO {
  success: boolean
  processedCount: number
  results: NurseStationOrderExecuteWriteResultVO[]
}
