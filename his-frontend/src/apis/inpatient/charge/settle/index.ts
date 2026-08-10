import * as mock from './mock'
import * as c1 from '@/apis/c1/charge-client'
import type { InpatientSettlePresentationMode, RequestContext } from './type'
const useC1 = import.meta.env.VITE_CHARGE_PROVIDER === 'c1'

export const loadInpatientSettleInit = (useC1 ? c1.c1LoadInpatientSettleInit : mock.mockLoadInpatientSettleInit) as typeof mock.mockLoadInpatientSettleInit
export const pageInpatientSettleQueue = (useC1 ? c1.c1PageInpatientSettleQueue : mock.mockPageInpatientSettleQueue) as typeof mock.mockPageInpatientSettleQueue
export const listInpatientSettleChargeCategories = (useC1 ? c1.c1ListInpatientSettleChargeCategories : mock.mockListInpatientSettleChargeCategories) as typeof mock.mockListInpatientSettleChargeCategories
export const listInpatientSettleChargeDetails = (useC1 ? c1.c1ListInpatientSettleChargeDetails : mock.mockListInpatientSettleChargeDetails) as typeof mock.mockListInpatientSettleChargeDetails
export const preSettleInpatientCharges = (useC1 ? c1.c1PreSettleInpatientCharges : mock.mockPreSettleInpatientCharges) as typeof mock.mockPreSettleInpatientCharges
export const confirmInpatientSettle = (useC1 ? c1.c1ConfirmInpatientSettle : mock.mockConfirmInpatientSettle) as typeof mock.mockConfirmInpatientSettle

export function getInpatientSettleSummary(
  encounterId: string,
  patientId: string,
  selectedChargeItemIds: string[] = [],
  context: RequestContext = {}
) {
  return (useC1 ? c1.c1GetInpatientSettleSummary(encounterId, patientId, selectedChargeItemIds, context) : mock.mockGetInpatientSettleSummary({ encounterId, patientId, selectedChargeItemIds }, context)) as ReturnType<typeof mock.mockGetInpatientSettleSummary>
}

export function listInpatientSettleCharges(
  encounterId: string,
  patientId: string,
  presentationMode: InpatientSettlePresentationMode = 'DETAIL',
  context: RequestContext = {}
) {
  return (useC1 ? c1.c1ListInpatientSettleCharges(encounterId, patientId, presentationMode, context) : mock.mockListInpatientSettleCharges({ encounterId, patientId, presentationMode }, context)) as ReturnType<typeof mock.mockListInpatientSettleCharges>
}

export function listInpatientSettlementHistory(
  encounterId: string,
  patientId: string,
  context: RequestContext = {}
) {
  return (useC1 ? c1.c1ListInpatientSettlementHistory(encounterId, patientId, context) : mock.mockListInpatientSettlementHistory({ encounterId, patientId }, context)) as ReturnType<typeof mock.mockListInpatientSettlementHistory>
}

export function getSettledInpatientPayment(
  encounterId: string,
  patientId: string,
  settlementId: string,
  context: RequestContext = {}
) {
  return (useC1 ? c1.c1GetSettledInpatientPayment(encounterId, patientId, settlementId, context) : mock.mockGetSettledInpatientPayment({ encounterId, patientId, paymentId: settlementId }, context)) as ReturnType<typeof mock.mockGetSettledInpatientPayment>
}

export function reverseInpatientSettle(
  encounterId: string,
  patientId: string,
  settlementId: string,
  _reason: string,
  context: RequestContext = {}
) {
  return (useC1 ? c1.c1ReverseInpatientSettle(encounterId, patientId, settlementId, _reason, context) : mock.mockReverseInpatientSettle({ encounterId, patientId, paymentId: settlementId }, context)) as ReturnType<typeof mock.mockReverseInpatientSettle>
}

export type * from './type'
