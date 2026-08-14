import type { PageQuery } from '../../type'

export type NurseStationCostPresentationMode = 'DETAIL' | 'SUMMARY'

export interface NurseStationCostDetailQueryDTO extends PageQuery {
  patientId: string
  encounterId: string
  startDate: string
  endDate: string
  typeCode?: string
  busNo?: string
}

export interface NurseStationCostDetailLineVO {
  id: string
  patientId: string
  encounterId: string
  chargeName: string
  typeCode: string
  unitPrice: number
  quantityValue: number
  quantityUnit: string
  totalPrice: number
  baseAmount: number
  discountAmount: number
  occurrenceTime: string
  performingOrgId: string
  statusEnum: string
  busNo: string
}

export interface NurseStationCostSummaryQueryDTO {
  encounterId: string
  startDate: string
  endDate: string
}

export interface NurseStationCostSummaryLineVO {
  typeCode: string
  typeName: string
  totalAmount: number
  count: number
  insuranceAmount: number
  selfPayAmount: number
  summaryDate: string
}
