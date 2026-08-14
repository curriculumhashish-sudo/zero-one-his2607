import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface RoleRecord {
  id: string
  name: string
  displayName: string
  description: string
  permissionIds: string[]
  permissionNames: string[]
  isDefault: boolean
  code?: string
  isStatic?: boolean
  tenantId?: string
  createdAt?: string
  updatedAt?: string
}

export interface RoleQuery extends BasicPageQuery {}
export type RoleSavePayload = Omit<RoleRecord, 'id' | 'permissionNames'>
export interface RoleNameOption {
  id: string
  name: string
}
