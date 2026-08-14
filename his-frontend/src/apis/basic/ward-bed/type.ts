import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export type WardBedForm = 'WARD' | 'HOUSE' | 'BED'

export interface WardBedRecord {
  id: string
  /** 后端乐观锁版本；树列表接口当前未返回。 */
  version?: number
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
  typeJson?: string
  pyStr?: string
  wbStr?: string
  modeEnum?: string
  typeCode?: string
}

export interface WardBedQuery extends BasicPageQuery {
  /** C5 树查询必填的所属组织 ID。 */
  organizationId?: string
}
export type WardBedSavePayload = Omit<
  WardBedRecord,
  'id' | 'busNo' | 'formEnumText' | 'organizationName' | 'parentName' | 'statusEnumText' | 'operationalEnumText'
>

export interface WardBedNameOption {
  label: string
  value: string
}
