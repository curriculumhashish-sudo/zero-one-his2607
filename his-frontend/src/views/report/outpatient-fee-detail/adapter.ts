import type {
  OutpatientFeeDetailAmountSummaryDto,
  OutpatientFeeDetailEncounterBlockDto,
  OutpatientFeeDetailLineDto
} from '@/apis/report/outpatient-fee-detail'
import type { FeeDetailTableRow } from './type'

export function flattenEncounterBlocks(
  blocks: OutpatientFeeDetailEncounterBlockDto[]
): FeeDetailTableRow[] {
  const rows: FeeDetailTableRow[] = []
  blocks.forEach((block) => {
    const key = String(block.encounterId ?? block.busNo ?? rows.length)
    const lines = block.lines ?? []
    lines.forEach((line, index) => {
      rows.push({
        ...line,
        rowKey: `line-${key}-${line.chargeItemId ?? index}`,
        rowType: 'line',
        encounterId: block.encounterId,
        patientName: block.patientName,
        busNo: block.busNo,
        registrationDepartmentName: block.registrationDepartmentName,
        encounterDate: block.encounterDate
      })
    })
    if (lines.length && block.subtotal) rows.push(subtotalRow(block.subtotal, key))
  })
  return rows
}

function subtotalRow(summary: OutpatientFeeDetailAmountSummaryDto, key: string): FeeDetailTableRow {
  return {
    rowKey: `subtotal-${key}`,
    rowType: 'subtotal',
    busNo: '就诊小计',
    baseAmount: summary.baseAmountTotal,
    discountAmount: summary.discountAmountTotal,
    receivableAmount: summary.receivableAmountTotal,
    discountedAmount: summary.discountedAmountTotal,
    lineAmount: summary.lineAmountTotal,
    actualReceivedAmount: summary.actualReceivedAmountTotal,
    insuranceAmount: summary.insuranceAmountTotal,
    selfPayAmount: summary.selfPayAmountTotal
  }
}

export function isSubtotal(row: { rowType?: unknown }) {
  return row.rowType === 'subtotal'
}

export function feeDetailSpanMethod({
  row,
  columnIndex
}: {
  row: FeeDetailTableRow
  columnIndex: number
}): [number, number] {
  if (!isSubtotal(row)) return [1, 1]
  if (columnIndex === 0) return [1, 17]
  if (columnIndex < 17) return [0, 0]
  return [1, 1]
}

export function displayAmount(value: OutpatientFeeDetailLineDto[keyof OutpatientFeeDetailLineDto]) {
  if (value === undefined || value === null || value === '') return '—'
  const amount = Number(value)
  return Number.isFinite(amount) ? amount.toFixed(2) : String(value)
}
