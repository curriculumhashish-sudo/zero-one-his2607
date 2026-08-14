import type { OutpatientFeeDetailLineDto } from '@/apis/report/outpatient-fee-detail'

export type FeeDetailTableRow = OutpatientFeeDetailLineDto & {
  rowKey: string
  rowType: 'line' | 'subtotal'
  encounterId?: string
  patientName?: string
  busNo?: string
  registrationDepartmentName?: string
  encounterDate?: string
}

export interface FeeDetailQuery {
  pageIndex: number
  pageSize: number
  dateRange: [string, string]
  registrationDepartmentId?: string
  feeCategoryCodes?: string[]
  subCategoryCodes?: string[]
  patientKeyword?: string
  productNameKeyword?: string
  productCodeKeyword?: string
  cashierId?: string
  tradeDirection: string
}
