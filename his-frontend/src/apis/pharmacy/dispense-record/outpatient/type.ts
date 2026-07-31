export interface OutpatientDispenseRecordInitDTO {
  pharmacyOptions: { label: string; value: string }[]
  categoryOptions: { label: string; value: string }[]
  dispenserOptions: { label: string; value: string }[]
  recordTypeOptions: { label: string; value: string }[]
  defaultDateRange: [string, string]
}

export interface OutpatientDispenseRecordItemDTO {
  id: string
  recordId: string
  prescriptionNo: string
  recordType: 'DISPENSE' | 'REFUND'
  patientId: string
  patientName: string
  outpatientNo: string
  departmentName: string
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

export interface PageOutpatientDispenseRecordsParams {
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

export interface PageOutpatientDispenseRecordsResult {
  records: OutpatientDispenseRecordItemDTO[]
  total: number
  pageIndex: number
  pageSize: number
}

export interface ExportOutpatientDispenseRecordsResult {
  success: boolean
  message: string
  url?: string
}