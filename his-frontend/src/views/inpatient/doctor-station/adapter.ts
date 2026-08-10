import type {
  CreateInpatientDiagnosisDTO,
  InpatientDiagnosisDictionaryItemVO,
  InpatientDiagnosisRecordVO,
  InpatientDiagnosisUsageHistoryVO,
  InpatientDoctorPatientQueueItemVO,
  InpatientEncounterStatus,
  UpdateInpatientDiagnosisDTO
} from '@/apis/inpatient/doctor-station'
import type {
  ClinicalDiagnosisCandidate,
  ClinicalDiagnosisItem
} from '@/components/clinical/diagnosis'
import type { PatientStatusTone, PatientSummary } from '@/components/patient/type'

const statusToneMap: Record<InpatientEncounterStatus, PatientStatusTone> = {
  IN_HOSPITAL: 'primary',
  DISCHARGE_PENDING: 'warning'
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
    patientNo: patient.patientNo,
    visitNo: patient.inpatientNo,
    visitTime: patient.admissionAt,
    departmentName: patient.departmentName,
    status: patient.encounterStatusName,
    statusTone: statusToneMap[patient.encounterStatus],
    paymentType: patient.paymentTypeName
  }
}

/** 将住院诊断记录转换为公共诊断编辑器模型。 */
export function toInpatientClinicalDiagnosis(
  diagnosis: InpatientDiagnosisRecordVO
): ClinicalDiagnosisItem {
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

/** 将住院诊断字典候选项转换为公共选择弹窗模型。 */
export function toInpatientDiagnosisCandidate(
  diagnosis: InpatientDiagnosisDictionaryItemVO | InpatientDiagnosisUsageHistoryVO
): ClinicalDiagnosisCandidate {
  return {
    id: diagnosis.id,
    code: diagnosis.code,
    name: diagnosis.name,
    typeName: diagnosis.typeName
  }
}

/** 将公共诊断草稿转换为住院新增参数。 */
export function toCreateInpatientDiagnosisDTO(
  diagnosis: ClinicalDiagnosisItem,
  index: number,
  encounterId: string,
  patientId: string
): CreateInpatientDiagnosisDTO {
  return {
    encounterId,
    patientId,
    catalogId: diagnosis.catalogId,
    code: diagnosis.code,
    name: diagnosis.name,
    typeName: diagnosis.typeName,
    primary: diagnosis.primary,
    orderNo: index + 1
  }
}

/** 将已保存的公共诊断转换为住院修改参数。 */
export function toUpdateInpatientDiagnosisDTO(
  diagnosis: ClinicalDiagnosisItem,
  index: number,
  version: number
): UpdateInpatientDiagnosisDTO {
  return {
    catalogId: diagnosis.catalogId,
    code: diagnosis.code,
    name: diagnosis.name,
    typeName: diagnosis.typeName,
    primary: diagnosis.primary,
    orderNo: index + 1,
    version
  }
}
