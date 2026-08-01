import {
  mockAddTenantUsers,
  mockCreateTenant,
  mockLoadTenantOptions,
  mockPageTenantUsers,
  mockPageTenants,
  mockRemoveTenant,
  mockRemoveTenantUsers,
  mockSearchAvailableUsers,
  mockUpdateTenant
} from './mock'
import type {
  TenantAddUsersPayload,
  TenantOption,
  TenantPageResult,
  TenantQuery,
  TenantRecord,
  TenantRemoveUsersPayload,
  TenantSavePayload,
  TenantUserPageResult,
  TenantUserQuery,
  TenantUserRecord
} from './type'

export const loadTenantOptions = (): Promise<TenantOption[]> => mockLoadTenantOptions()
export const pageTenants = (query: TenantQuery): Promise<TenantPageResult> => mockPageTenants(query)
export const createTenant = (data: TenantSavePayload): Promise<TenantRecord> => mockCreateTenant(data)
export const updateTenant = (id: string, data: TenantSavePayload): Promise<TenantRecord> =>
  mockUpdateTenant(id, data)
export const removeTenant = (id: string): Promise<void> => mockRemoveTenant(id)
export const pageTenantUsers = (
  tenantId: string,
  query: TenantUserQuery
): Promise<TenantUserPageResult> => mockPageTenantUsers(tenantId, query)
export const addTenantUsers = (payload: TenantAddUsersPayload): Promise<void> => mockAddTenantUsers(payload)
export const removeTenantUsers = (payload: TenantRemoveUsersPayload): Promise<void> =>
  mockRemoveTenantUsers(payload)
export const searchAvailableUsers = (keyword: string): Promise<TenantUserRecord[]> =>
  mockSearchAvailableUsers(keyword)

export type * from './type'
