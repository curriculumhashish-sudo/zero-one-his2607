export type PrepayChannel = 'cash' | 'card' | 'wechat' | 'alipay'
export type PrepayRecordStatus = 'processing' | 'settled' | 'refunded' | 'cancelled'
export interface PageResult<T> {
  rows: T[]
  content?: T[]
  records?: T[]
  total: number
  totalCount?: number
  pageNo?: number
  pageSize?: number
}
export interface InpatientPatient {
  id: string
  encounterId: string
  patientId?: string
  patientNo: string
  encounterBusNo?: string
  visitNo: string
  patientName?: string
  name: string
  genderEnum?: string
  genderEnumText?: string
  gender: string
  ageText?: string
  age: number
  organizationName?: string
  departmentName: string
  wardName: string
  bedName?: string
  bedNo: string
  startTime?: string
  admissionDate: string
  statusEnum?: string
  statusEnumText?: string
  status: 'in-hospital'
  contractDisplay?: string
  paymentType: string
  accountBalance?: number
  availableBalance?: number
  prepayNetBalance?: number
  pendingSettlementTotal?: number
  lowBalanceWarning?: boolean
  balance: number
}
export interface PrepaySummary {
  patient: InpatientPatient
  balance: number
  totalCollected: number
  totalRefunded: number
  refundableBalance: number
}
export interface PrepayRecord {
  paymentId?: string
  accountBalance?: number
  id: string
  encounterId: string
  type: 'collect' | 'refund'
  bizTypeText?: string
  amount: number
  channel: PrepayChannel
  paymentDetails?: Array<{ accountId?: string; amount: number; payChannelCode: PrepayChannel }>
  payMethodText?: string
  paymentEnum?: string
  operatorName?: string
  ticketNo?: string
  statusEnum?: string
  statusEnumText?: string
  tradeTime?: string
  status: PrepayRecordStatus
  transactionNo: string
  createdAt: string
  refundableAmount: number
}
export interface InpatientQueueQuery {
  admissionDate?: string | null
  admissionStartDate?: string | null
  admissionEndDate?: string | null
  keyword?: string
  page?: number
  pageNo?: number
  pageSize?: number
  PageIndex?: number
  PageSize?: number
}
export interface PrepayRecordQuery {
  page?: number
  pageNo?: number
  pageSize?: number
  PageIndex?: number
  PageSize?: number
}
export interface CollectPrepayAdvanceDepositParams {
  encounterId: string
  patientId: string
  amount: number | string
  paymentDetails: Array<{ accountId?: string; amount: number | string; payChannelCode: string }>
  paymentCode?: string
  paymentDraftId?: string
  clientScanResult?: {
    merTradeNo: string
    tradeNo: string
    payType: string
    respCode: string
    respMsg: string
    txnAmtFen?: number
  }
  operatorId?: string
  remark?: string
}
export interface CollectPrepayAdvanceDepositResult {
  accountBalance: number | string
  paymentId: string
}
export interface PreparePrepayAdvanceDepositResult { paymentDraftId: string }
export interface RefundPrepayAdvanceDepositParams {
  encounterId: string
  patientId: string
  originalPaymentId?: string
  refundAmount: number | string
  paymentDetails: Array<{ accountId?: string; amount: number | string; payChannelCode: string }>
  operatorId?: string
  remark?: string
  clientRefundResult?: {
    merTradeNo: string
    tradeNo: string
    refundMerTradeNo?: string
    respCode: string
    respMsg: string
    refundAmountYuan?: number | string
  }
}
export interface RefundPrepayAdvanceDepositResult {
  accountBalance: number | string
  paymentId: string
}
export type CollectPrepayPayload = CollectPrepayAdvanceDepositParams
export type RefundPrepayPayload = RefundPrepayAdvanceDepositParams
export interface RequestContext {
  signal?: AbortSignal
}
