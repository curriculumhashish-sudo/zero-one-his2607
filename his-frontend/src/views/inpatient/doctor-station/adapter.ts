import type { InpatientDoctorPatientQueueItemVO } from '@/apis/inpatient/doctor-station'
import type { PatientStatusTone, PatientSummary } from '@/components/patient/type'

const statusNameMap: Record<string, string> = {
  IN_PROGRESS: '在院'
}

const statusToneMap: Record<string, PatientStatusTone> = {
  IN_PROGRESS: 'primary'
}

/** 将住院队列 DTO 转换为公共患者卡片模型。 */
export function toInpatientPatientSummary(
  patient: InpatientDoctorPatientQueueItemVO
): PatientSummary {
  const wardBed = [patient.wardName, patient.bedName].filter(Boolean).join(' · ')

  return {
    id: patient.encounterId,
    name: patient.patientName,
    gender: patient.genderName,
    age: patient.ageText,
    patientType: 'inpatient',
    serviceType: wardBed,
    visitNo: patient.inpatientNo,
    visitTime: patient.admissionAt,
    departmentName: patient.departmentName,
    statusCode: patient.statusEnum,
    status: statusNameMap[patient.statusEnum] ?? patient.statusEnum,
    statusTone: statusToneMap[patient.statusEnum] ?? 'info'
  }
}
