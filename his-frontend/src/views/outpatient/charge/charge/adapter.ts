import type { ChargePatient } from '@/apis/outpatient/charge/charge'
import type { PatientSummary } from '@/components/patient/type'
export function toChargePatientSummary(row: ChargePatient): PatientSummary {
  return {
    id: row.encounterId,
    name: row.name,
    gender: row.gender,
    age: row.age,
    patientType: 'outpatient',
    patientNo: row.patientNo,
    visitNo: row.visitNo,
    visitTime: row.visitTime,
    departmentName: row.departmentName,
    serviceType: '门诊',
    status: row.status === 'pending' ? '待收费' : '已收费',
    statusTone: row.status === 'pending' ? 'warning' : 'success',
    paymentType: row.paymentType
  }
}
