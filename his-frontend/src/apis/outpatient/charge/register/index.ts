import * as mock from './mock'
import type { RequestContext } from './type'
import * as c1 from '@/apis/c1/charge-client'

const useC1 = import.meta.env.VITE_CHARGE_PROVIDER === 'c1'

export const loadOutpatientRegisterInit = (useC1 ? c1.c1LoadOutpatientRegisterInit : mock.mockLoadOutpatientRegisterInit) as typeof mock.mockLoadOutpatientRegisterInit
export const pageRegisterPatientOptions = (useC1 ? c1.c1PageRegisterPatientOptions : mock.mockPageRegisterPatientOptions) as typeof mock.mockPageRegisterPatientOptions
export const listRegisterPractitioners = (useC1 ? c1.c1ListRegisterPractitioners : mock.mockListRegisterPractitioners) as typeof mock.mockListRegisterPractitioners
export const listBookableSlotHints = (useC1 ? c1.c1ListBookableSlotHints : mock.mockListBookableSlotHints) as typeof mock.mockListBookableSlotHints
export const loadRegisterBenefitPreview = (useC1 ? c1.c1LoadRegisterBenefitPreview : mock.mockLoadRegisterBenefitPreview) as typeof mock.mockLoadRegisterBenefitPreview
export const preSettleOutpatientRegister = (useC1 ? c1.c1PreSettleOutpatientRegister : mock.mockPreSettleOutpatientRegister) as typeof mock.mockPreSettleOutpatientRegister
export const confirmOutpatientRegister = (useC1 ? c1.c1ConfirmOutpatientRegister : mock.mockConfirmOutpatientRegister) as typeof mock.mockConfirmOutpatientRegister
export const pageOutpatientRegisterHistory = (useC1 ? c1.c1PageOutpatientRegisterHistory : mock.mockPageOutpatientRegisterHistory) as typeof mock.mockPageOutpatientRegisterHistory
export const getTodayRegisterSummary = (useC1 ? c1.c1GetTodayRegisterSummary : mock.mockGetTodayRegisterSummary) as typeof mock.mockGetTodayRegisterSummary
export const cancelOutpatientRegister = (useC1 ? c1.c1CancelOutpatientRegister : mock.mockCancelOutpatientRegister) as typeof mock.mockCancelOutpatientRegister
export const cancelRegisterWithRefund = (useC1 ? c1.c1CancelRegisterWithRefund : mock.mockCancelRegisterWithRefund) as typeof mock.mockCancelRegisterWithRefund

export function getRegisterAppointment(appointmentId: string, context: RequestContext = {}) {
  return (useC1 ? c1.c1GetRegisterAppointment({ appointmentBusNo: appointmentId }, context) : mock.mockGetRegisterAppointment({ appointmentBusNo: appointmentId }, context)) as ReturnType<typeof mock.mockGetRegisterAppointment>
}

export function validateRegisterCancellation(encounterId: string, context: RequestContext = {}) {
  return (useC1 ? c1.c1ValidateRegisterCancellation({ encounterId }, context) : mock.mockValidateRegisterCancellation({ encounterId }, context)) as ReturnType<typeof mock.mockValidateRegisterCancellation>
}

export function getRegisterTicket(encounterId: string, context: RequestContext = {}) {
  return (useC1 ? c1.c1GetRegisterTicket({ encounterId }, context) : mock.mockGetRegisterTicket({ encounterId }, context)) as ReturnType<typeof mock.mockGetRegisterTicket>
}

export type * from './type'
