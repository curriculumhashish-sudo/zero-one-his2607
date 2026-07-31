export type DispenseRecordScope = 'outpatient' | 'inpatient'

export type DisplayMode = 'prescription' | 'encounter' | 'flat'

export type RecordType = 'DISPENSE' | 'REFUND'

export interface DispenseRecordItem {
  id: string
  recordId: string
  prescriptionNo: string
  recordType: RecordType
  recordTypeLabel: string
  patientId: string
  patientName: string
  visitNo: string
  departmentName: string
  wardName?: string
  bedNo?: string
  dispenseTime: string
  pharmacyName: string
  dispenserName: string
  drugName: string
  spec: string
  quantity: number
  unit: string
  price: number
  amount: number
  batchNo: string
  traceCode: string
  dosage: string
  summaryNo: string
}

export interface SearchFormState {
  startDate?: string
  endDate?: string
  recordType?: string
  pharmacyId?: string
  category?: string
  dispenserId?: string
  drugKeyword?: string
  patientKeyword?: string
}

export interface GroupedPrescription {
  key: string
  prescriptionNo: string
  recordType: RecordType
  recordTypeLabel: string
  patientName: string
  visitNo: string
  departmentName: string
  dispenseTime: string
  pharmacyName: string
  dispenserName: string
  summaryNo: string
  items: DispenseRecordItem[]
}

export interface GroupedEncounter {
  key: string
  visitNo: string
  patientName: string
  departmentName: string
  wardName?: string
  bedNo?: string
  items: DispenseRecordItem[]
}

export interface InitOptions {
  pharmacyOptions: { label: string; value: string }[]
  categoryOptions: { label: string; value: string }[]
  dispenserOptions: { label: string; value: string }[]
  recordTypeOptions: { label: string; value: string }[]
  defaultDateRange: [string, string]
}