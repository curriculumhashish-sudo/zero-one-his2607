export type RefundQueueStatus = 'all' | 'pending' | 'refunded'
export type RefundItemStatus = 'refundable' | 'partial' | 'refunded' | 'unavailable'
export type PaymentChannel = 'cash' | 'card' | 'wechat' | 'alipay'
export interface PageResult<T> {
  rows: T[]
  records: T[]
  content: T[]
  total: number
  totalCount: number
  pageNo: number
  pageSize: number
}
export interface RefundPatient {
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
  chargeItemStatusSummary?: string
  totalAmount?: number
  paidAmount?: number
  status: 'pending' | 'refunded'
  paymentType: string
}
export interface RefundItem {
  chargeItemId?: string
  id: string
  paymentId?: string
  paymentRecordId: string
  paymentBusNo?: string
  paymentNo: string
  itemDisplayName?: string
  itemName: string
  category: string
  quantity: number
  lineTotalAmount?: number
  amount: number
  insuranceMode: 'insurance' | 'self-pay'
  insuranceAmount: number
  selfPayAmount: number
  refundableAmount: number
  chargeItemStatusEnum?: string
  status: RefundItemStatus
  paidAt: string
  channel: PaymentChannel
}
export interface RefundDetail {
  patient: RefundPatient
  items: RefundItem[]
  interruptedRefundId?: string
  refundFailureReason?: string
  paymentGroups: RefundPaymentGroup[]
}
export interface RefundPaymentLine {
  chargeItemId: string
  chargeItemStatusEnum: string
  itemDisplayName: string
  lineTotalAmount: number
  paymentId: string
}
export interface RefundPaymentGroup {
  halfRefundOnPayment: boolean
  originalPayChannelCodes: PaymentChannel[]
  paymentBusNo: string
  paymentId: string
  paymentStatusEnum: string
  preservedLines: RefundPaymentLine[]
  refundableAmount: number
  refundingLines: RefundPaymentLine[]
}
export interface RefundQueueQuery {
  keyword?: string
  queueTabCode?: 'ALL' | 'PENDING' | 'SETTLED'
  startDate?: string | null
  endDate?: string | null
  status: RefundQueueStatus
  pageNo?: number
  page: number
  pageSize: number
}
export interface RefundVerification {
  allowed: boolean
  rejectReasonCode?: string | null
}
export interface VerifyRefundableBeforeExecutionRequest {
  encounterId: string
  refundingChargeItemIds: string[]
}
export interface ExecuteOutpatientBillRefundByPaymentRequest {
  billDate?: string | null
  clientRefundResult?: {
    merTradeNo: string
    tradeNo: string
    refundMerTradeNo?: string
    respCode: string
    respMsg: string
    refundAmountYuan?: number | string
  }
  encounterId: string
  originalPaymentId: string
  preservedNewChargeItemIds?: string[]
  refundingChargeItemIds: string[]
  refundPaymentDetails: Array<{ amount: number | string; payChannelCode: string }>
}
export interface ExecuteOutpatientBillRefundByPaymentResponse {
  halfRefundExecuted?: boolean
  insuranceRevoked?: boolean
  originalPaymentId?: string
  reversePaymentId?: string
  success: boolean
}
export interface SwitchPreservedChargeItemsRequest { encounterId: string }
export interface SwitchPreservedChargeItemsAccountResponse { success: boolean }
export type RefundPayload = ExecuteOutpatientBillRefundByPaymentRequest
export interface RefundRecord {
  reversePaymentId?: string
  halfRefundExecuted?: boolean
  insuranceRevoked?: boolean
  originalPaymentId?: string
  success?: boolean
  id: string
  encounterId: string
  paymentRecordId: string
  amount: number
  status: 'pending' | 'success' | 'failed'
  failureReason?: string
  createdAt: string
}
export interface RequestContext {
  signal?: AbortSignal
}
