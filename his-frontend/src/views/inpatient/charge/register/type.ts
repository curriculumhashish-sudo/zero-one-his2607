import type { RegistrationOption, RegistrationStatus } from '@/apis/inpatient/charge/register'

export interface RegistrationInitModel {
  statusOptions: RegistrationOption[]
  paymentChannelOptions: RegistrationOption[]
  contractOptions: RegistrationOption[]
  organizationOptions: RegistrationOption[]
  inWayCodeOptions: RegistrationOption[]
  admitSourceCodeOptions: RegistrationOption[]
}

export interface RegistrationPageQuery {
  keyword: string
  idCard: string
  status: RegistrationStatus | ''
  dateRange: [string, string] | null
  pageNo: number
  pageSize: number
}

export interface RegistrationDetailModel extends Record<string, unknown> {
  patientName: string
  patientNo: string
  gender: string
  birthDate: string
  age: number | string
  phone: string
  idCard: string
  inpatientNo: string
  department: string
  ward: string
  bed: string
  outpatientDoctor: string
  attendingDoctor: string
  diagnosis: string
  patientCondition: string
  admissionSource: string
  admissionWay: string
  admissionTime: string
  status: string
}
