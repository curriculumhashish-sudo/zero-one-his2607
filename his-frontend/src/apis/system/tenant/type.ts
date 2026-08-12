export interface TenantQuery {
  page: number
  pageSize: number
  code?: string
  keyword?: string
  isActive?: string
}

export interface TenantRecord {
  id: string
  code: string
  name: string
  description: string
  extraProperties?: string
  isActive: string
  createdAt: string
  updatedAt: string
  createdBy?: string
  updatedBy?: string
}

export interface TenantPageResult {
  list: TenantRecord[]
  total: number
}

export interface TenantSavePayload {
  code?: string
  name: string
  description?: string
  extraProperties?: string
  isActive: string
}

export interface TenantUserRecord {
  id: string
  userName: string
  name: string
  email?: string
  phone?: string
  roleNames?: string[]
}

export interface TenantUserPageResult {
  list: TenantUserRecord[]
  total: number
}

export interface TenantUserQuery {
  page: number
  pageSize: number
  keyword?: string
}

/**
 * 添加租户用户载荷
 * 后端 AddTenantUserDTO 要求 users: [{username, name}]
 */
export interface TenantAddUsersPayload {
  tenantId: string
  users: Array<{ username: string; name: string }>
}

/**
 * 移除租户用户载荷
 * 后端 RemoveTenantUserDTO 要求 usernames: string[]
 */
export interface TenantRemoveUsersPayload {
  tenantId: string
  usernames: string[]
}

export interface TenantOption {
  id: string
  name: string
  code: string
}
