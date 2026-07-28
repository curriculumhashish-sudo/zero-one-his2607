import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export type WardBedForm = 'WARD' | 'HOUSE' | 'BED'

export interface WardBedRecord {
  id: string
  busNo: string
  name: string
  formEnum: WardBedForm
  formEnumText: string
  organizationId: string
  organizationName: string
  parentId: string
  parentName: string
  displayOrder: number
  statusEnum: 'ACTIVE' | 'INACTIVE'
  statusEnumText: string
  operationalEnum: 'IN_USE' | 'NOT_IN_USE'
  operationalEnumText: string
}

export interface WardBedQuery extends BasicPageQuery {}
export type WardBedSavePayload = Omit<
  WardBedRecord,
  'id' | 'busNo' | 'formEnumText' | 'organizationName' | 'parentName' | 'statusEnumText' | 'operationalEnumText'
>

export interface WardBedNameOption {
  label: string
  value: string
}
