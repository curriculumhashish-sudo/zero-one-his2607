import type { PatientSummary } from '@/components/patient/type'
import type {
  OutpatientReturnMedicinePatientQueueDTO,
  OutpatientReturnMedicineSlipDTO,
  OutpatientReturnMedicineItemDTO,
  OutpatientReturnMedicineResult
} from './type'

interface BackendReturnQueueRow {
  id?: string
  patient_name?: string
  patient_no?: string
  visit_no?: string
  department_name?: string
  doctor_name?: string
  visit_time?: string
  service_type?: string
  payment_type?: string
  refund_status?: string
  status?: string
}

interface BackendReturnDetailResponse {
  returnId?: string
  drugList?: {
    drugId?: string
    drug_name?: string
    quantity?: number | string
    dispense_date?: string
  }[]
}

export function adaptBackendReturnQueueRow(
  row: BackendReturnQueueRow
): OutpatientReturnMedicinePatientQueueDTO {
  const statusMap: Record<string, 'pending' | 'completed' | 'partial'> = {
    pending: 'pending',
    completed: 'completed',
    partial: 'partial',
    PENDING: 'pending',
    COMPLETED: 'completed',
    PARTIAL: 'partial'
  }
  const rawStatus = (row.refund_status || row.status || 'pending') as string
  const status = statusMap[rawStatus] || 'pending'

  const statusToneMap: Record<string, 'warning' | 'success' | 'info'> = {
    pending: 'warning',
    completed: 'success',
    partial: 'info'
  }

  return {
    id: row.id || '',
    encounterId: row.id || '',
    patientId: '',
    patientName: row.patient_name || '',
    patientNo: row.patient_no || '',
    visitNo: row.visit_no || '',
    departmentName: row.department_name || '',
    doctorName: row.doctor_name || '',
    status,
    statusTone: statusToneMap[status] || 'info',
    visitTime: row.visit_time || '',
    serviceType: row.service_type || '',
    paymentType: row.payment_type || ''
  }
}

export function adaptBackendReturnDetailResponse(
  response: BackendReturnDetailResponse,
  encounterId: string
): OutpatientReturnMedicineSlipDTO {
  const items: OutpatientReturnMedicineItemDTO[] = (response.drugList || []).map((d, idx) => ({
    itemId: d.drugId || `ITEM_${idx}`,
    slipId: response.returnId || encounterId,
    drugId: d.drugId || '',
    itemName: d.drug_name || '',
    spec: '',
    dosage: '',
    route: '',
    frequency: '',
    days: 0,
    quantity: Number(d.quantity) || 0,
    unit: '',
    price: 0,
    totalAmount: 0,
    status: 'pending',
    batchNo: '',
    traceCode: '',
    factory: ''
  }))

  return {
    slipId: response.returnId || encounterId,
    encounterId,
    patientId: '',
    patientName: '',
    patientNo: '',
    visitNo: '',
    doctorName: '',
    departmentName: '',
    createTime: response.drugList?.[0]?.dispense_date || '',
    status: 'pending',
    items
  }
}

export function adaptReturnResult(success: boolean, message: string): OutpatientReturnMedicineResult {
  return { success, message }
}

export function adaptPatientSummary(
  dto: OutpatientReturnMedicinePatientQueueDTO
): PatientSummary {
  const statusMap: Record<string, string> = {
    pending: '待退药',
    partial: '部分退药',
    completed: '已退药'
  }

  const statusToneMap: Record<string, 'warning' | 'success' | 'info'> = {
    pending: 'warning',
    partial: 'info',
    completed: 'success'
  }

  return {
    id: dto.id,
    name: dto.patientName,
    gender: '',
    age: '',
    serviceType: dto.serviceType,
    patientNo: dto.patientNo,
    visitNo: dto.visitNo,
    visitTime: dto.visitTime,
    departmentName: dto.departmentName,
    status: statusMap[dto.status] || dto.status,
    statusTone: statusToneMap[dto.status] || 'info',
    paymentType: dto.paymentType
  }
}