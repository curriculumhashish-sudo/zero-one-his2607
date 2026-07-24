import type {
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisVO,
  DoctorPatientQueueItemVO,
  DoctorVisitStatus,
  SaveDoctorDiagnosisItemDTO
} from '@/apis/outpatient/doctor-station'
import type {
  ClinicalDiagnosisCandidate,
  ClinicalDiagnosisItem
} from '@/components/clinical/diagnosis'
import type { PatientStatusTone, PatientSummary } from '@/components/patient/type'

/**
 * 后端状态码与公共患者组件语义色的对应关系。
 *
 * 状态码属于医生站业务，不能写进公共 PatientCard。
 */
const visitStatusToneMap: Record<DoctorVisitStatus, PatientStatusTone> = {
  WAITING: 'warning',
  IN_PROGRESS: 'primary',
  COMPLETED: 'success'
}

/**
 * 将接口 VO 转换为公共组件认识的 PatientSummary。
 *
 * 适配器隔离了接口字段名和组件字段名：
 * 即使后端以后把 patientName 改成 name，也只需要修改这里。
 */
export function toPatientSummary(patient: DoctorPatientQueueItemVO): PatientSummary {
  return {
    id: patient.encounterId,
    name: patient.patientName,
    gender: patient.genderName,
    age: patient.age,
    patientType: 'outpatient',
    serviceType: patient.visitTypeName,
    patientNo: patient.patientNo,
    visitNo: patient.visitNo,
    queueNumber: patient.queueNumber,
    visitTime: patient.registeredAt,
    departmentName: patient.departmentName,
    status: patient.visitStatusName,
    statusTone: visitStatusToneMap[patient.visitStatus],
    paymentType: patient.paymentTypeName,
    priority: patient.priorityName,
    riskHint: patient.riskHint
  }
}

/** 将门诊诊断 VO 转换为门诊、住院都能使用的公共诊断模型。 */
export function toClinicalDiagnosis(diagnosis: DoctorDiagnosisVO): ClinicalDiagnosisItem {
  return {
    key: diagnosis.id,
    recordId: diagnosis.id,
    catalogId: diagnosis.catalogId,
    code: diagnosis.code,
    name: diagnosis.name,
    typeName: diagnosis.typeName,
    primary: diagnosis.primary,
    diagnosedAt: diagnosis.diagnosedAt
  }
}

/** 将门诊诊断字典 VO 转换为公共诊断选择弹窗认识的候选项。 */
export function toClinicalDiagnosisCandidate(
  diagnosis: DoctorDiagnosisDictionaryItemVO
): ClinicalDiagnosisCandidate {
  return {
    id: diagnosis.id,
    code: diagnosis.code,
    name: diagnosis.name,
    typeName: diagnosis.typeName
  }
}

/** 将公共诊断模型转换回门诊保存 DTO。 */
export function toSaveDoctorDiagnosisItem(
  diagnosis: ClinicalDiagnosisItem,
  index: number
): SaveDoctorDiagnosisItemDTO {
  return {
    id: diagnosis.recordId,
    catalogId: diagnosis.catalogId,
    code: diagnosis.code,
    name: diagnosis.name,
    typeName: diagnosis.typeName,
    primary: diagnosis.primary,
    orderNo: index + 1
  }
}
