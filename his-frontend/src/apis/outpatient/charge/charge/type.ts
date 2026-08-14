export type ChargeQueueStatus = 'all' | 'pending' | 'settled'
export type InsuranceMode = 'insurance' | 'self-pay'
export type PaymentChannel = 'cash' | 'card' | 'wechat' | 'alipay'
export type PaymentStatus = 'pending' | 'success' | 'failed'
export type ReceiptStatus = 'pending' | 'issued' | 'failed'
export type ReceiptPrintStatus = 'auto-queued' | 'scheduled' | 'printed'

export interface PageResult<T> {
  rows: T[]
  records: T[]
  content?: T[]
  total: number
  totalCount?: number
  pageIndex?: number
  pageSize?: number
}
export interface ChargePatient {
  id: string
  encounterId: string
  patientId?: string
  patientNo: string
  encounterBusNo?: string
  visitNo: string
  patientName?: string
  name: string
  genderCode?: string
  genderEnumText?: string
  gender: string
  ageText?: string
  age: number
  organizationName?: string
  doctorName?: string
  departmentName: string
  startTime?: string
  visitTime: string
  queueLabelCode?: string
  totalAmount?: number
  paidAmount?: number
  paymentId?: string
  status: 'pending' | 'settled'
  paymentType: string
}
export interface ChargeItem {
  accountId?: string
  chargeItemId?: string
  id: string
  busNo?: string
  cashierName?: string
  cashierUserId?: string
  chargeCategoryCode?: string
  chargeCategoryDisplay?: string
  contractId?: string
  category: string
  itemName: string
  medicalInsuranceCode?: string
  feeNatureDisplay?: string
  discountRateDisplay?: string
  medTypeCode?: string
  specification: string
  quantityDisplay?: string
  quantityValue?: number
  quantityUnit?: string
  serviceId?: string
  serviceTable?: string
  quantity: number
  unitPrice: number
  totalPrice?: number
  amount: number
  insuranceAmount: number
  selfPayAmount: number
  statusCode?: string
  statusText?: string
  status: 'pending' | 'settled'
  recordId?: string
}
export interface ChargeSummary {
  totalAmount: number
  insuranceAmount: number
  selfPayAmount: number
}
export interface ChargeDetail {
  encounterId: string
  patient: ChargePatient
  items: ChargeItem[]
  insuranceMode: InsuranceMode
  summary: ChargeSummary
  interruptedPaymentId?: string
  latestSettledRecordId?: string
  receiptWarning?: string
}
export interface ChargeQueueQuery {
  organizationId?: string | null
  keyword?: string
  queueTabCode?: 'ALL' | 'PENDING' | 'SETTLED'
  startDate?: string | null
  endDate?: string | null
  /** 仅兼容旧 mock；正式接口使用 queueTabCode。 */
  status?: ChargeQueueStatus
  pageSize?: number
  pageIndex?: number
}
export interface PageOutpatientChargeQueueRequest {
  endDate?: string | null
  keyword?: string
  organizationId?: string | null
  pageSize?: number
  pageIndex?: number
  queueTabCode: 'ALL' | 'PENDING' | 'SETTLED'
  startDate?: string | null
}
export interface PageOutpatientChargeQueueResponse {
  records: ChargePatient[]
  total: number | string
}
export interface LoadPendingChargeItemsRequest {
  encounterId: string
  includePriceExtension?: boolean
}
export interface LoadPendingChargeItemsResponse {
  encounterId: string
  items: ChargeItem[]
}
export interface PreSettleOutpatientChargesResponse {
  paymentDraftId: string
  displayAmount: number | string
  previews?: Array<{
    chargeItemId: string
    itemLabel?: string | null
    lineAmount: number | string
    quantityDisplay?: string | null
    quantityUnit?: string | null
    feeNatureDisplay?: string | null
  }>
  insuranceGroups?: Array<{
    accountId?: string
    chargeItemIds: string[]
    contractId?: string
    chrgBchno?: string
    diseCodg?: string
    diseName?: string
    groupIndex?: number
    mdtrtId?: string
    medType?: string
    medTypeDisplay?: string
    medfeeSumamt?: number
    preSettleId?: string
    fundPaySumamt?: number | string | null
    acctPay?: number | string | null
    psnCashPay?: number | string | null
  }>
  selfPayRollup?: { accountId?: string | null; chargeItemIds?: string[]; totalAmount?: number | string | null } | null
  insurancePreSettled?: boolean
  settlementComposition?: string | null
  ybFundPaySumamt?: number | string | null
  ybAcctPay?: number | string | null
  ybPsnCashPay?: number | string | null
  ybMedfeeSumamt?: number | string | null
  ybChrgBchno?: string | null
}
export type PreSettlement = PreSettleOutpatientChargesResponse
export interface PreSettleOutpatientChargesRequest {
  chargeItemIds: string[]
  contractId?: string | null
  encounterId: string
  insuplcAdmdvs?: string | null
  insutype?: string | null
  medType?: string | null
  psnNo?: string | null
  ybCardInfo?: string | null
  ybMediumType?: string | null
}
export interface ConfirmOutpatientChargeSettlementRequest {
  billDate: string
  chargeItemIds: string[]
  encounterId: string
  outcomeEnum: string
  paymentDetails: Array<{ amount: number | string; payChannelCode: string }>
  paymentDraftId: string
  clientScanResult?: {
    merTradeNo: string
    tradeNo: string
    payType: string
    respCode: string
    respMsg: string
    txnAmtFen?: number
  }
  ybChrgBchno?: string | null
  ybChrgBchnos?: string[]
}
export interface ConfirmOutpatientChargeSettlementResponse {
  invoiceId: string
  invoiceStatus?: string
  paymentId: string
  successMessageCode?: string | null
  ybSetlId?: string | null
}
export type SettleChargePayload = ConfirmOutpatientChargeSettlementRequest
export interface ChargeRecord {
  paymentId?: string
  invoiceId?: string
  invoiceStatus?: string
  successMessageCode?: string
  ybSetlId?: string
  id: string
  encounterId: string
  itemIds: string[]
  receiptNo: string
  amount: number
  channel: PaymentChannel
  paymentDetails?: Array<{ amount: number; payChannelCode: PaymentChannel }>
  status: PaymentStatus
  receiptStatus: ReceiptStatus
  printStatus: ReceiptPrintStatus
  receiptFailureReason?: string
  createdAt: string
}
export interface ReceiptData {
  recordId: string
  receiptNo: string
  patientName: string
  amount: number
  issuedAt: string
  printStatus: ReceiptPrintStatus
  previewText: string
  slip: {
    encounterId: string
    patientName: string
    hospitalName: string
    genderEnumText: string
    ageText: string
    feeNatureText: string
    chargeItemLines: Array<{
      itemName: string
      quantityText: string
      amountText: string
      chargeCategoryText: string
    }>
    totalAmountText: string
    poolingPayText: string
    fundPayText: string
    accountPayText: string
    deductibleLineText: string
    cashPayText: string
    wechatPayText: string
    alipayPayText: string
    accountBalanceText: string
    cashierName: string
    chargeTimeText: string
  }
}
export interface PrintChargeReceiptParams {
  encounterId?: string | null
  paymentId?: string | null
}
export interface RequestContext {
  signal?: AbortSignal
}
