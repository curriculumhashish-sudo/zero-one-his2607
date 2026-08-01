import * as legacy from './index'

export type GetRegisterTicketContractRequest = {
  encounterId?: number | string
  paymentId?: number | string
  slipNo?: string
}

export type GetRegisterTicketContractResponse = {
  printCount?: number
  slip?: {
    accountBalanceText?: string
    accountPayText?: string
    ageText?: string
    alipayPayText?: string
    amount?: number
    cashierName?: string
    cashPayText?: string
    chargeItemLines?: Array<{
      amountText?: string
      chargeCategoryText?: string
      itemName?: string
    }>
    chargeTimeText?: string
    deductibleLineText?: string
    doctorName?: string
    encounterId?: number | string
    feeNatureText?: string
    fundPayText?: string
    genderEnumText?: string
    hospitalName?: string
    organizationName?: string
    patientName?: string
    poolingPayText?: string
    printCount?: number
    registerDepartmentName?: string
    serviceTypeName?: string
    slipNo?: string
    statusText?: string
    totalAmountText?: string
    wechatPayText?: string
  }
}

export async function getRegisterTicketContract(
  request: GetRegisterTicketContractRequest
): Promise<GetRegisterTicketContractResponse> {
  return legacy.getRegisterTicket(
    String(request.encounterId)
  ) as unknown as Promise<GetRegisterTicketContractResponse>
}
