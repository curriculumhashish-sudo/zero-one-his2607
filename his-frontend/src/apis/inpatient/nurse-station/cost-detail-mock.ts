import dayjs from 'dayjs'

import { createPageDTO } from '../../type'
import type {
  NurseStationCostDetailLineVO,
  NurseStationCostDetailQueryDTO,
  NurseStationCostSummaryLineVO,
  NurseStationCostSummaryQueryDTO
} from './cost-detail-type'

const today = dayjs().format('YYYY-MM-DD')
const yesterday = dayjs().subtract(1, 'day').format('YYYY-MM-DD')

const detailRows: NurseStationCostDetailLineVO[] = [
  {
    id: 'cost-001',
    patientId: 'patient-001',
    encounterId: 'encounter-001',
    chargeName: '注射用头孢曲松钠',
    typeCode: 'DRUG',
    unitPrice: 18.5,
    quantityValue: 2,
    quantityUnit: '瓶',
    totalPrice: 37,
    baseAmount: 37,
    discountAmount: 0,
    occurrenceTime: `${today} 08:20:00`,
    performingOrgId: 'org-inpatient',
    statusEnum: 'PENDING_SETTLEMENT',
    busNo: 'CHG-001'
  },
  {
    id: 'cost-002',
    patientId: 'patient-001',
    encounterId: 'encounter-001',
    chargeName: 'Ⅱ级护理',
    typeCode: 'SERVICE',
    unitPrice: 18,
    quantityValue: 1,
    quantityUnit: '日',
    totalPrice: 18,
    baseAmount: 18,
    discountAmount: 0,
    occurrenceTime: `${today} 00:05:00`,
    performingOrgId: 'org-inpatient',
    statusEnum: 'PENDING_SETTLEMENT',
    busNo: 'CHG-002'
  },
  {
    id: 'cost-003',
    patientId: 'patient-001',
    encounterId: 'encounter-001',
    chargeName: '一次性使用输液器',
    typeCode: 'DEVICE',
    unitPrice: 6.8,
    quantityValue: 1,
    quantityUnit: '套',
    totalPrice: 6.8,
    baseAmount: 6.8,
    discountAmount: 0,
    occurrenceTime: `${today} 08:25:00`,
    performingOrgId: 'org-inpatient',
    statusEnum: 'PENDING_SETTLEMENT',
    busNo: 'CHG-003'
  },
  {
    id: 'cost-004',
    patientId: 'patient-001',
    encounterId: 'encounter-001',
    chargeName: '血细胞分析',
    typeCode: 'SERVICE',
    unitPrice: 28,
    quantityValue: 1,
    quantityUnit: '次',
    totalPrice: 28,
    baseAmount: 28,
    discountAmount: 0,
    occurrenceTime: `${yesterday} 10:35:00`,
    performingOrgId: 'org-laboratory',
    statusEnum: 'SETTLED',
    busNo: 'CHG-004'
  }
]

function queryRows(query: NurseStationCostDetailQueryDTO) {
  return detailRows
    .filter(
      (row) =>
        row.patientId === query.patientId &&
        row.encounterId === query.encounterId &&
        row.occurrenceTime.slice(0, 10) >= query.startDate &&
        row.occurrenceTime.slice(0, 10) <= query.endDate &&
        (!query.typeCode || row.typeCode === query.typeCode) &&
        (!query.busNo || row.busNo === query.busNo)
    )
    .sort((left, right) => right.occurrenceTime.localeCompare(left.occurrenceTime))
}

export async function mockQueryNurseStationCostDetail(query: NurseStationCostDetailQueryDTO) {
  await Promise.resolve()
  const rows = queryRows(query)
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, Math.min(query.pageSize, 100))
  const start = (pageIndex - 1) * pageSize
  return createPageDTO({
    pageIndex,
    pageSize,
    total: rows.length,
    rows: rows.slice(start, start + pageSize).map((row) => ({ ...row }))
  })
}

export async function mockQueryNurseStationCostSummary(
  query: NurseStationCostSummaryQueryDTO
): Promise<NurseStationCostSummaryLineVO[]> {
  await Promise.resolve()
  const rows = detailRows.filter(
    (row) =>
      row.encounterId === query.encounterId &&
      row.occurrenceTime.slice(0, 10) >= query.startDate &&
      row.occurrenceTime.slice(0, 10) <= query.endDate
  )
  const groups = new Map<string, NurseStationCostDetailLineVO[]>()
  for (const row of rows) {
    const summaryDate = row.occurrenceTime.slice(0, 10)
    const key = `${row.typeCode}|${summaryDate}`
    groups.set(key, [...(groups.get(key) ?? []), row])
  }
  return [...groups.values()].map((items) => {
    const first = items[0]!
    const totalAmount = items.reduce((sum, item) => sum + item.totalPrice, 0)
    return {
      typeCode: first.typeCode,
      typeName: first.typeCode,
      totalAmount,
      count: items.length,
      insuranceAmount: 0,
      selfPayAmount: totalAmount,
      summaryDate: first.occurrenceTime.slice(0, 10)
    }
  })
}
