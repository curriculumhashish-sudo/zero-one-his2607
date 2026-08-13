export interface PageOutpatientFeeDetailParams {
  pageIndex: number
  pageSize: number
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
  pageIndex?: number
  pageSize?: number
  summary?: OutpatientFeeDetailAmountSummaryDto
}

/** Backend response before the frontend normalizes the eight flat totals. */
export interface PageOutpatientFeeDetailBackendResponse {
  rows?: OutpatientFeeDetailBackendRowDto[]
  summary?: OutpatientFeeDetailAmountSummaryDto
  totalRowCount?: number
  // The deployed J8 response uses content as a flat charge-item row list.
  // Mock responses may already contain grouped encounter blocks.
  content?: OutpatientFeeDetailBackendRowDto[] | OutpatientFeeDetailEncounterBlockDto[]
  totalCount?: number
  pageIndex?: number
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

/** J8 返回的单条收费项目；同一次就诊的字段会在每行重复。 */
export interface OutpatientFeeDetailBackendRowDto extends OutpatientFeeDetailLineDto {
  sequenceNo?: number
  outpatientNo?: string
  patientName?: string
  registrationDepartmentName?: string
  deptName?: string
  encounterDate?: string
  sourceDisplay?: string
}

export type ExportOutpatientFeeDetailParams = Omit<
  PageOutpatientFeeDetailParams,
  'pageIndex' | 'pageSize'
>

export interface ExportOutpatientFeeDetailResponse {
  fileName?: string
  contentType?: string
  body?: string
  downloadUrl?: string
  rowCount?: number
  exportedRowCount?: number
  truncated?: boolean
}

export interface RequestContext {
  signal?: AbortSignal
}
