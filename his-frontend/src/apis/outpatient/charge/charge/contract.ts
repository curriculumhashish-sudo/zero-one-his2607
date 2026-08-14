import * as legacy from './index'

export type PageChargePatientQueueContractRequest = {
  endDate?: string
  keyword?: string
  organizationId?: string
  pageIndex?: number
  pageSize?: number
  queueTabCode?: string
  startDate?: string
}

export type PageChargePatientQueueContractResponse = {
  records?: Array<{
    ageText?: string
    doctorName?: string
    encounterBusNo?: string
    encounterId?: string
    genderCode?: string
    genderEnumText?: string
    organizationName?: string
    patientId?: string
    patientName?: string
    paidAmount?: number | string
    paymentId?: string
    queueLabelCode?: string
    startTime?: string
    totalAmount?: number | string
  }>
  total?: number | string
}

export async function pageChargePatientQueueContract(
  request: PageChargePatientQueueContractRequest
): Promise<PageChargePatientQueueContractResponse> {
  return legacy.pageChargePatientQueue(
    request as never
  ) as unknown as Promise<PageChargePatientQueueContractResponse>
}

export type GetPendingChargeDetailContractRequest = {
  encounterId?: string
  includePriceExtension?: boolean
}

export type GetPendingChargeDetailContractResponse = {
  encounterId?: string
  items?: Array<{
    accountId?: string
    busNo?: string
    cashierName?: string
    cashierUserId?: string
    chargeCategoryCode?: string
    chargeCategoryDisplay?: string
    chargeItemId?: string
    contractId?: string
    discountRateDisplay?: string
    feeNatureDisplay?: string
    itemName?: string
    medicalInsuranceCode?: string
    medTypeCode?: string
    quantityDisplay?: string
    quantityUnit?: string
    quantityValue?: number | string
    serviceId?: string
    serviceTable?: string
    statusCode?: string
    statusText?: string
    totalPrice?: number | string
    unitPrice?: number | string
  }>
}

export async function getPendingChargeDetailContract(
  request: GetPendingChargeDetailContractRequest
): Promise<GetPendingChargeDetailContractResponse> {
  return legacy.getPendingChargeDetail(
    String(request.encounterId)
  ) as unknown as Promise<GetPendingChargeDetailContractResponse>
}

export type GetChargeReceiptContractRequest = {
  encounterId?: number | string
  paymentId?: number | string
}

export type GetChargeReceiptContractResponse = {
  slip?: {
    encounterId?: number | string
    patientName?: string
    hospitalName?: string
    genderEnumText?: string
    ageText?: string
    feeNatureText?: string
    chargeItemLines?: Array<{
      itemName?: string
      quantityText?: string
      amountText?: string
      chargeCategoryText?: string
    }>
    totalAmountText?: string
    poolingPayText?: string
    fundPayText?: string
    accountPayText?: string
    deductibleLineText?: string
    cashPayText?: string
    wechatPayText?: string
    alipayPayText?: string
    accountBalanceText?: string
    cashierName?: string
    chargeTimeText?: string
  }
}

export async function getChargeReceiptContract(
  request: GetChargeReceiptContractRequest
): Promise<GetChargeReceiptContractResponse> {
  return legacy.getChargeReceipt(
    request as never
  ) as unknown as Promise<GetChargeReceiptContractResponse>
}
