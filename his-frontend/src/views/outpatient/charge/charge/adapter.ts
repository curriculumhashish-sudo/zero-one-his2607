import type { ChargePatient } from '@/apis/outpatient/charge/charge'
import type { PatientSummary } from '@/components/patient/type'
export function toChargePatientSummary(row: ChargePatient): PatientSummary {
  return {
    id: row.encounterId,
    name: row.patientName ?? row.name,
    gender: row.genderEnumText ?? row.gender,
    age: Number.parseInt(row.ageText ?? '', 10) || row.age,
    patientType: 'outpatient',
    patientNo: row.patientNo,
    visitNo: row.encounterBusNo ?? row.visitNo,
    visitTime: row.startTime ?? row.visitTime,
    departmentName: row.organizationName ?? row.departmentName,
    serviceType: '门诊',
    status: row.status === 'pending' ? '待收费' : '已收费',
    statusTone: row.status === 'pending' ? 'warning' : 'success',
    paymentType: row.paymentType
  }
}
