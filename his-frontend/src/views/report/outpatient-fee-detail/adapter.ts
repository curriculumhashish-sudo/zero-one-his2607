import type {
  OutpatientFeeDetailAmountSummaryDto,
  OutpatientFeeDetailEncounterBlockDto,
  OutpatientFeeDetailLineDto
} from '@/apis/report/outpatient-fee-detail'
import type { FeeDetailTableRow } from './type'

export function flattenEncounterBlocks(blocks: OutpatientFeeDetailEncounterBlockDto[]): FeeDetailTableRow[] {
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
    if (lines.length && block.subtotal) {
      rows.push({ ...subtotalRow(block.subtotal, key), patientName: '就诊小计' })
    }
  })
  return rows
}

function subtotalRow(summary: OutpatientFeeDetailAmountSummaryDto, key: string): FeeDetailTableRow {
  return {
    rowKey: `subtotal-${key}`,
    rowType: 'subtotal',
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

export function displayAmount(value: OutpatientFeeDetailLineDto[keyof OutpatientFeeDetailLineDto]) {
  return value === undefined || value === null || value === '' ? '—' : String(value)
}
