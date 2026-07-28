import type { PatientSummary } from '@/components/patient/type'
import type {
  InpatientReturnMedicineInitDto,
  PageInpatientReturnPatientQueueParams,
  PageInpatientReturnPatientQueueResult,
  InpatientReturnPatientQueueRowDto,
  InpatientReturnMedicineDetailDto,
  ConfirmInpatientMedicineReturnData,
  InpatientReturnMedicineResultDto,
  RejectInpatientMedicineReturnData
} from './type'

import {
  mockLoadInpatientReturnMedicineInit,
  mockPageInpatientReturnPatientQueue,
  mockGetInpatientReturnMedicineDetail,
  mockConfirmInpatientMedicineReturn,
  mockRejectInpatientMedicineReturn
} from './mock'

export async function loadInpatientReturnMedicineInit(): Promise<InpatientReturnMedicineInitDto> {
  return mockLoadInpatientReturnMedicineInit()
}

export async function pageInpatientReturnPatientQueue(params: PageInpatientReturnPatientQueueParams): Promise<PageInpatientReturnPatientQueueResult> {
  return mockPageInpatientReturnPatientQueue(params)
}

export async function getInpatientReturnMedicineDetail(encounterId: string): Promise<InpatientReturnMedicineDetailDto | undefined> {
  return mockGetInpatientReturnMedicineDetail(encounterId)
}

export async function confirmInpatientMedicineReturn(data: ConfirmInpatientMedicineReturnData): Promise<InpatientReturnMedicineResultDto> {
  return mockConfirmInpatientMedicineReturn()
}

export async function rejectInpatientMedicineReturn(data: RejectInpatientMedicineReturnData): Promise<InpatientReturnMedicineResultDto> {
  return mockRejectInpatientMedicineReturn()
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
