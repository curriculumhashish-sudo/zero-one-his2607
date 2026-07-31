import * as mock from './mock'
import type { RequestContext } from './type'

export const loadOutpatientRegisterInit = mock.mockLoadOutpatientRegisterInit
export const pageRegisterPatientOptions = mock.mockPageRegisterPatientOptions
export const listRegisterPractitioners = mock.mockListRegisterPractitioners
export const listBookableSlotHints = mock.mockListBookableSlotHints
export const loadRegisterBenefitPreview = mock.mockLoadRegisterBenefitPreview
export const preSettleOutpatientRegister = mock.mockPreSettleOutpatientRegister
export const confirmOutpatientRegister = mock.mockConfirmOutpatientRegister
export const pageOutpatientRegisterHistory = mock.mockPageOutpatientRegisterHistory
export const getTodayRegisterSummary = mock.mockGetTodayRegisterSummary
export const cancelOutpatientRegister = mock.mockCancelOutpatientRegister
export const cancelRegisterWithRefund = mock.mockCancelRegisterWithRefund

export function getRegisterAppointment(appointmentId: string, context: RequestContext = {}) {
  return mock.mockGetRegisterAppointment({ appointmentBusNo: appointmentId }, context)
}

export function validateRegisterCancellation(encounterId: string, context: RequestContext = {}) {
  return mock.mockValidateRegisterCancellation({ encounterId }, context)
}

export function getRegisterTicket(encounterId: string, context: RequestContext = {}) {
  return mock.mockGetRegisterTicket({ encounterId }, context)
}

export type * from './type'
