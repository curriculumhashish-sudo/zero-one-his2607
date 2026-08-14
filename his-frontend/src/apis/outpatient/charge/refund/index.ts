import * as mock from './mock'
import * as c1 from '@/apis/c1/charge-client'
const useC1 = import.meta.env.VITE_CHARGE_PROVIDER === 'c1'
export const pageRefundPatientQueue = (useC1 ? c1.c1PageRefundPatientQueue : mock.mockPageRefundPatientQueue) as typeof mock.mockPageRefundPatientQueue
export const getRefundableExecutionDetail = (useC1 ? c1.c1GetRefundableExecutionDetail : mock.mockGetRefundableExecutionDetail) as typeof mock.mockGetRefundableExecutionDetail
export const verifyOutpatientRefund = (useC1 ? c1.c1VerifyOutpatientRefund : mock.mockVerifyOutpatientRefund) as typeof mock.mockVerifyOutpatientRefund
export const refundOutpatientCharge = (useC1 ? c1.c1RefundOutpatientCharge : mock.mockRefundOutpatientCharge) as typeof mock.mockRefundOutpatientCharge
export const switchRetainedItemsInsuranceMode = (useC1 ? c1.c1SwitchRetainedItemsInsuranceMode : mock.mockSwitchRetainedItemsInsuranceMode) as typeof mock.mockSwitchRetainedItemsInsuranceMode
export const queryRefundResult = (useC1 ? c1.c1QueryRefundResult : mock.mockQueryRefundResult) as typeof mock.mockQueryRefundResult
export const recoverRefund = (useC1 ? c1.c1RecoverRefund : mock.mockRecoverRefund) as typeof mock.mockRecoverRefund
export type * from './type'
