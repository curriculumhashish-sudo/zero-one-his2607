import type { PatientSummary, PatientStatusTone } from '@/components/patient/type'
import type { PendingDispenseEncounterQueueRowDto } from './type'

export function adaptPatientSummary(dto: PendingDispenseEncounterQueueRowDto): PatientSummary {
  const statusTone: PatientStatusTone = dto.dispenseStatus === 'PREPARATION' ? 'warning' : 'success'
  return {
    id: dto.encounterId,
    name: dto.patientName,
    gender: dto.genderText,
    age: dto.ageText,
    serviceType: dto.serviceType,
    patientNo: dto.patientId,
    visitNo: dto.visitNo,
    visitTime: dto.registerTime,
    departmentName: dto.departmentName,
    status: dto.dispenseStatus === 'PREPARATION' ? '待发药' : '已发药',
    statusTone,
    paymentType: dto.paymentType
  }
}
