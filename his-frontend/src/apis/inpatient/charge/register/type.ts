export type RegistrationStatus =
  | 'ADMITTED'
  | 'CANCELLED'
  | 'COMPLETED'
  | 'DISCHARGED'
  | 'ENTERED_IN_ERROR'
  | 'ON_HOLD'
  | 'PENDING'
  | 'PLANNED'
  | 'PRE_ADMISSION'
  | 'WAIT_ADMISSION'

export interface RegistrationOption {
  label: string
  value: string
  disabled?: boolean
}

export interface RegistrationQueueRecord {
  ambEncounterId?: string
  bedId?: string
  bedName?: string
  cancelable: boolean
  encounterBusNo: string
  encounterId: string
  insurancePlatformRegistered?: boolean
  inWayCode?: string
  inWayCodeText?: string
  organizationId?: string
  organizationName?: string
  patientId: string
  patientName: string
  startTime?: string
  statusEnum: RegistrationStatus
  statusEnumText: string
  wardId?: string
  wardName?: string
}

export interface RegistrationQueueQuery {
  page?: number
  pageNo?: number
  pageSize?: number
  PageIndex?: number
  PageSize?: number
  keyword?: string
  idCard?: string
  registrationListStatus?: RegistrationStatus | ''
  registrationListStatuses?: RegistrationStatus[]
}

/** 任务书要求但原型请求 DTO 暂未提供的 Mock 演示筛选。 */
export interface RegistrationQueueMockFilter {
  dateFrom?: string
  dateTo?: string
}

export interface RegistrationPageResult {
  content: RegistrationQueueRecord[]
  pageNo: number
  pageSize: number
  totalCount: number
  totalElements: number
}

export interface RegistrationPatient {
  birthDate?: string
  genderEnum?: string
  idCard?: string
  name?: string
  patientId?: string
  phone?: string
}

/** 原型复用 outpatientRegistration/pagePatientsForSelect 的患者行。 */
export interface RegistrationPatientOption {
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

export interface RegistrationEncounter {
  admitSourceCode?: string
  admitSourceCodeText?: string
  ambEncounterId?: string
  encounterBusNo?: string
  encounterId?: string
  inWayCode?: string
  inWayCodeText?: string
  organizationId?: string
  organizationName?: string
  outpatientDoctorName?: string
  primaryConditionDefinitionId?: string
  primaryConditionDefinitionName?: string
  priorityEnum?: string
  startTime?: string
  statusEnum?: RegistrationStatus
  wardId?: string
  wardName?: string
  bedId?: string
  bedName?: string
  attendingDoctorId?: string
  attendingDoctorName?: string
}

export interface RegistrationContext {
  admitSourceCodeOptions?: RegistrationOption[]
  contractOptions: RegistrationOption[]
  organizationOptions?: RegistrationOption[]
  inWayCodeOptions?: RegistrationOption[]
  encounter: RegistrationEncounter | null
  patient: RegistrationPatient | null
  readOnly: boolean
}

export interface RegistrationPageInitResult {
  paymentChannelOptions?: RegistrationOption[]
}

export interface RegistrationPatientQuery {
  keyword?: string
  page?: number
  pageNo?: number
  pageSize?: number
  PageIndex?: number
  PageSize?: number
}

export interface RegistrationPatientPage {
  records: RegistrationPatientOption[]
  total: number | string
}

export interface WardOption {
  wardCode?: string
  wardId: string
  wardName: string
}

export interface WardCapacity {
  totalBedCount: number | string
  vacantBedCount: number | string
  wardId: string
}

export interface RegistrationContextQuery {
  encounterId?: string
  organizationId?: string
}

export interface RegistrationWardQuery {
  organizationId: string
}

export interface WardCapacityQuery {
  wardId: string
}

export interface AttendingDoctorQuery {
  organizationId?: string
  wardLocationId?: string
}

export interface AttendingDoctorResult {
  doctorOptions?: RegistrationOption[]
  primaryNurseOptions?: RegistrationOption[]
}

export interface AdmissionDiagnosisPayload {
  conditionDefinitionId?: string
  conditionId?: string
  description?: string
  primaryFlag: boolean
}

export interface RegistrationPaymentDetail {
  accountId?: string
  amount: number | string
  payChannelCode: string
}

export interface SubmitRegistrationPayload {
  admissionDiagnoses?: AdmissionDiagnosisPayload[]
  admitSourceCode?: string
  advanceDepositAmount?: number | string
  attendingDoctorId?: string
  birthDate?: string
  contractId: string
  encounterId?: string
  genderEnum?: string
  idCard?: string
  insuplcAdmdvs?: string
  insutype?: string
  inWayCode?: string
  medType?: string
  operatorId: string
  organizationId: string
  patientId?: string
  patientName?: string
  paymentDetails?: RegistrationPaymentDetail[]
  phone?: string
  priorityEnum?: string
  psnNo?: string
  startTime: string
  updatePatientFromIdCard?: boolean
  wardId: string
  ybCardInfo?: string
  ybMediumType?: string
}

export interface SubmitRegistrationResult {
  accountOpened?: boolean
  encounterBusNo: string
  encounterId: string
  paymentId?: string
  statusEnum: RegistrationStatus
  ybMdtrtId?: string
}

export interface CancelRegistrationResult {
  encounterId: string
  statusEnum: RegistrationStatus
  success: boolean
}

export interface CancelRegistrationPayload {
  cancelReason?: string
  encounterId: string
  operatorId: string
}

export interface RevokeInsuranceAdmissionResult {
  encounterStatus?: RegistrationStatus
  insurancePlatformRevoked?: boolean
}

export interface RequestContext {
  signal?: AbortSignal
}
