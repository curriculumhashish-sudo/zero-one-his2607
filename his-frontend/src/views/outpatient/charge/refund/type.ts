export interface RefundPageQuery {
  keyword: string
  startDate: string | null
  endDate: string | null
  status: 'all' | 'pending' | 'refunded'
  page: number
  pageSize: number
}

import type { PaymentChannel } from '@/apis/outpatient/charge/refund'

export interface RefundVerificationView {
  allowed: boolean
  originalPaymentId: string
  paymentRecordId: string
  refundingChargeItemIds: string[]
  preservedNewChargeItemIds: string[]
  halfRefundOnPayment: boolean
  originalPayChannelCodes: PaymentChannel[]
  refundAmount: number
  retainedAmount: number
  channel: PaymentChannel
  message: string
}
