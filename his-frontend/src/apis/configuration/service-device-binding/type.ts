import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface ServiceDeviceBindingRecord {
  id: string
  serviceName: string
  serviceCategory: string
  deviceName: string
  specification: string
  quantity: number
  unit: string
  enabled: boolean
}

export interface ServiceDeviceBindingQuery extends BasicPageQuery {}
export type ServiceDeviceBindingSavePayload = Omit<ServiceDeviceBindingRecord, 'id'>
export interface ServiceDeviceBindingInitData {
  serviceCategories: string[]
  units: string[]
}
