import type { RegistrationContext, RegistrationQueueRecord } from '@/apis/inpatient/charge/register'
import type { PatientSummary } from '@/components/patient/type'
import type { RegistrationDetailModel } from './type'

export function calculateRegistrationAge(birthDate?: string, referenceDate = new Date()) {
  const normalized = birthDate?.slice(0, 10) ?? ''
  if (!normalized) return null
  const [yearText, monthText, dayText] = normalized.split('-')
  const year = Number.parseInt(yearText ?? '', 10)
  const month = Number.parseInt(monthText ?? '', 10)
  const day = Number.parseInt(dayText ?? '', 10)
  const parsed = new Date(year, month - 1, day)
  if (
    parsed.getFullYear() !== year ||
    parsed.getMonth() !== month - 1 ||
    parsed.getDate() !== day
  ) {
    return null
  }
  let age = referenceDate.getFullYear() - year
  const birthdayPassed =
    referenceDate.getMonth() + 1 > month ||
    (referenceDate.getMonth() + 1 === month && referenceDate.getDate() >= day)
  if (!birthdayPassed) age -= 1
  return age >= 0 ? age : null
}

/** 与参考原型一致：解析 18 位居民身份证，不校验末位校验码。 */
export function parseRegistrationIdCard(value: string) {
  const idCard = value.trim().toUpperCase()
  if (!/^\d{17}[\dX]$/.test(idCard)) return null
  const birthText = idCard.slice(6, 14)
  const birthDate = `${birthText.slice(0, 4)}-${birthText.slice(4, 6)}-${birthText.slice(6, 8)}`
  const age = calculateRegistrationAge(birthDate)
  if (age == null) return null
  const genderEnum = Number(idCard.charAt(16)) % 2 === 1 ? 'MALE' : 'FEMALE'
  return { age, birthDate, genderEnum }
}

export function toRegistrationPatientSummary(record: RegistrationQueueRecord): PatientSummary {
  return {
    id: record.encounterId,
    name: record.patientName,
    patientType: 'inpatient',
    patientNo: record.patientId,
    visitNo: record.encounterBusNo,
    visitTime: record.startTime,
    departmentName: record.organizationName,
    serviceType: record.wardName,
    status: record.statusEnumText,
    statusTone:
      record.statusEnum === 'ADMITTED'
        ? 'success'
        : record.statusEnum === 'WAIT_ADMISSION'
          ? 'warning'
          : 'info',
    paymentType: record.insurancePlatformRegistered ? '医保' : '自费'
  }
}

export function toRegistrationDetail(context: RegistrationContext): RegistrationDetailModel {
  const birthDate = context.patient?.birthDate?.slice(0, 10) ?? ''
  return {
    patientName: context.patient?.name ?? '',
    patientNo: context.patient?.patientId ?? '',
    gender: context.patient?.genderEnum ?? '',
    birthDate,
    age: calculateRegistrationAge(birthDate) ?? '',
    phone: context.patient?.phone ?? '',
    idCard: context.patient?.idCard ?? '',
    inpatientNo: context.encounter?.encounterBusNo ?? '',
    department: context.encounter?.organizationName ?? '',
    ward: context.encounter?.wardName ?? '',
    bed: context.encounter?.bedName ?? '',
    outpatientDoctor: context.encounter?.outpatientDoctorName ?? '',
    attendingDoctor: context.encounter?.attendingDoctorName ?? '',
    diagnosis: context.encounter?.primaryConditionDefinitionName ?? '',
    patientCondition: context.encounter?.priorityEnum ?? '',
    admissionSource: context.encounter?.admitSourceCodeText ?? '',
    admissionWay: context.encounter?.inWayCodeText ?? '',
    admissionTime: context.encounter?.startTime ?? '',
    status: context.encounter?.statusEnum ?? ''
  }
}
