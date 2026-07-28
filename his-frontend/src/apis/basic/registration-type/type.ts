import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface RegistrationTypeRecord {
  id: string
  busNo: string
  name: string
  ybNo: string
  amount: number
  activeFlag: number
  activeFlagText: string
}

export interface RegistrationTypeQuery extends BasicPageQuery {}
export type RegistrationTypeSavePayload = Omit<RegistrationTypeRecord, 'id' | 'busNo' | 'activeFlagText'>
export interface RegistrationTypeNameOption {
  label: string
  value: string
}
