import type { PatientSummary } from '@/components/patient/type'
import type {
  InpatientReturnPatientQueueRowDto,
  InpatientReturnMedicineDetailDto,
  InpatientReturnMedicineItemDto,
  InpatientReturnMedicineResultDto
} from './type'

interface BackendReturnPatientRow {
  name?: string
  gender?: string
  age?: number | string
  hospitalizationNo?: string
  ward?: string
  bedNo?: string
  refundStatus?: string
}

interface BackendReturnDetailItem {
  detailId?: string
  serialNo?: number
  drugName?: string
  quantity?: string | number
  dispenseDate?: string
}

interface BackendReturnQueryResponse {
  patientInfo?: {
    name?: string
    gender?: string
    age?: number | string
    hospitalizationNo?: string
    ward?: string
    bedNo?: string
    refundStatus?: string
  }
  drugList?: BackendReturnDetailItem[]
}

export function adaptBackendReturnPatientRow(row: BackendReturnPatientRow): InpatientReturnPatientQueueRowDto {
  const statusMap: Record<string, 'pending' | 'completed'> = {
    pending: 'pending',
    completed: 'completed',
    PENDING: 'pending',
    COMPLETED: 'completed'
  }
  const rawStatus = (row.refundStatus || 'pending') as string
  const status = statusMap[rawStatus] || 'pending'

  return {
    encounterId: row.hospitalizationNo || '',
    patientName: row.name || '',
    gender: row.gender || '',
    age: String(row.age || ''),
    serviceType: '',
    patientNo: row.hospitalizationNo || '',
    visitNo: '',
    visitTime: '',
    departmentName: row.ward || '',
    status,
    paymentType: '',
    bedNo: row.bedNo || ''
  }
}

export function adaptBackendReturnQueryResponse(
  response: BackendReturnQueryResponse
): InpatientReturnMedicineDetailDto {
  const info = response.patientInfo || {}
  const items: InpatientReturnMedicineItemDto[] = (response.drugList || []).map((d, idx) => ({
    itemId: d.detailId || `ITEM_${idx}`,
    itemName: d.drugName || '',
    spec: '',
    quantity: Number(d.quantity) || 0,
    unit: '',
    batchNo: '',
    totalPrice: 0,
    pharmacy: '',
    traceCode: '',
    factory: '',
    status: 'refundable',
    refundQuantity: Number(d.quantity) || 0,
    originalQuantity: Number(d.quantity) || 0
  }))

  return {
    encounterId: info.hospitalizationNo || '',
    patientName: info.name || '',
    patientNo: info.hospitalizationNo || '',
    bedNo: info.bedNo || '',
    items
  }
}

export function adaptConfirmResult(success: boolean, message: string): InpatientReturnMedicineResultDto {
  return {
    success,
    message,
    results: []
  }
}

export function adaptPatientSummary(dto: InpatientReturnPatientQueueRowDto): PatientSummary {
  return {
    id: dto.encounterId,
    name: dto.patientName,
    gender: dto.gender,
    age: dto.age,
    serviceType: `${dto.serviceType} ${dto.bedNo || ''}`.trim(),
    patientNo: dto.patientNo,
    visitNo: dto.visitNo,
    visitTime: dto.visitTime,
    departmentName: dto.departmentName,
    status: dto.status === 'pending' ? '待退药' : '已退药',
    statusTone: dto.status === 'pending' ? 'warning' : 'success',
    paymentType: dto.paymentType
  }
}