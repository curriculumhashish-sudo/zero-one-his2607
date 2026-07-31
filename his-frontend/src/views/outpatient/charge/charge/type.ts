export interface ChargePageQuery {
  keyword: string
  startDate: string | null
  endDate: string | null
  status: 'all' | 'pending' | 'settled'
  page: number
  pageSize: number
}

import type { ChargeSummary, PaymentChannel, PreSettleOutpatientChargesResponse } from '@/apis/outpatient/charge/charge'

export interface ChargeSettlementView extends PreSettleOutpatientChargesResponse {
  encounterId: string
  chargeItemIds: string[]
  summary: ChargeSummary
  insuranceMode: 'insurance' | 'self-pay'
  channels: PaymentChannel[]
}
