import type {
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisVO,
  DoctorPatientQueueItemVO,
  DoctorVisitStatus,
  OpdoctorCreateDiagnosisDTO,
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
  arrived: 'warning',
  'in-progress': 'primary',
  finished: 'success'
}

/** 就诊状态码对应的中文名；后端不再返回名称，由前端根据 visitStatus 判断。 */
const visitStatusNameMap: Record<DoctorVisitStatus, string> = {
  arrived: '候诊',
  'in-progress': '在诊',
  finished: '完诊'
}

/** 根据就诊状态码返回中文状态名。 */
export function getVisitStatusName(status: DoctorVisitStatus): string {
  return visitStatusNameMap[status]
}

/** 门诊医生只需要区分医保和自费，医保子类型不在患者卡片中展开。 */
function toSimplePaymentType(paymentTypeName: string): '医保' | '自费' {
  return paymentTypeName.includes('自费') ? '自费' : '医保'
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
    gender: patient.gender,
    age: patient.age,
    patientType: 'outpatient',
    serviceType: patient.registerType,
    patientNo: patient.visitNo,
    queueNumber: patient.queueNumber,
    visitTime: patient.visitTime,
    departmentName: patient.departmentName,
    statusCode: patient.visitStatus,
    status: getVisitStatusName(patient.visitStatus),
    statusTone: visitStatusToneMap[patient.visitStatus],
    paymentType: toSimplePaymentType(patient.paymentTypeName),
    priority: patient.priorityName
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

/**
 * 将门诊诊断项映射为后端新增诊断请求体。
 *
 * conditionId（= catalogId）、diagCode（= ICD 编码）已与后端确认；
 * diagTypeCode 的分类编码值待后端提供编码表，当前先直传分类名称。
 * primary 转 primaryFlag（1/0）。
 */
export function toOpdoctorCreateDiagnosis(
  encounterId: string,
  item: SaveDoctorDiagnosisItemDTO
): OpdoctorCreateDiagnosisDTO {
  return {
    encounterId,
    conditionId: item.catalogId,
    diagCode: item.code,
    description: item.name,
    diagTypeCode: item.typeName,
    primaryFlag: item.primary ? 1 : 0,
    rank: item.orderNo
  }
}
