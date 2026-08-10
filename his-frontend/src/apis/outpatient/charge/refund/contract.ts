import * as legacy from './index'

export type PageRefundPatientQueueContractRequest = {
  endDate?: string
  keyword?: string
  pageIndex?: number
  pageSize?: number
  queueTabCode?: string
  startDate?: string
}

export type PageRefundPatientQueueContractResponse = {
  content?: Array<{
    ageText?: string
    chargeItemStatusSummary?: string
    doctorName?: string
    encounterBusNo?: string
    encounterId?: string
    genderCode?: string
    genderEnumText?: string
    organizationName?: string
    paidAmount?: number | string
    patientId?: string
    patientName?: string
    queueLabelCode?: string
    startTime?: string
    totalAmount?: number | string
  }>
  pageIndex?: number
  pageSize?: number
  records?: Array<{
    ageText?: string
    chargeItemStatusSummary?: string
    doctorName?: string
    encounterBusNo?: string
    encounterId?: string
    genderCode?: string
    genderEnumText?: string
    organizationName?: string
    paidAmount?: number | string
    patientId?: string
    patientName?: string
    queueLabelCode?: string
    startTime?: string
    totalAmount?: number | string
  }>
  rows?: Array<{
    ageText?: string
    chargeItemStatusSummary?: string
    doctorName?: string
    encounterBusNo?: string
    encounterId?: string
    genderCode?: string
    genderEnumText?: string
    organizationName?: string
    paidAmount?: number | string
    patientId?: string
    patientName?: string
    queueLabelCode?: string
    startTime?: string
    totalAmount?: number | string
  }>
  total?: number | string
  totalCount?: number | string
}

export async function pageRefundPatientQueueContract(
  request: PageRefundPatientQueueContractRequest
): Promise<PageRefundPatientQueueContractResponse> {
  return legacy.pageRefundPatientQueue(
    request as never
  ) as unknown as Promise<PageRefundPatientQueueContractResponse>
}

export type GetRefundableExecutionDetailContractRequest = {
  encounterId?: string
  detailViewCode?: string
}

export type GetRefundableExecutionDetailContractResponse = {
  paymentGroups?: Array<{
    halfRefundOnPayment?: boolean
    originalPayChannelCodes?: string[]
    paymentBusNo?: string
    paymentId?: string
    paymentStatusEnum?: string
    preservedLines?: Array<{
      chargeItemId?: string
      chargeItemStatusEnum?: string
      itemDisplayName?: string
      lineTotalAmount?: number | string
      paymentId?: string
    }>
    refundableAmount?: number | string
    refundingLines?: Array<{
      chargeItemId?: string
      chargeItemStatusEnum?: string
      itemDisplayName?: string
      lineTotalAmount?: number | string
      paymentId?: string
    }>
  }>
}

export async function getRefundableExecutionDetailContract(
  request: GetRefundableExecutionDetailContractRequest
): Promise<GetRefundableExecutionDetailContractResponse> {
  return legacy.getRefundableExecutionDetail(
    String(request.encounterId)
  ) as unknown as Promise<GetRefundableExecutionDetailContractResponse>
}
