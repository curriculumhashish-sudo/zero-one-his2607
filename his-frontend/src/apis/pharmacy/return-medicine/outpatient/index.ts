import type { PatientSummary } from '@/components/patient/type'
import type {
  OutpatientReturnMedicinePatientQueueDTO,
  OutpatientReturnMedicineSlipDTO,
  OutpatientReturnMedicineInitDTO,
  PageOutpatientReturnMedicinePatientQueueParams,
  PageOutpatientReturnMedicinePatientQueueResult,
  OutpatientReturnMedicineRequest,
  OutpatientReturnMedicineResult
} from './type'

import {
  mockLoadOutpatientReturnMedicineInit,
  mockPageOutpatientReturnMedicinePatientQueue,
  mockListOutpatientReturnMedicineSlips,
  mockReturnOutpatientMedicine
} from './mock'

export async function loadOutpatientReturnMedicineInit(): Promise<OutpatientReturnMedicineInitDTO> {
  return mockLoadOutpatientReturnMedicineInit()
}

export async function pageOutpatientReturnMedicinePatientQueue(
  params: PageOutpatientReturnMedicinePatientQueueParams
): Promise<PageOutpatientReturnMedicinePatientQueueResult> {
  return mockPageOutpatientReturnMedicinePatientQueue(params)
}

export async function listOutpatientReturnMedicineSlips(
  encounterId: string
): Promise<OutpatientReturnMedicineSlipDTO[]> {
  return mockListOutpatientReturnMedicineSlips(encounterId)
}

export async function returnOutpatientMedicine(
  request: OutpatientReturnMedicineRequest
): Promise<OutpatientReturnMedicineResult> {
  return mockReturnOutpatientMedicine(request)
}

export function adaptPatientSummary(dto: OutpatientReturnMedicinePatientQueueDTO): PatientSummary {
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