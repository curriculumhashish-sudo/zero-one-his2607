import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface ServiceDeviceBindingRecord {
  id: string
  typeCode?: string
  typeName?: string
  sourceId?: string
  devActTable: string
  devActId: string
  devActName?: string
  quantity: number
  unitCode: string
  unitName?: string
  statusEnum: string
  statusName?: string
}

export interface ServiceDeviceBindingQuery extends BasicPageQuery {
  bindType?: string
  sourceId?: string
}

export type ServiceDeviceBindingSavePayload = Omit<ServiceDeviceBindingRecord, 'id' | 'typeName' | 'devActName' | 'unitName' | 'statusName'>

export interface ServiceDeviceBindingInitData {
  usageOptions?: Array<{ sort: string; value: string; label: string }>
  activityOptions?: Array<{ sort: string; value: string; label: string }>
}

