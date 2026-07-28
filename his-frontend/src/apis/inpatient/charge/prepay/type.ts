export type PrepayChannel = 'cash' | 'card' | 'wechat' | 'alipay'
export type PrepayRecordStatus = 'processing' | 'settled' | 'refunded' | 'cancelled'
export interface PageResult<T> {
  rows: T[]
  total: number
}
export interface InpatientPatient {
  id: string
  encounterId: string
  patientNo: string
  visitNo: string
  name: string
  gender: string
  age: number
  departmentName: string
  wardName: string
  bedNo: string
  admissionDate: string
  status: 'in-hospital'
  paymentType: string
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
  id: string
  encounterId: string
  type: 'collect' | 'refund'
  amount: number
  channel: PrepayChannel
  status: PrepayRecordStatus
  transactionNo: string
  createdAt: string
  refundableAmount: number
}
export interface InpatientQueueQuery {
  keyword?: string
  page: number
  pageSize: number
}
export interface PrepayRecordQuery {
  page: number
  pageSize: number
}
export interface CollectPrepayPayload {
  encounterId: string
  amount: number
  channel: PrepayChannel
}
export interface RefundPrepayPayload {
  encounterId: string
  recordId: string
  amount: number
  channel: PrepayChannel
}
export interface RequestContext {
  signal?: AbortSignal
}
