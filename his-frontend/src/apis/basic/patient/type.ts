import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface PatientRecord {
  id: string
  /** 后端乐观锁版本；详情接口用于修改时必须保留。 */
  version?: number
  nameJson?: string
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
  addressStreet?: string
  addressExtendJson?: string
  linkName: string
  linkTelcom: string
  linkRelationCode?: number | string
  prfsEnum?: string | number
  pyStr?: string
  wbStr?: string
  nativePlace?: string
  tempFlag?: boolean
  deceasedDate?: string
  organizationId?: string
  statusEnum?: string
  activeFlag: boolean
}

export interface PatientQuery extends BasicPageQuery {}
export type PatientSavePayload = Omit<PatientRecord, 'id' | 'busNo' | 'genderEnumText'>

export interface PatientNameOption {
  label: string
  value: string
}
