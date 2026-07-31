import type {
  NurseExecutionStatus,
  NursePatientQueueItemVO,
  NursePatientStatus
} from '@/apis/outpatient/nurse-station'
import type { PatientStatusTone, PatientSummary } from '@/components/patient/type'

const patientStatusTone: Record<NursePatientStatus, PatientStatusTone> = {
  WAITING: 'warning',
  READY: 'primary',
  COMPLETED: 'success'
}

/** 将护士站患者 DTO 转为公共患者摘要。 */
export function toNursePatientSummary(patient: NursePatientQueueItemVO): PatientSummary {
  return {
    id: patient.encounterId,
    name: patient.patientName,
    gender: patient.genderName,
    age: patient.ageText,
    patientType: 'outpatient',
    serviceType: patient.visitTypeName,
    patientNo: patient.medicalRecordNo,
    visitNo: patient.visitNo,
    queueNumber: patient.queueNumber,
    visitTime: patient.registeredAt,
    departmentName: patient.departmentName,
    status: patient.statusName,
    statusTone: patientStatusTone[patient.status],
    paymentType: patient.paymentTypeName,
    priority: patient.priorityName,
    riskHint: patient.riskHint
  }
}

/** 执行状态对应的 Element Plus 标签颜色。 */
export function executionStatusTagType(
  status: NurseExecutionStatus
): 'success' | 'danger' | 'primary' {
  if (status === 'COMPLETED') return 'success'
  if (status === 'CANCELLED') return 'danger'
  return 'primary'
}
