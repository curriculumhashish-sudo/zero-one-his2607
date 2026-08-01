import * as legacy from './index'

export type PageInpatientQueueContractRequest = {
  admissionDate?: string
  keyword?: string
  PageIndex?: number
  PageSize?: number
}

export type PageInpatientQueueContractResponse = {
  content?: Array<{
    accountBalance?: number | string
    accountBalanceDisplayMode?: string
    accountBalanceSubText?: string
    accountBalanceSummaryText?: string
    availableBalance?: number | string
    ageText?: string
    bedName?: string
    contractDisplay?: string
    encounterBusNo?: string
    encounterId?: string
    genderEnum?: string
    genderEnumText?: string
    lowBalanceWarning?: boolean
    organizationName?: string
    patientId?: string
    patientName?: string
    pendingSettlementTotal?: number | string
    prepayNetBalance?: number | string
    startTime?: string
    statusEnum?: string
    statusEnumText?: string
    wardName?: string
  }>
  pageNo?: number
  pageSize?: number
  totalCount?: number
  totalElements?: number
}

export async function pageInpatientQueueContract(
  request: PageInpatientQueueContractRequest
): Promise<PageInpatientQueueContractResponse> {
  return legacy.pageInpatientQueue(
    request as never
  ) as unknown as Promise<PageInpatientQueueContractResponse>
}

export type PagePrepayRecordsContractRequest = {
  encounterId?: string
  patientId?: string
  PageIndex?: number
  PageSize?: number
}

export type PagePrepayRecordsContractResponse = {
  records?: Array<{
    amount?: number | string
    bizTypeText?: string
    operatorName?: string
    payMethodText?: string
    paymentEnum?: string
    paymentId?: string
    statusEnum?: string
    statusEnumText?: string
    ticketNo?: string
    tradeTime?: string
  }>
}

export async function pagePrepayRecordsContract(
  request: PagePrepayRecordsContractRequest
): Promise<PagePrepayRecordsContractResponse> {
  return legacy.pagePrepayRecords(String(request.encounterId), {
    PageIndex: request.PageIndex ?? 1,
    PageSize: request.PageSize ?? 200
  }) as unknown as Promise<PagePrepayRecordsContractResponse>
}
