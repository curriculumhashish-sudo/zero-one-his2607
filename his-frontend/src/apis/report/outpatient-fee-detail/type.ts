export interface PageOutpatientFeeDetailParams {
  PageIndex: number
  PageSize: number
  dateFrom: string
  dateTo: string
  registrationDepartmentId?: string
  feeCategoryCodes?: string[]
  subCategoryCodes?: string[]
  patientKeyword?: string
  productNameKeyword?: string
  productCodeKeyword?: string
  cashierId?: string
  tradeDirection?: string
}

export interface OutpatientFeeDetailAmountSummaryDto {
  baseAmountTotal?: number | string
  discountAmountTotal?: number | string
  receivableAmountTotal?: number | string
  discountedAmountTotal?: number | string
  lineAmountTotal?: number | string
  actualReceivedAmountTotal?: number | string
  insuranceAmountTotal?: number | string
  selfPayAmountTotal?: number | string
}

export interface OutpatientFeeDetailLineDto {
  chargeItemId?: string
  paymentId?: string
  feeCategoryCode?: string
  feeCategoryDisplay?: string
  subCategoryCode?: string
  subCategoryDisplay?: string
  productName?: string
  productCode?: string
  specification?: string
  quantityValue?: number | string
  quantityUnitDisplay?: string
  orderingDoctorId?: string
  orderingDoctorName?: string
  cashierId?: string
  cashierName?: string
  billDate?: string
  tradeDirection?: string
  insuranceLevelDisplay?: string
  insuranceTypeDisplay?: string
  baseAmount?: number | string
  discountAmount?: number | string
  receivableAmount?: number | string
  discountedAmount?: number | string
  unitPrice?: number | string
  lineAmount?: number | string
  actualReceivedAmount?: number | string
  insuranceAmount?: number | string
  selfPayAmount?: number | string
  discountRateDisplay?: string
}

export interface OutpatientFeeDetailEncounterBlockDto {
  encounterId?: string
  patientName?: string
  busNo?: string
  registrationDepartmentName?: string
  encounterDate?: string
  lines?: OutpatientFeeDetailLineDto[]
  subtotal?: OutpatientFeeDetailAmountSummaryDto
}

export interface PageOutpatientFeeDetailResponse {
  content?: OutpatientFeeDetailEncounterBlockDto[]
  totalCount?: number
  pageNo?: number
  pageSize?: number
  summary?: OutpatientFeeDetailAmountSummaryDto
}

/** Backend response before the frontend normalizes the eight flat totals. */
export interface PageOutpatientFeeDetailBackendResponse {
  content?: OutpatientFeeDetailEncounterBlockDto[]
  totalCount?: number
  pageNo?: number
  pageSize?: number
  baseAmountSum?: number | string
  discountAmountSum?: number | string
  receivableAmountSum?: number | string
  discountedAmountSum?: number | string
  lineAmountSum?: number | string
  actualReceivedAmountSum?: number | string
  insuranceAmountSum?: number | string
  selfPayAmountSum?: number | string
}

export type ExportOutpatientFeeDetailParams = Omit<
  PageOutpatientFeeDetailParams,
  'PageIndex' | 'PageSize'
>

export interface ExportOutpatientFeeDetailResponse {
  fileName?: string
  contentType?: string
  body?: string
  rowCount?: number
  exportedRowCount?: number
  truncated?: boolean
}

export interface RequestContext {
  signal?: AbortSignal
}
