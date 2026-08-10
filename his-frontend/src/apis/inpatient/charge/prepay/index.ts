import * as mock from './mock'
import * as c1 from '@/apis/c1/charge-client'
const useC1 = import.meta.env.VITE_CHARGE_PROVIDER === 'c1'
export const pageInpatientQueue = (useC1 ? c1.c1PageInpatientQueue : mock.mockPageInpatientQueue) as typeof mock.mockPageInpatientQueue
export const getInpatientPrepaySummary = (useC1 ? c1.c1GetInpatientPrepaySummary : mock.mockGetInpatientPrepaySummary) as typeof mock.mockGetInpatientPrepaySummary
export const pagePrepayRecords = (useC1 ? c1.c1PagePrepayRecords : mock.mockPagePrepayRecords) as typeof mock.mockPagePrepayRecords
export const collectInpatientPrepay = (useC1 ? c1.c1CollectInpatientPrepay : mock.mockCollectInpatientPrepay) as typeof mock.mockCollectInpatientPrepay
export const refundInpatientPrepay = (useC1 ? c1.c1RefundInpatientPrepay : mock.mockRefundInpatientPrepay) as typeof mock.mockRefundInpatientPrepay
export const queryPrepayPaymentResult = (useC1 ? c1.c1QueryPrepayPaymentResult : mock.mockQueryPrepayPaymentResult) as typeof mock.mockQueryPrepayPaymentResult
export const recoverPrepaySettlement = (useC1 ? c1.c1RecoverPrepaySettlement : mock.mockRecoverPrepaySettlement) as typeof mock.mockRecoverPrepaySettlement
export const cancelPrepayPayment = (useC1 ? c1.c1CancelPrepayPayment : mock.mockCancelPrepayPayment) as typeof mock.mockCancelPrepayPayment
export type * from './type'
