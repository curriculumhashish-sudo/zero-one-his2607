export interface InpatientDispenseInitDto {
  dispenserOptions: { label: string; value: string }[]
  receiverOptions: { label: string; value: string }[]
}

export interface PendingInpatientDispenseQueueRowDto {
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

export interface PageInpatientDispenseQueueParams {
  status?: 'pending' | 'completed'
  keyword?: string
  dateRange?: string[]
  pageNum?: number
  pageSize?: number
}

export interface PageInpatientDispenseQueueResult {
  records: PendingInpatientDispenseQueueRowDto[]
  total: number
  pageNum: number
  pageSize: number
}

export interface InpatientMedicationDispenseSlipDto {
  slipId: string
  rxNo: string
  rxType: 'western' | 'herbal'
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  items: InpatientDispenseItemDto[]
}

export interface InpatientDispenseItemDto {
  dispenseId: string
  itemName: string
  spec: string
  quantity: number
  unit: string
  batchNo: string
  totalPrice: number
  pharmacy: string
  traceCode: string
  factory: string
  sourceStatus: string
  status: 'pending' | 'partial' | 'completed' | 'blocking'
}

export interface InpatientDispensePreparationData {
  summaryNo: string
  items: {
    dispenseId: string
    dispenseQuantity: number
    lotNumber?: string
  }[]
  practitionerId?: string
  recPractitionerId?: string
}

export interface InpatientDispenseResultDto {
  success: boolean
  message: string
}

export interface DispenseRejectData {
  summaryNo: string
  rejectReason: string
}

export interface MedicationSummarySlipDto {
  summaryId: string
  summaryNo: string
  status: 'pending' | 'completed' | 'rejected'
  receiverName: string
  receiverId: string
  createTime: string
  items: InpatientDispenseItemDto[]
}

export interface PageSummarySlipParams {
  status?: 'pending' | 'completed' | 'rejected'
  keyword?: string
  dateRange?: string[]
  pageNum?: number
  pageSize?: number
}

export interface PageSummarySlipResult {
  records: MedicationSummarySlipDto[]
  total: number
  pageNum: number
  pageSize: number
}

export interface SummaryDispenseData {
  summaryNo: string
  items: {
    dispenseId: string
    dispenseQuantity: number
    lotNumber?: string
  }[]
  practitionerId?: string
  recPractitionerId?: string
}

export interface SummaryDispenseResultDto {
  success: boolean
  message: string
}
