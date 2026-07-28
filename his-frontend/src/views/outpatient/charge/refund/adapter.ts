import type { RefundPatient } from '@/apis/outpatient/charge/refund'
import type { PatientSummary } from '@/components/patient/type'
export function toRefundPatientSummary(row: RefundPatient): PatientSummary {
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
    serviceType: '门诊退费',
    status: row.status === 'pending' ? '待退费' : '已退费',
    statusTone: row.status === 'pending' ? 'warning' : 'success',
    paymentType: row.paymentType
  }
}
