import type { InpatientPatient } from '@/apis/inpatient/charge/prepay'
import type { PatientSummary } from '@/components/patient/type'
export function toPrepayPatientSummary(row: InpatientPatient): PatientSummary {
  return {
    id: row.encounterId,
    name: row.patientName ?? row.name,
    gender: row.genderEnumText ?? row.gender,
    age: Number.parseInt(row.ageText ?? '', 10) || row.age,
    patientType: 'inpatient',
    patientNo: row.patientNo,
    visitNo: row.encounterBusNo ?? row.visitNo,
    visitTime: row.startTime ?? row.admissionDate,
    departmentName: row.organizationName ?? row.departmentName,
    serviceType: `${row.wardName} · ${row.bedName ?? row.bedNo}`,
    status: row.statusEnumText ?? '在院',
    statusTone: 'success',
    paymentType: row.paymentType
  }
}
