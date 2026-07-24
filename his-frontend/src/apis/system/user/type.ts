export interface UserQuery {
  keyword?: string
  enabled?: boolean | null
  locked?: boolean | null
  page: number
  pageSize: number
}

export interface UserRecord {
  id: string
  username: string
  realName: string
  phone: string
  email: string
  roleIds: string[]
  roleNames: string[]
  enabled: boolean
  locked: boolean
  accessFailedCount: number
  lockoutEnd: string
  createdAt: string
  updatedAt: string
}

export interface UserSavePayload {
  username: string
  realName: string
  phone: string
  email: string
  roleIds: string[]
  enabled: boolean
}

export interface RoleOption extends Record<string, unknown> {
  id: string
  name: string
}

export interface UserPageResult {
  rows: UserRecord[]
  total: number
}
