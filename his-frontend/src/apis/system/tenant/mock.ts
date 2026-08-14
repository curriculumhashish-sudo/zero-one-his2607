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

let tenantSeed = 1
const tenantUsersMap = new Map<string, string[]>()

const TENANTS: TenantRecord[] = [
  {
    id: 't1',
    code: 'T001',
    name: '总部租户',
    description: '集团总部默认租户',
    extraProperties: '',
    isActive: 'true',
    createdAt: '2025-01-10 09:00:00',
    updatedAt: '2025-06-20 14:30:00',
    createdBy: 'admin',
    updatedBy: 'admin'
  },
  {
    id: 't2',
    code: 'T002',
    name: '华东分公司',
    description: '华东区域运营租户',
    extraProperties: '',
    isActive: 'true',
    createdAt: '2025-02-15 10:00:00',
    updatedAt: '2025-07-01 08:00:00',
    createdBy: 'admin',
    updatedBy: 'manager'
  },
  {
    id: 't3',
    code: 'T003',
    name: '华南分公司',
    description: '',
    extraProperties: '',
    isActive: 'false',
    createdAt: '2025-03-01 11:30:00',
    updatedAt: '2025-06-15 16:00:00',
    createdBy: 'admin',
    updatedBy: 'admin'
  }
]

const ALL_USERS: TenantUserRecord[] = [
  { id: 'u1', userName: 'zhangsan', name: '张三', email: 'zs@demo.com', phone: '13800000001', roleNames: ['管理员'] },
  { id: 'u2', userName: 'lisi', name: '李四', email: 'ls@demo.com', phone: '13800000002', roleNames: ['医生'] },
  { id: 'u3', userName: 'wangwu', name: '王五', email: 'ww@demo.com', phone: '13800000003', roleNames: ['护士'] },
  { id: 'u4', userName: 'zhaoliu', name: '赵六', email: 'zl@demo.com', phone: '13800000004', roleNames: ['药师'] },
  { id: 'u5', userName: 'qianqi', name: '钱七', email: 'qq@demo.com', phone: '13800000005', roleNames: ['财务'] },
  { id: 'u6', userName: 'sunba', name: '孙八', email: 'sb@demo.com', phone: '13800000006', roleNames: ['管理员'] }
]

tenantUsersMap.set('t1', ['u1', 'u2'])
tenantUsersMap.set('t2', ['u3'])
tenantUsersMap.set('t3', [])

function delay<T>(data: T, ms = 200): Promise<T> {
  return new Promise((resolve) => setTimeout(() => resolve(data), ms))
}

function nextId(prefix: string) {
  return `${prefix}${Date.now()}${tenantSeed++}`
}

export function mockPageTenants(query: TenantQuery): Promise<TenantPageResult> {
  let filtered = [...TENANTS]
  if (query.code) filtered = filtered.filter((t) => t.code!.toLowerCase().includes(query.code!.toLowerCase()))
  if (query.keyword)
    filtered = filtered.filter(
      (t) =>
        t.name.toLowerCase().includes(query.keyword!.toLowerCase()) ||
        (t.description ?? '').toLowerCase().includes(query.keyword!.toLowerCase())
    )
  if (query.isActive) filtered = filtered.filter((t) => t.isActive === query.isActive)
  const total = filtered.length
  const start = (query.page - 1) * query.pageSize
  const list = filtered.slice(start, start + query.pageSize)
  return delay({ list, total })
}

export function mockCreateTenant(data: TenantSavePayload): Promise<TenantRecord> {
  const record: TenantRecord = {
    id: nextId('t'),
    code: data.code || '',
    name: data.name,
    description: data.description ?? '',
    extraProperties: data.extraProperties,
    isActive: data.isActive || 'true',
    createdAt: new Date().toLocaleString('zh-CN'),
    updatedAt: new Date().toLocaleString('zh-CN'),
    createdBy: 'current_user',
    updatedBy: 'current_user'
  }
  TENANTS.push(record)
  tenantUsersMap.set(record.id, [])
  return delay(record)
}

export function mockUpdateTenant(id: string, data: TenantSavePayload): Promise<TenantRecord> {
  const idx = TENANTS.findIndex((t) => t.id === id)
  if (idx < 0) throw new Error('租户不存在')
  const existing = TENANTS[idx]!
  const merged: TenantRecord = {
    ...existing,
    name: data.name,
    description: data.description ?? '',
    extraProperties: data.extraProperties ?? '',
    isActive: data.isActive || 'true',
    updatedAt: new Date().toLocaleString('zh-CN')
  }
  TENANTS[idx] = merged
  return delay(merged)
}

export function mockRemoveTenant(id: string): Promise<void> {
  const idx = TENANTS.findIndex((t) => t.id === id)
  if (idx < 0) throw new Error('租户不存在')
  TENANTS.splice(idx, 1)
  tenantUsersMap.delete(id)
  return delay(undefined)
}

export function mockLoadTenantOptions(): Promise<TenantOption[]> {
  const options = TENANTS.filter((t) => t.isActive === 'true').map((t) => ({ id: t.id, name: t.name, code: t.code }))
  return delay(options)
}

export function mockPageTenantUsers(tenantId: string, query: TenantUserQuery): Promise<TenantUserPageResult> {
  const memberIds = tenantUsersMap.get(tenantId) ?? []
  let list = ALL_USERS.filter((u) => memberIds.includes(u.id))
  if (query.keyword)
    list = list.filter(
      (u) =>
        u.name.toLowerCase().includes(query.keyword!.toLowerCase()) ||
        u.userName.toLowerCase().includes(query.keyword!.toLowerCase())
    )
  const total = list.length
  const start = (query.page - 1) * query.pageSize
  return delay({ list: list.slice(start, start + query.pageSize), total })
}

export function mockAddTenantUsers(payload: TenantAddUsersPayload): Promise<void> {
  const ids = tenantUsersMap.get(payload.tenantId) ?? []
  // payload.users 为 [{username, name}]，这里转成内部 id 存储
  const newUserIds = payload.users.map((u) => u.username)
  const merged = Array.from(new Set([...ids, ...newUserIds]))
  tenantUsersMap.set(payload.tenantId, merged)
  return delay(undefined)
}

export function mockRemoveTenantUsers(payload: TenantRemoveUsersPayload): Promise<void> {
  const ids = tenantUsersMap.get(payload.tenantId) ?? []
  tenantUsersMap.set(
    payload.tenantId,
    ids.filter((id) => !payload.usernames.includes(id))
  )
  return delay(undefined)
}

export function mockSearchAvailableUsers(keyword: string): Promise<TenantUserRecord[]> {
  const list = keyword
    ? ALL_USERS.filter(
        (u) =>
          u.name.toLowerCase().includes(keyword.toLowerCase()) ||
          u.userName.toLowerCase().includes(keyword.toLowerCase())
      )
    : ALL_USERS
  return delay(list)
}
