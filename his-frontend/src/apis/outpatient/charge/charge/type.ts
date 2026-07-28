export type ChargeQueueStatus = 'all' | 'pending' | 'settled'
export type InsuranceMode = 'insurance' | 'self-pay'
export type PaymentChannel = 'cash' | 'card' | 'wechat' | 'alipay'
export type PaymentStatus = 'pending' | 'success' | 'failed'
export type ReceiptStatus = 'pending' | 'issued' | 'failed'
export type ReceiptPrintStatus = 'auto-queued' | 'scheduled' | 'printed'

export interface PageResult<T> {
  rows: T[]
  total: number
}
export interface ChargePatient {
  id: string
  encounterId: string
  patientNo: string
  visitNo: string
  name: string
  gender: string
  age: number
  departmentName: string
  visitTime: string
  status: 'pending' | 'settled'
  paymentType: string
}
export interface ChargeItem {
  id: string
  category: string
  itemName: string
  specification: string
  quantity: number
  unitPrice: number
  amount: number
  insuranceAmount: number
  selfPayAmount: number
  status: 'pending' | 'settled'
  recordId?: string
}
export interface ChargeSummary {
  totalAmount: number
  insuranceAmount: number
  selfPayAmount: number
}
export interface ChargeDetail {
  patient: ChargePatient
  items: ChargeItem[]
  insuranceMode: InsuranceMode
  summary: ChargeSummary
  interruptedPaymentId?: string
  latestSettledRecordId?: string
  receiptWarning?: string
}
export interface ChargeQueueQuery {
  keyword?: string
  status: ChargeQueueStatus
  page: number
  pageSize: number
}
export interface PreSettlement {
  settlementId: string
  encounterId: string
  itemIds: string[]
  summary: ChargeSummary
  insuranceMode: InsuranceMode
  channels: PaymentChannel[]
}
export interface SettleChargePayload {
  settlementId: string
  encounterId: string
  itemIds: string[]
  channel: PaymentChannel
}
export interface ChargeRecord {
  id: string
  encounterId: string
  itemIds: string[]
  receiptNo: string
  amount: number
  channel: PaymentChannel
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
}
export interface RequestContext {
  signal?: AbortSignal
}
