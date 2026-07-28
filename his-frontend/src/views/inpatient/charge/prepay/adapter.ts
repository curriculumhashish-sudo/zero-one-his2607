import type { InpatientPatient } from '@/apis/inpatient/charge/prepay'
import type { PatientSummary } from '@/components/patient/type'
export function toPrepayPatientSummary(row: InpatientPatient): PatientSummary {
  return {
    id: row.encounterId,
    name: row.name,
    gender: row.gender,
    age: row.age,
    patientType: 'inpatient',
    patientNo: row.patientNo,
    visitNo: row.visitNo,
    visitTime: row.admissionDate,
    departmentName: row.departmentName,
    serviceType: `${row.wardName} · ${row.bedNo}`,
    status: '在院',
    statusTone: 'success',
    paymentType: row.paymentType
  }
}
