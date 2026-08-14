import type { InpatientSettlePatient, InpatientSettleSummary } from '@/apis/inpatient/charge/settle'
import type { DetailSummaryItem } from '@/components/common/type'
import type { PatientSummary } from '@/components/patient/type'

export function getSettlePatientCardId(row: InpatientSettlePatient): string {
  const encounterId = String(row.encounterId ?? '').trim()
  if (encounterId && encounterId !== '0') return encounterId

  return String(row.encounterBusNo ?? row.patientId ?? encounterId).trim()
}

export function toSettlePatientSummary(row: InpatientSettlePatient): PatientSummary {
  return {
    id: getSettlePatientCardId(row),
    name: row.patientName,
    gender: row.genderEnumText,
    age: row.ageText,
    patientType: 'inpatient',
    patientNo: row.patientId,
    visitNo: row.encounterBusNo,
    visitTime: row.startTime,
    departmentName: row.organizationName,
    serviceType: [row.wardName, row.bedName].filter(Boolean).join(' · '),
    status: row.statusEnumText,
    statusTone: row.statusEnum === 'SETTLED' ? 'success' : 'warning',
    paymentType: row.contractDisplay
  }
}

export function toSettleSummaryItems(summary: InpatientSettleSummary | null): DetailSummaryItem[] {
  if (!summary) return []
  return [
    {
      key: 'pending',
      label: '待结算费用',
      value: `¥${Number(summary.pendingSettlementTotal).toFixed(2)}`
    },
    { key: 'selected', label: '本次已选', value: `¥${Number(summary.selectedTotal).toFixed(2)}` },
    {
      key: 'balance',
      label: '预交金余额',
      value: `¥${Number(summary.prepayNetBalance).toFixed(2)}`
    },
    { key: 'after', label: '结算后余额', value: `¥${Number(summary.balanceHint).toFixed(2)}` }
  ]
}
