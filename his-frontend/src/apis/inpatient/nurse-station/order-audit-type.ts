/** Verification status supported by the current backend. */
export type NurseStationOrderAuditStatus = 'PENDING' | 'PASSED'

/** Medical order request types supported by the current backend. */
export type NurseStationOrderAuditOrderType = 'MED_REQUEST' | 'SERVICE_REQUEST'

/** Patient context required by the legacy name-based query endpoint. */
export interface NurseStationOrderAuditPatientContextDTO {
  encounterId: string
  patientId: string
  patientName: string
}

/** Query parameters used by the nurse station verification page. */
export interface NurseStationOrderAuditQueryDTO {
  patientContexts: NurseStationOrderAuditPatientContextDTO[]
  auditStatus: NurseStationOrderAuditStatus
}

/** Minimal row model backed by the current verification response. */
export interface NurseStationOrderAuditRowVO {
  listRowKey: string
  requestId: string
  orderType: NurseStationOrderAuditOrderType
  content: string
}

/** Medical orders grouped under one selected encounter. */
export interface NurseStationOrderAuditPatientGroupVO {
  encounterId: string
  orders: NurseStationOrderAuditRowVO[]
}

/** Verification list response used by the page. */
export interface NurseStationOrderAuditListVO {
  groups: NurseStationOrderAuditPatientGroupVO[]
}

/** Minimal item accepted by the current backend write endpoints. */
export interface NurseStationOrderAuditWriteItemDTO {
  orderType: NurseStationOrderAuditOrderType
  requestId: string
}

/** Batch verification request. */
export interface NurseStationVerifyOrdersDTO {
  items: NurseStationOrderAuditWriteItemDTO[]
}

/** Batch rejection request. */
export interface NurseStationRejectOrdersDTO extends NurseStationVerifyOrdersDTO {
  reason: string
}

/** Aggregate result returned by the current backend. */
export interface NurseStationOrderAuditBatchResultVO {
  success: boolean
  processedCount: number
}
