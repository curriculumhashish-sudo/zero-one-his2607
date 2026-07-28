import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface PatientRecord {
  id: string
  busNo: string
  name: string
  genderEnum: string
  genderEnumText: string
  age: number
  birthDate: string
  phone: string
  idCard: string
  nationalityCode: string
  workCompany: string
  maritalStatusEnum: string
  bloodAbo: string
  bloodRh: string
  addressProvince: string
  addressCity: string
  addressDistrict: string
  address: string
  linkName: string
  linkTelcom: string
  activeFlag: boolean
}

export interface PatientQuery extends BasicPageQuery {}
export type PatientSavePayload = Omit<PatientRecord, 'id' | 'busNo' | 'genderEnumText'>

export interface PatientNameOption {
  label: string
  value: string
}
