import type { RefundPatient } from '@/apis/outpatient/charge/refund'
import type { PatientSummary } from '@/components/patient/type'
export function toRefundPatientSummary(row: RefundPatient): PatientSummary {
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
    serviceType: '门诊退费',
    status: row.status === 'pending' ? '待退费' : '已退费',
    statusTone: row.status === 'pending' ? 'warning' : 'success',
    paymentType: row.paymentType
  }
}
