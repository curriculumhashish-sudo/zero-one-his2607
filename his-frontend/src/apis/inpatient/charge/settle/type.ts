export type InpatientSettleQueueTab = 'PENDING_SETTLE' | 'SETTLED'
export type InpatientSettlePresentationMode = 'SUMMARY' | 'DETAIL'

export interface SettleOption {
  label: string
  value: string
  disabled?: boolean
}

export interface InpatientSettleInitResult {
  defaultPresentationMode: InpatientSettlePresentationMode
  defaultSettleQueueTab: InpatientSettleQueueTab
  paymentChannelOptions: SettleOption[]
  presentationModeOptions: SettleOption[]
  settleQueueTabOptions: SettleOption[]
}

export interface InpatientSettlePatient {
  accountBalance?: null | number | string
  accountBalanceDisplayMode?: string
  accountBalanceSubText?: string
  accountBalanceSummaryText?: string
  availableBalance?: null | number | string
  lowBalanceWarning?: boolean
  ageText?: string
  bedName?: string
  contractDisplay?: string
  contractId?: string
  encounterBusNo: string
  encounterId: string
  genderEnum?: string
  genderEnumText?: string
  lastSettleTime?: string
  organizationName?: string
  patientId: string
  patientName: string
  pendingSettlementTotal?: null | number | string
  startTime?: string
  statusEnum: string
  statusEnumText: string
  wardName?: string
}

export interface InpatientSettleQueueQuery {
  admissionDate?: string
  keyword?: string
  settleQueueTab: InpatientSettleQueueTab
  pageNo?: number
  pageSize?: number
  PageIndex?: number
  PageSize?: number
}

export interface InpatientSettleQueuePage {
  content: InpatientSettlePatient[]
  pageNo: number
  pageSize: number
  totalCount: number
  totalElements: number
}

export interface InpatientSettleSummary {
  accountBalanceDisplayMode?: string
  accountBalanceSubText?: string
  accountBalanceSummaryText?: string
  availableBalance?: number | string
  balanceHint: number | string
  bedName?: string
  displayAccountBalance: number | string
  lowBalanceWarning?: boolean
  encounterBusNo: string
  encounterId: string
  organizationName?: string
  patientId: string
  patientName: string
  pendingSettlementTotal: number | string
  prepayNetBalance: number | string
  selectedTotal: number | string
  statusEnum: string
  statusEnumText: string
  wardName?: string
}

export interface InpatientSettleChargeCategory {
  amount: number | string
  categoryCode: string
  categoryDisplay: string
  lineCount: number
}

export interface InpatientSettleChargeLine {
  categoryCode?: string
  categoryDisplay?: string
  chargeItemId: string
  itemName?: string
  lineAmount: number | string
  occurrenceTime?: string
  quantityUnit?: string
  quantityValue?: null | number | string
  selected: boolean
  sourceText?: string
  specText?: string
  statusEnum: string
  statusEnumText?: string
  unitPrice?: null | number | string
}

export interface InpatientSettleChargeQuery {
  chargeDateFrom?: string
  chargeDateTo?: string
  encounterId: string
  patientId: string
  presentationMode: InpatientSettlePresentationMode
}

export interface InpatientSettleChargeResult {
  categories?: InpatientSettleChargeCategory[]
  defaultSelectAll: boolean
  encounterId: string
  lines?: InpatientSettleChargeLine[]
  pendingSettlementTotal: number | string
  presentationMode: string
}

export interface PreSettleInpatientPayload {
  acctUsedFlag?: string
  chargeItemIds: string[]
  contractId?: string
  encounterId: string
  insuplcAdmdvs?: string
  insutype?: string
  patientId: string
  psnNo?: string
  psnSetlway?: string
  settleMode?: string
  ybCardInfo?: string
  ybMediumType?: string
}

export interface PreSettleInpatientResult {
  displayAmount?: null | number | string
  insurancePreSettled?: boolean
  paymentDraftId: string
  previewText?: string
  receivableTotal: number | string
  settlementComposition?: string
  settleMode?: string
  ybAcctPay?: null | number | string
  ybFundPaySumamt?: null | number | string
  ybMedfeeSumamt?: null | number | string
  ybPreSettleId?: string
  ybPsnCashPay?: null | number | string
}

export interface InpatientSettlePaymentDetail {
  amount: number | string
  payChannelCode: string
}

export interface ClientScanResultDto {
  merTradeNo: string
  tradeNo: string
  payType: string
  respCode: string
  respMsg: string
  txnAmtFen?: number
}

export interface ConfirmInpatientSettlePayload {
  billDate?: string
  chargeItemIds: string[]
  clientScanResult?: ClientScanResultDto
  encounterId: string
  patientId: string
  paymentDetails: InpatientSettlePaymentDetail[]
  paymentDraftId: string
  ybPreSettleId?: string
}

export interface ConfirmInpatientSettleResult {
  advanceRefundPaymentId?: string
  dischargeFullSettle?: boolean
  insuranceSettled?: boolean
  invoiceDraftId?: string
  invoiceStatus?: string
  paymentId: string
  successMessageCode?: string
  ybSetlId?: string
}

export interface InpatientSettlementPayment {
  payMethodText?: string
  paymentId: string
  receivableTotal: number | string
  recordType?: 'FORWARD' | 'REVERSE' | string
  relationId?: string
  reverseDisabledReason?: string
  reverseEnabled?: boolean
  signedAmount?: number | string
  settleTime: string
  statusEnum?: string
}

export interface InpatientSettlementHistory {
  activePaymentId: string
  categories?: InpatientSettleChargeCategory[]
  encounterId: string
  lines?: InpatientSettleChargeLine[]
  operatorName?: string
  payMethodText?: string
  payments: InpatientSettlementPayment[]
  netSettleTotal?: null | number | string
  receivableTotal?: null | number | string
  settleTime?: string
}

export interface ReverseInpatientSettleResult {
  refundPaymentId?: string
  succeeded?: boolean
}

export interface InpatientSettleSummaryQuery {
  encounterId: string
  patientId: string
  selectedChargeItemIds?: string[]
}

export interface SettledInpatientPaymentQuery {
  encounterId: string
  patientId: string
  paymentId?: string
  presentationMode?: InpatientSettlePresentationMode | string
}

export interface ReverseInpatientSettlePayload {
  encounterId: string
  patientId: string
  paymentId: string
}

export interface RequestContext {
  signal?: AbortSignal
}
