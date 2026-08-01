export interface RegisterOption {
  label: string
  value: string
  disabled?: boolean
}

export interface RegisterInitResult {
  departmentOptions: RegisterOption[]
  serviceTypeOptions: RegisterOption[]
  contractOptions: RegisterOption[]
  paymentChannelOptions?: RegisterOption[]
  genderOptions: RegisterOption[]
  practitionerOptions: RegisterOption[]
  contractCategoryById?: Record<string, string>
  contractInsuranceProfileById?: Record<
    string,
    {
      busNo?: string
      categoryCode?: string
      contractId?: string
      treatmentAreaCode?: string
      ybFlag?: boolean
    }
  >
  serviceTypeFeeById?: Record<string, number | string>
}

export interface RegisterContractMatchDto {
  blockRegister?: boolean
  hintMessage?: string
  matchStatus?: 'CROSS_PROVINCE' | 'IN_PROVINCE' | 'MATCH' | 'UNKNOWN' | string
  patientInsuplcAdmdvs?: string
  recommendedCategoryCode?: string
  recommendedContractId?: string
  recommendedContractName?: string
  selectedContractId?: string
  selectedTreatmentAreaCode?: string
}

export interface YbInsuInfoPreviewDto {
  balc?: number | string
  cvlservFlag?: string
  empName?: string
  insuplcAdmdvs?: string
  insutype?: string
  psnInsuDate?: string
  psnInsuStas?: string
  psnType?: string
}

export interface YbTreatmentInfoPreviewDto {
  begndate?: string
  enddate?: string
  fundPayType?: string
  psnNo?: string
  trtChkRslt?: string
  trtChkType?: string
  trtEnjymntFlag?: string
}

export interface YbOpspFilingPreviewDto {
  begndate?: string
  enddate?: string
  opspDiseCode?: string
  opspDiseName?: string
}

export interface YbCumulativeInfoPreviewDto {
  cum?: number | string
  cumTypeCode?: string
  cumYm?: string
  insutype?: string
  year?: string
}

export interface YbRegisterBenefitAreaHintDto {
  areaKind?: 'CROSS_PROVINCE' | 'IN_PROVINCE' | 'LOCAL' | 'UNKNOWN' | string
  hintMessage?: string
  insuplcAdmdvs?: string
  mdtrtareaAdmdvs?: string
}

export interface YbRegisterBenefitPreviewDto {
  areaHint?: YbRegisterBenefitAreaHintDto
  balc?: number | string
  brdy?: string
  certno?: string
  cumulativeInfoList?: YbCumulativeInfoPreviewDto[]
  gend?: string
  insuinfoList?: YbInsuInfoPreviewDto[]
  insuplcAdmdvs?: string
  insutype?: string
  opspFilingList?: YbOpspFilingPreviewDto[]
  psnName?: string
  psnNo?: string
  treatmentBlocked?: boolean
  treatmentInfoList?: YbTreatmentInfoPreviewDto[]
}

export interface LoadRegisterBenefitPreviewParams {
  contractId: number | string
  insuplcAdmdvs?: string
  insutype?: string
  medType?: string
  patientId?: null | number | string
  psnNo?: string
  runCumulativeQuery?: boolean
  runOpspFilingQuery?: boolean
  runTreatmentCheck?: boolean
  studentId?: null | number | string
  ybCardInfo?: string
  ybCardSn?: string
  ybCertno?: string
  ybMediumType?: string
  ybPsnName?: string
}

export interface LoadRegisterBenefitPreviewResult {
  benefitKind?: 'INSURANCE' | 'NONE' | 'STUDENT' | string
  contractMatch?: RegisterContractMatchDto
  studentEligibility?: StudentEligibilityPreviewDto
  studentPreview?: StudentRegisterBenefitPreviewDto
  ybPreview?: YbRegisterBenefitPreviewDto
}

export interface StudentRecordPreviewDto {
  college?: string
  grade?: string
  leaveTimeText?: string
  major?: string
  name?: string
  physicalExamResult?: number
  schoolStatus?: number
  studentId?: number | string
  studentNo?: string
}

export interface StudentEligibilityPreviewDto {
  eligible?: boolean
  patientId?: number | string
  reasonCode?: string
  reasonMessage?: string
  studentId?: number | string
}

export interface StudentRegisterBenefitPreviewDto {
  idCard?: string
  patientId?: number | string
  patientName?: string
  phone?: string
  studentRecords?: StudentRecordPreviewDto[]
}

export interface RegisterPatient {
  age?: null | number
  birthDate?: string
  genderCode?: string
  genderText?: string
  idCardNoMasked?: string
  name: string
  patientBusNo: string
  patientId: string
  phoneMasked?: string
}

export interface RegisterPatientQuery {
  keyword?: string
  page?: number
  pageNo?: number
  pageSize?: number
  PageIndex?: number
  PageSize?: number
}

export interface RegisterPatientPage {
  records: RegisterPatient[]
  total: number | string
}

export interface LoadOutpatientRegisterInitQuery {
  organizationId?: null | number | string
  practitionerId?: null | number | string
  registrationDate?: string
  keyword?: string
}

export interface RegisterPractitionerQuery {
  keyword?: string
  organizationId?: null | number | string
}

export interface RegisterPractitionerResult {
  practitionerOptions: RegisterOption[]
}

export interface BookableSlotHint {
  remainingCount?: null | number
  slotEnd?: string
  slotId?: null | number | string
  slotStart?: string
}

export interface BookableSlotQuery {
  organizationId: number | string
  registrationDate?: string
  practitionerId?: null | number | string
  healthcareServiceId?: null | number | string
}

export interface BookableSlotResult {
  hints: BookableSlotHint[]
}

export interface RegisterHistoryQuery {
  dateFrom: string
  dateTo: string
  keyword?: string
  page?: number
  pageNo?: number
  pageSize?: number
  PageIndex?: number
  PageSize?: number
  patientId?: null | number | string
}

export interface RegisterHistoryRecord {
  amount: number
  canCancel?: boolean
  contactPhone: string
  doctorName: string
  encounterBusNo: string
  encounterId: string
  organizationName: string
  patientGenderCode: string
  patientGenderText: string
  patientName: string
  paymentDraftId?: string
  paymentId?: string
  printCount?: null | number
  serviceTypeId: string
  serviceTypeName: string
  startTime: string
  statusEnum: string
  statusText: string
  hasLinkedAppointment?: boolean
  contractName?: string
  insuplcAdmdvs?: string
  insuplcAdmdvsName?: string
  insurancePlatformRegistered?: boolean
  insurancePreSettled?: boolean
  ybFundPaySumamt?: number
  ybAcctPay?: number
  ybPsnCashPay?: number
}

export interface RegisterHistoryPage {
  records: RegisterHistoryRecord[]
  total: number | string
}

export interface RegisterChargeItem {
  amount: number | string
  chargeItemCode: string
  chargeItemName: string
  quantity: number | string
  unitPrice: number | string
}

export interface PreSettleOutpatientRegisterPayload {
  appointmentId?: null | number | string
  birthDate: string
  contractId: number | string
  gender: string
  idCardNo: string
  name: string
  organizationId: number | string
  patientId?: null | number | string
  phone: string
  practitionerId: number | string
  serviceTypeId: number | string
  slotId?: null | number | string
  ybRegisterFee?: boolean
  ybUseAcctPay?: boolean
  psnNo?: string
  insuplcAdmdvs?: string
  insutype?: string
  ybMediumType?: string
  ybCardInfo?: string
  medType?: string
}

export interface PreSettleOutpatientRegisterResult {
  appointmentId?: null | number | string
  chargeItems: RegisterChargeItem[]
  encounterId?: number | string
  paymentDraftId: number | string
  slotId?: null | number | string
  totalAmount: number | string
  insurancePreSettled?: boolean
  ybFundPaySumamt?: number | string
  ybAcctPay?: number | string
  ybPsnCashPay?: number | string
}

export interface RegisterPaymentDetail {
  payAmount: number | string
  payChannelCode: string
  transactionNo?: string
}

export interface ClientScanResultDto {
  merTradeNo: string
  tradeNo: string
  payType: string
  respCode: string
  respMsg: string
  txnAmtFen?: number
}

export interface ConfirmOutpatientRegisterPayload {
  appointmentId?: null | number | string
  paymentDetails: RegisterPaymentDetail[]
  paymentDraftId: number | string
  clientScanResult?: ClientScanResultDto
}

export interface ConfirmOutpatientRegisterResult {
  encounterId?: null | number | string
  invoiceId: number | string
  invoiceStatus?: string
  paymentId: number | string
  status: string
}

export interface RegisterCancellationValidation {
  cancelable: boolean
  reasonCode?: string
  reasonText?: string
}

export interface CancelOutpatientRegisterPayload {
  encounterId: number | string
  cancelReason: string
  refundIntent?: string
  refundRequestId: string
  clientRefundResult?: {
    merTradeNo: string
    tradeNo: string
    refundMerTradeNo?: string
    respCode: string
    respMsg: string
    refundAmountYuan?: number | string
  }
}

export interface CancelOutpatientRegisterResult {
  encounterStatus: string
  redInvoiceId?: null | number | string
  refundStatus: string
  appointmentReleaseMessage?: string
}

export interface RegisterTicketLine {
  amountText?: string
  chargeCategoryText?: string
  itemName?: string
}

export interface RegisterTicketSlip {
  accountBalanceText?: string
  accountPayText?: string
  ageText?: string
  alipayPayText?: string
  amount?: number
  cashierName?: string
  cashPayText?: string
  chargeItemLines?: RegisterTicketLine[]
  chargeTimeText?: string
  deductibleLineText?: string
  doctorName?: string
  encounterId?: null | number | string
  feeNatureText?: string
  fundPayText?: string
  genderEnumText?: string
  hospitalName?: string
  organizationName?: string
  patientName?: string
  poolingPayText?: string
  printCount?: null | number
  registerDepartmentName?: string
  serviceTypeName?: string
  slipNo?: string
  statusText?: string
  totalAmountText?: string
  wechatPayText?: string
}

export interface RegisterTicketResult {
  printCount?: null | number
  slip?: RegisterTicketSlip
}

export interface TodayRegisterSummary {
  date?: string
  totalCount?: number
  activeCount?: number
  cancelledCount?: number
}

export interface TodayRegisterSummaryQuery {
  date?: string
}

export interface ValidateRegisterCancellationPayload {
  encounterId: number | string
  reason?: string
}

export interface CancelPlannedRegistrationPayload {
  cancelReason?: string
  encounterId: number | string
}

export interface CancelPlannedRegistrationResult {
  encounterStatus: string
  insurancePlatformRevoked?: boolean
}

export interface RegisterTicketQuery {
  encounterId?: null | number | string
  paymentId?: null | number | string
  slipNo?: string
}

export interface RegisterAppointmentContext {
  appointmentBusNo: string
  appointmentId: number | string
  healthcareServiceId: number | string
  organizationId: number | string
  organizationName?: string
  practitionerId?: null | number | string
  practitionerName?: string
  serviceTypeName?: string
  slotEnd?: string
  slotId?: null | number | string
  slotStart?: string
  statusEnum?: string
}

export interface RegisterAppointmentQuery {
  appointmentBusNo?: string
  patientId?: null | number | string
  registrationDate?: string
}

export interface RequestContext {
  signal?: AbortSignal
}
