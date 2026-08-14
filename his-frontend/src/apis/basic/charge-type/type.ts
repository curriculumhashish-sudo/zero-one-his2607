import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface ChargeTypeRecord {
  id: string
  busNo: string
  name: string
  displayOrder: number
  categoryEnum: string
  categoryEnumText: string
  activeFlag: number
  activeFlagText: string
}

export interface ChargeTypeQuery extends BasicPageQuery {}
export type ChargeTypeSavePayload = Omit<ChargeTypeRecord, 'id' | 'categoryEnumText' | 'activeFlagText'>
export interface ChargeTypeNameOption {
  label: string
  value: string
}
