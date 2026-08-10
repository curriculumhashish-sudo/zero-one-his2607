import type { PageQuery } from '../../type'

/** Medicine category used by the nurse-station page. */
export type NurseStationDispenseMedicineCategory = 'TCM' | 'WEST'

/** Long-term or temporary order filter used by the page. */
export type NurseStationDispenseTherapyFilter = 'ALL' | 'LONG' | 'TEMP'

/** Medication-dispense status returned by the current backend. */
export type NurseStationDispenseSummaryStatus = 'DISPENSED' | 'SUMMARIZED' | 'WAIT_REVIEW'

/** Query for selectable dispense time points. */
export interface NurseStationDispensePendingQueryDTO extends PageQuery {
  encounterIds: string[]
  medicineCategory: NurseStationDispenseMedicineCategory
  therapyFilter: NurseStationDispenseTherapyFilter
  queryDeadline: string
  keyword?: string
}

/** A selectable planned dispense time point. */
export interface NurseStationDispenseTimePointVO {
  dispenseId: string
  procedureId: string
  plannedDispenseTime: string
  quantity: number
  unitCode: string
  unitCodeText: string
  chargeUnitPrice: number
}

/** A medication order row in the pending list. */
export interface NurseStationDispenseOrderRowVO {
  medicationRequestId: string
  medicationId: string
  medicationName: string
  specificationText: string
  therapyType: 'LONG' | 'TEMP'
  therapyTypeText: string
  dosageInstruction: string
  methodText: string
  frequencyText: string
  pharmacyId: string
  pharmacyName: string
  timePoints: NurseStationDispenseTimePointVO[]
}

/** Pending medication orders grouped by patient. */
export interface NurseStationDispensePatientGroupVO {
  encounterId: string
  patientId: string
  patientName: string
  bedDisplay: string
  orderRows: NurseStationDispenseOrderRowVO[]
}

/** Page result for selectable dispense time points. */
export interface NurseStationDispensePendingResultVO {
  pageIndex: number
  pageSize: number
  total: number
  patientGroups: NurseStationDispensePatientGroupVO[]
}

/** A selected dispense record used to create summary slips. */
export interface NurseStationCreateDispenseItemDTO {
  dispenseId: string
  encounterId: string
}

/** Create-summary request supported by the backend. */
export interface NurseStationCreateDispenseSummaryDTO {
  dispenseItems: NurseStationCreateDispenseItemDTO[]
}

/** Create-summary response supported by the backend. */
export interface NurseStationCreateDispenseSummaryResultVO {
  summaryNos: string[]
  message: string
}

/** Query for generated summary slips. */
export interface NurseStationDispenseSummaryQueryDTO extends PageQuery {
  statusEnum?: NurseStationDispenseSummaryStatus
  dispenseDate?: string
}

/** Summary-slip row returned by the current backend. */
export interface NurseStationDispenseSummaryHeaderVO {
  summaryNo: string
  statusEnum: NurseStationDispenseSummaryStatus
  desLocationId: string
  itemCount: number
  plannedDispenseTime: string
  dispenseTime?: string
}

/** A raw medication-dispense detail row. */
export interface NurseStationDispenseSummaryLineVO {
  id: string
  name: string
  quantity: number
  unitCode: string
}

/** Batch request used by submit and cancel endpoints. */
export interface NurseStationDispenseSummaryNosDTO {
  summaryNos: string[]
}

export type NurseStationSubmitDispenseSummariesDTO = NurseStationDispenseSummaryNosDTO
export type NurseStationCancelDispenseSummariesDTO = NurseStationDispenseSummaryNosDTO

/** Batch response used by create, submit, and cancel endpoints. */
export interface NurseStationDispenseSummaryWriteResultVO {
  summaryNos: string[]
  message: string
}
