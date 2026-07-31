import * as mock from './mock'
import type { InpatientSettlePresentationMode, RequestContext } from './type'

export const loadInpatientSettleInit = mock.mockLoadInpatientSettleInit
export const pageInpatientSettleQueue = mock.mockPageInpatientSettleQueue
export const listInpatientSettleChargeCategories = mock.mockListInpatientSettleChargeCategories
export const listInpatientSettleChargeDetails = mock.mockListInpatientSettleChargeDetails
export const preSettleInpatientCharges = mock.mockPreSettleInpatientCharges
export const confirmInpatientSettle = mock.mockConfirmInpatientSettle

export function getInpatientSettleSummary(
  encounterId: string,
  patientId: string,
  selectedChargeItemIds: string[] = [],
  context: RequestContext = {}
) {
  return mock.mockGetInpatientSettleSummary(
    { encounterId, patientId, selectedChargeItemIds },
    context
  )
}

export function listInpatientSettleCharges(
  encounterId: string,
  patientId: string,
  presentationMode: InpatientSettlePresentationMode = 'DETAIL',
  context: RequestContext = {}
) {
  return mock.mockListInpatientSettleCharges({ encounterId, patientId, presentationMode }, context)
}

export function listInpatientSettlementHistory(
  encounterId: string,
  patientId: string,
  context: RequestContext = {}
) {
  return mock.mockListInpatientSettlementHistory({ encounterId, patientId }, context)
}

export function getSettledInpatientPayment(
  encounterId: string,
  patientId: string,
  settlementId: string,
  context: RequestContext = {}
) {
  return mock.mockGetSettledInpatientPayment(
    { encounterId, patientId, paymentId: settlementId },
    context
  )
}

export function reverseInpatientSettle(
  encounterId: string,
  patientId: string,
  settlementId: string,
  _reason: string,
  context: RequestContext = {}
) {
  return mock.mockReverseInpatientSettle(
    { encounterId, patientId, paymentId: settlementId },
    context
  )
}

export type * from './type'
