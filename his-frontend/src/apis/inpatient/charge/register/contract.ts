import * as legacy from './index'

export type PageInpatientRegistrationQueueContractRequest = {
  idCard?: string
  keyword?: string
  registrationListStatus?: string
  registrationListStatuses?: string[]
  pageIndex?: number
  pageSize?: number
}

export type PageInpatientRegistrationQueueContractResponse = {
  content?: Array<{
    ambEncounterId?: string
    bedId?: string
    bedName?: string
    cancelable?: boolean
    encounterBusNo?: string
    encounterId?: string
    insurancePlatformRegistered?: boolean
    inWayCode?: string
    inWayCodeText?: string
    organizationId?: string
    organizationName?: string
    patientId?: string
    patientName?: string
    startTime?: string
    statusEnum?: string
    statusEnumText?: string
    wardId?: string
    wardName?: string
  }>
  pageIndex?: number
  pageSize?: number
  totalCount?: number
  totalElements?: number
}

export async function pageInpatientRegistrationQueueContract(
  request: PageInpatientRegistrationQueueContractRequest
): Promise<PageInpatientRegistrationQueueContractResponse> {
  return legacy.pageInpatientRegistrationQueue(
    request as never
  ) as unknown as Promise<PageInpatientRegistrationQueueContractResponse>
}

export type CancelInpatientRegistrationContractRequest = {
  cancelReason?: string
  encounterId?: string
  operatorId?: string
}

export type CancelInpatientRegistrationContractResponse = {
  encounterId?: string
  statusEnum?: string
  success?: boolean
}

export async function cancelInpatientRegistrationContract(
  request: CancelInpatientRegistrationContractRequest
): Promise<CancelInpatientRegistrationContractResponse> {
  return legacy.cancelInpatientRegistration(
    String(request.encounterId),
    request.cancelReason == null ? undefined : String(request.cancelReason)
  ) as unknown as Promise<CancelInpatientRegistrationContractResponse>
}

export type RevokeInsuranceAdmissionContractRequest = {
  cancelReason?: string
  encounterId?: string
  operatorId?: string
}

export type RevokeInsuranceAdmissionContractResponse = {
  encounterStatus?: string
  insurancePlatformRevoked?: boolean
}

export async function revokeInsuranceAdmissionContract(
  request: RevokeInsuranceAdmissionContractRequest
): Promise<RevokeInsuranceAdmissionContractResponse> {
  return legacy.revokeInsuranceAdmission(
    String(request.encounterId),
    request.cancelReason == null ? undefined : String(request.cancelReason)
  ) as unknown as Promise<RevokeInsuranceAdmissionContractResponse>
}
