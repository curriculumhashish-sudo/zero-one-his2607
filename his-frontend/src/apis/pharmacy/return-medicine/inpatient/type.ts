export interface InpatientReturnMedicineInitDto {
  insuranceContractOptions: { label: string; value: string }[]
  wardOptions: { label: string; value: string }[]
}

export interface InpatientReturnPatientQueueRowDto {
  encounterId: string
  patientName: string
  gender: string
  age: string
  serviceType: string
  patientNo: string
  visitNo: string
  visitTime: string
  departmentName: string
  status: 'pending' | 'completed'
  paymentType: string
  bedNo: string
}

export interface PageInpatientReturnPatientQueueParams {
  status?: 'pending' | 'completed'
  keyword?: string
  dateRange?: string[]
  pageNum?: number
  pageSize?: number
}

export interface PageInpatientReturnPatientQueueResult {
  records: InpatientReturnPatientQueueRowDto[]
  total: number
  pageNum: number
  pageSize: number
}

export interface InpatientReturnMedicineItemDto {
  itemId: string
  itemName: string
  spec: string
  quantity: number
  unit: string
  batchNo: string
  totalPrice: number
  pharmacy: string
  traceCode: string
  factory: string
  status: 'refundable' | 'partially-refundable' | 'non-refundable' | 'refunded'
  refundQuantity: number
  originalQuantity: number
}

export interface InpatientReturnMedicineDetailDto {
  encounterId: string
  patientName: string
  patientNo: string
  bedNo: string
  items: InpatientReturnMedicineItemDto[]
}

export interface ConfirmInpatientMedicineReturnData {
  encounterId: string
  items: {
    itemId: string
    quantity: number
  }[]
}

export interface InpatientReturnMedicineResultDto {
  success: boolean
  message: string
  results: {
    itemId: string
    success: boolean
    message: string
  }[]
}

export interface RejectInpatientMedicineReturnData {
  encounterId: string
  reason: string
}
