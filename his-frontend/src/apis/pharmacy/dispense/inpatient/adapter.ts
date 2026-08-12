import type { PatientSummary } from '@/components/patient/type'
import type {
  PendingInpatientDispenseQueueRowDto,
  InpatientMedicationDispenseSlipDto,
  InpatientDispenseItemDto,
  MedicationSummarySlipDto,
  InpatientDispenseResultDto,
  SummaryDispenseResultDto
} from './type'

interface BackendSummaryRow {
  id?: string
  summaryNo?: string
  summary_status?: string
  createTime?: string
  pharmacy?: string
  applicant?: string
  summaryStatus?: string
}

interface BackendDrugDetailItem {
  detailId?: string
  serialNo?: number
  drugName?: string
  quantity?: string | number
  specification?: string
}

interface BackendDrugDetailResponse {
  summaryNo?: string
  ward?: string
  createTime?: string
  pharmacy?: string
  applicant?: string
  drugList?: BackendDrugDetailItem[]
}

export function adaptBackendSummaryRow(row: BackendSummaryRow): PendingInpatientDispenseQueueRowDto {
  const statusMap: Record<string, 'pending' | 'completed'> = {
    pending: 'pending',
    completed: 'completed',
    rejected: 'pending'
  }
  const rawStatus = (row.summaryStatus || row.summary_status || 'pending') as string
  const status = statusMap[rawStatus] || 'pending'

  return {
    encounterId: row.id || row.summaryNo || '',
    patientName: '',
    gender: '',
    age: '',
    serviceType: '',
    patientNo: '',
    visitNo: row.summaryNo || '',
    visitTime: row.createTime || '',
    departmentName: row.pharmacy || '',
    status,
    paymentType: '',
    bedNo: ''
  }
}

export function adaptBackendSummaryRowToSlip(row: BackendSummaryRow): MedicationSummarySlipDto {
  const statusMap: Record<string, 'pending' | 'completed' | 'rejected'> = {
    pending: 'pending',
    completed: 'completed',
    rejected: 'rejected'
  }
  const rawStatus = (row.summaryStatus || row.summary_status || 'pending') as string

  return {
    summaryId: row.id || '',
    summaryNo: row.summaryNo || '',
    status: statusMap[rawStatus] || 'pending',
    receiverName: row.applicant || '',
    receiverId: '',
    createTime: row.createTime || '',
    items: []
  }
}

export function adaptBackendDrugDetailResponse(
  response: BackendDrugDetailResponse,
  encounterId: string,
  patientName: string
): InpatientMedicationDispenseSlipDto {
  const items: InpatientDispenseItemDto[] = (response.drugList || []).map((d, idx) => ({
    dispenseId: d.detailId || `DISP_${idx}`,
    itemName: d.drugName || '',
    spec: d.specification || '',
    quantity: Number(d.quantity) || 0,
    unit: '',
    batchNo: '',
    totalPrice: 0,
    pharmacy: response.pharmacy || '',
    traceCode: '',
    factory: '',
    sourceStatus: '',
    status: 'pending'
  }))

  return {
    slipId: response.summaryNo || encounterId,
    rxNo: response.summaryNo || '',
    rxType: 'western',
    status: 'pending',
    items
  }
}

export function adaptBackendDrugDetailToItems(
  response: BackendDrugDetailResponse
): InpatientDispenseItemDto[] {
  return (response.drugList || []).map((d, idx) => ({
    dispenseId: d.detailId || `DISP_${idx}`,
    itemName: d.drugName || '',
    spec: d.specification || '',
    quantity: Number(d.quantity) || 0,
    unit: '',
    batchNo: '',
    totalPrice: 0,
    pharmacy: response.pharmacy || '',
    traceCode: '',
    factory: '',
    sourceStatus: '',
    status: 'pending'
  }))
}

export function adaptPatientSummary(dto: PendingInpatientDispenseQueueRowDto): PatientSummary {
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
    status: dto.status === 'pending' ? '待发药' : '已发药',
    statusTone: dto.status === 'pending' ? 'warning' : 'success',
    paymentType: dto.paymentType
  }
}

export function adaptVerifyResult(success: boolean, message: string): InpatientDispenseResultDto {
  return {
    success,
    message
  }
}

export function adaptSummaryDispenseResult(success: boolean, message: string): SummaryDispenseResultDto {
  return { success, message }
}
