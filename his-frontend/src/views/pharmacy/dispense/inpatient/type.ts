import type { InpatientMedicationDispenseSlipDto, InpatientDispenseItemDto, MedicationSummarySlipDto } from '@/apis/pharmacy/dispense/inpatient/type'

export interface DispensePrescriptionBlock {
  slipId: string
  rxNo: string
  rxType: 'western' | 'herbal'
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  items: DispensePrescriptionItemRow[]
}

export interface DispensePrescriptionItemRow {
  key: string
  rxNo: string
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  itemName: string
  quantity: number
  spec: string
  batchNo: string
  totalPrice: number
  pharmacy: string
  traceCode: string
  factory: string
  sourceStatus: string
  dispenseId: string
}

export type DispenseMode = 'patient' | 'summary'

export interface SummarySlipRow {
  key: string
  summaryNo: string
  status: 'pending' | 'completed' | 'rejected'
  receiverName: string
  createTime: string
  summaryId: string
}
