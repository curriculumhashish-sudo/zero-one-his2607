import type { NurseExecutionStatus, NursePatientQueueItemVO } from '@/apis/outpatient/nurse-station'
import type { PatientSummary } from '@/components/patient/type'

/** 将护士站患者 DTO 转为公共患者摘要。 */
export function toNursePatientSummary(patient: NursePatientQueueItemVO): PatientSummary {
  return {
    id: patient.encounterId,
    name: patient.patientName,
    gender: patient.genderName,
    age: patient.ageText,
    patientType: 'outpatient',
    serviceType: patient.visitTypeName,
    // 后端提供患者 ID，但未提供独立病历号；患者号位置展示患者 ID。
    patientNo: patient.patientId,
    departmentName: patient.departmentName,
    priority: patient.priorityName,
    status: patient.statusName,
    statusTone:
      patient.status === 'COMPLETED'
        ? 'success'
        : patient.status === 'READY'
          ? 'warning'
          : 'primary'
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
