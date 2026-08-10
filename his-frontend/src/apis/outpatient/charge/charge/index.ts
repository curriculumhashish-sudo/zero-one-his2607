import * as mock from './mock'
import * as c1 from '@/apis/c1/charge-client'
const useC1 = import.meta.env.VITE_CHARGE_PROVIDER === 'c1'
export const pageChargePatientQueue = (useC1 ? c1.c1PageChargePatientQueue : mock.mockPageChargePatientQueue) as typeof mock.mockPageChargePatientQueue
export const getPendingChargeDetail = (useC1 ? c1.c1GetPendingChargeDetail : mock.mockGetPendingChargeDetail) as typeof mock.mockGetPendingChargeDetail
export const preSettleOutpatientCharge = (useC1 ? c1.c1PreSettleOutpatientCharge : mock.mockPreSettleOutpatientCharge) as typeof mock.mockPreSettleOutpatientCharge
export const settleOutpatientCharge = (useC1 ? c1.c1SettleOutpatientCharge : mock.mockSettleOutpatientCharge) as typeof mock.mockSettleOutpatientCharge
export const switchChargeInsuranceMode = (useC1 ? c1.c1SwitchChargeInsuranceMode : mock.mockSwitchChargeInsuranceMode) as typeof mock.mockSwitchChargeInsuranceMode
export const queryPaymentResult = (useC1 ? c1.c1QueryPaymentResult : mock.mockQueryPaymentResult) as typeof mock.mockQueryPaymentResult
export const recoverSettlement = (useC1 ? c1.c1RecoverSettlement : mock.mockRecoverSettlement) as typeof mock.mockRecoverSettlement
export const getChargeReceipt = (useC1 ? c1.c1GetChargeReceipt : mock.mockGetChargeReceipt) as typeof mock.mockGetChargeReceipt
export type * from './type'
