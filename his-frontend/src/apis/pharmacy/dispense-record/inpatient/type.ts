export interface InpatientDispenseRecordInitDTO {
  pharmacyOptions: { label: string; value: string }[]
  categoryOptions: { label: string; value: string }[]
  dispenserOptions: { label: string; value: string }[]
  recordTypeOptions: { label: string; value: string }[]
  defaultDateRange: [string, string]
}

export interface InpatientDispenseRecordItemDTO {
  id: string
  recordId: string
  prescriptionNo: string
  recordType: 'DISPENSE' | 'REFUND'
  patientId: string
  patientName: string
  inpatientNo: string
  departmentName: string
  wardName: string
  bedNo: string
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

export interface PageInpatientDispenseRecordsParams {
  startDate?: string
  endDate?: string
  recordType?: string
  pharmacyId?: string
  category?: string
  dispenserId?: string
  drugKeyword?: string
  patientKeyword?: string
  pageIndex: number
  pageSize: number
}

export interface PageInpatientDispenseRecordsResult {
  records: InpatientDispenseRecordItemDTO[]
  total: number
  pageIndex: number
  pageSize: number
}

export interface ExportInpatientDispenseRecordsResult {
  success: boolean
  message: string
  url?: string
}