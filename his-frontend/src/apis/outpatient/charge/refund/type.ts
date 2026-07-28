export type RefundQueueStatus = 'all' | 'pending' | 'refunded'
export type RefundItemStatus = 'refundable' | 'partial' | 'refunded' | 'unavailable'
export type PaymentChannel = 'cash' | 'card' | 'wechat' | 'alipay'
export interface PageResult<T> {
  rows: T[]
  total: number
}
export interface RefundPatient {
  id: string
  encounterId: string
  patientNo: string
  visitNo: string
  name: string
  gender: string
  age: number
  departmentName: string
  visitTime: string
  status: 'pending' | 'refunded'
  paymentType: string
}
export interface RefundItem {
  id: string
  paymentRecordId: string
  paymentNo: string
  itemName: string
  category: string
  quantity: number
  amount: number
  insuranceMode: 'insurance' | 'self-pay'
  insuranceAmount: number
  selfPayAmount: number
  refundableAmount: number
  status: RefundItemStatus
  paidAt: string
  channel: PaymentChannel
}
export interface RefundDetail {
  patient: RefundPatient
  items: RefundItem[]
  interruptedRefundId?: string
  refundFailureReason?: string
}
export interface RefundQueueQuery {
  keyword?: string
  status: RefundQueueStatus
  page: number
  pageSize: number
}
export interface RefundVerification {
  verificationId: string
  paymentRecordId: string
  itemIds: string[]
  refundAmount: number
  retainedAmount: number
  channel: PaymentChannel
  message: string
}
export interface RefundPayload {
  verificationId: string
  encounterId: string
  paymentRecordId: string
  itemIds: string[]
  channel: PaymentChannel
  retainedMode?: 'insurance' | 'self-pay'
}
export interface RefundRecord {
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
