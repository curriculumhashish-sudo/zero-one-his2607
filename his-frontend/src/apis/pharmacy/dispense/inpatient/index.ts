import type { PatientSummary } from '@/components/patient/type'
import type {
  InpatientDispenseInitDto,
  PageInpatientDispenseQueueParams,
  PageInpatientDispenseQueueResult,
  PendingInpatientDispenseQueueRowDto,
  InpatientMedicationDispenseSlipDto,
  InpatientDispensePreparationData,
  InpatientDispenseResultDto,
  PageSummarySlipParams,
  PageSummarySlipResult,
  MedicationSummarySlipDto,
  SummaryDispenseData,
  SummaryDispenseResultDto
} from './type'

import {
  mockLoadInpatientDispenseInit,
  mockPageInpatientDispenseQueue,
  mockPageInpatientMedicationDispenseSlips,
  mockSaveInpatientDispensePreparation,
  mockDispenseInpatientMedications,
  mockPagePendingMedicationSummarySlips,
  mockGetMedicationSummaryDispenseDetail,
  mockDispenseSummaryMedications,
  mockRejectSummaryDispense
} from './mock'

export async function loadInpatientDispenseInit(): Promise<InpatientDispenseInitDto> {
  return mockLoadInpatientDispenseInit()
}

export async function pageInpatientDispenseQueue(params: PageInpatientDispenseQueueParams): Promise<PageInpatientDispenseQueueResult> {
  return mockPageInpatientDispenseQueue(params)
}

export async function pageInpatientMedicationDispenseSlips(encounterId: string): Promise<InpatientMedicationDispenseSlipDto[]> {
  return mockPageInpatientMedicationDispenseSlips(encounterId)
}

export async function saveInpatientDispensePreparation(data: InpatientDispensePreparationData): Promise<void> {
  return mockSaveInpatientDispensePreparation()
}

export async function dispenseInpatientMedications(data: {
  encounterId: string
  dispenserId: string
  items: { dispenseId: string }[]
}): Promise<InpatientDispenseResultDto> {
  return mockDispenseInpatientMedications()
}

export async function pagePendingMedicationSummarySlips(params: PageSummarySlipParams): Promise<PageSummarySlipResult> {
  return mockPagePendingMedicationSummarySlips(params)
}

export async function getMedicationSummaryDispenseDetail(summaryId: string): Promise<MedicationSummarySlipDto | undefined> {
  return mockGetMedicationSummaryDispenseDetail(summaryId)
}

export async function dispenseSummaryMedications(data: SummaryDispenseData): Promise<SummaryDispenseResultDto> {
  return mockDispenseSummaryMedications()
}

export async function rejectSummaryDispense(id: string, reason: string): Promise<SummaryDispenseResultDto> {
  return mockRejectSummaryDispense()
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
