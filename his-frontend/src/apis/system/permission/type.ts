import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface PermissionRecord {
  id: string
  name: string
  displayName: string
  parentName: string
  description: string
  providerName?: string
  grantable?: boolean
  children?: PermissionRecord[]
}

export interface PermissionQuery extends BasicPageQuery {}

export type PermissionSavePayload = Omit<PermissionRecord, 'id' | 'providerName'>

export interface PermissionNameOption {
  label: string
  value: string
}
