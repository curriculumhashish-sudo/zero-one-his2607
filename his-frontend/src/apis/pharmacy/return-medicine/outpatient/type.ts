export interface OutpatientReturnMedicinePatientQueueDTO {
  id: string
  encounterId: string
  patientId: string
  patientName: string
  patientNo: string
  visitNo: string
  departmentName: string
  doctorName: string
  status: 'pending' | 'completed' | 'partial'
  statusTone: 'warning' | 'success' | 'info'
  visitTime: string
  serviceType: string
  paymentType: string
}

export interface PageOutpatientReturnMedicinePatientQueueResult {
  records: OutpatientReturnMedicinePatientQueueDTO[]
  total: number
  pageIndex: number
  pageSize: number
}

export interface OutpatientReturnMedicineSlipDTO {
  slipId: string
  encounterId: string
  patientId: string
  patientName: string
  patientNo: string
  visitNo: string
  doctorName: string
  departmentName: string
  createTime: string
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  items: OutpatientReturnMedicineItemDTO[]
}

export interface OutpatientReturnMedicineItemDTO {
  itemId: string
  slipId: string
  drugId: string
  itemName: string
  spec: string
  dosage: string
  route: string
  frequency: string
  days: number
  quantity: number
  unit: string
  price: number
  totalAmount: number
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  batchNo: string
  traceCode: string
  factory: string
  reason?: string
}

export interface OutpatientReturnMedicineInitDTO {
  dispenserOptions: { label: string; value: string }[]
}

export interface OutpatientReturnMedicineRequest {
  encounterId: string
  dispenserId: string
  items: { itemId: string; reason: string }[]
}

export interface OutpatientReturnMedicineResult {
  success: boolean
  message: string
}

export interface PageOutpatientReturnMedicinePatientQueueParams {
  status?: string
  keyword?: string
  dateRange?: [string, string]
}