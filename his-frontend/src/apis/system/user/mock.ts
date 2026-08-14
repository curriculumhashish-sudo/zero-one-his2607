import type {
  RoleOption,
  UserPageResult,
  UserQuery,
  UserRecord,
  UserSavePayload,
  UserSelectOption
} from './type'

const roles: RoleOption[] = [
  { id: 'role-admin', name: '系统管理员' },
  { id: 'role-doctor', name: '医生' },
  { id: 'role-nurse', name: '护士' },
  { id: 'role-pharmacist', name: '药师' },
  { id: 'role-finance', name: '财务人员' },
  { id: 'role-registrar', name: '挂号收费员' }
]

const roleNameMap = new Map(roles.map((role) => [role.id, role.name]))
let sequence = 8

const users: UserRecord[] = [
  createSeed('user-001', 'admin', '系统管理员', ['role-admin'], true, false),
  createSeed('user-002', 'zhangsan', '张三', ['role-doctor'], true, false),
  createSeed('user-003', 'lisi', '李四', ['role-nurse'], true, true, 5),
  createSeed('user-004', 'wangwu', '王五', ['role-pharmacist'], true, false),
  createSeed('user-005', 'zhaoliu', '赵六', ['role-finance'], false, false),
  createSeed('user-006', 'sunqi', '孙七', ['role-doctor', 'role-admin'], true, false),
  createSeed('user-007', 'zhouba', '周八', ['role-registrar'], true, false),
  createSeed('user-008', 'wujia', '吴佳', ['role-nurse'], false, false)
]

function createSeed(
  id: string,
  username: string,
  realName: string,
  roleIds: string[],
  enabled: boolean,
  locked: boolean,
  accessFailedCount = 0
): UserRecord {
  return {
    id,
    username,
    realName,
    phone: `1380000${id.replace(/\D/g, '').padStart(4, '0').slice(-4)}`,
    email: `${username}@hospital.test`,
    roleIds,
    roleNames: resolveRoleNames(roleIds),
    enabled,
    locked,
    accessFailedCount,
    lockoutEnd: locked ? '2026-07-25 09:00:00' : '',
    createdAt: '2026-07-01 08:00:00',
    updatedAt: '2026-07-01 08:00:00'
  }
}

function resolveRoleNames(roleIds: string[]) {
  return roleIds.map((id) => roleNameMap.get(id)).filter((name): name is string => Boolean(name))
}

function cloneUser(user: UserRecord): UserRecord {
  return { ...user, roleIds: [...user.roleIds], roleNames: [...user.roleNames] }
}

function requireUser(id: string) {
  const user = users.find((item) => item.id === id)
  if (!user) throw new Error('用户不存在')
  return user
}

function validatePayload(payload: UserSavePayload, currentId?: string) {
  if (!payload.username.trim()) throw new Error('用户名不能为空')
  if (!payload.realName.trim()) throw new Error('姓名不能为空')
  if (payload.roleIds.length === 0) throw new Error('请至少选择一个角色')
  const duplicate = users.some(
    (item) =>
      item.id !== currentId && item.username.toLowerCase() === payload.username.toLowerCase()
  )
  if (duplicate) throw new Error('用户名已存在')
}

function delay(ms = 60) {
  return new Promise<void>((resolve) => setTimeout(resolve, ms))
}

export async function mockPageUsers(query: UserQuery): Promise<UserPageResult> {
  await delay()
  const keyword = query.keyword?.trim().toLowerCase()
  const filtered = users.filter((user) => {
    if (
      keyword &&
      !user.username.toLowerCase().includes(keyword) &&
      !user.realName.toLowerCase().includes(keyword) &&
      !user.phone.includes(keyword)
    ) {
      return false
    }
    if (query.enabled != null && user.enabled !== query.enabled) return false
    if (query.locked != null && user.locked !== query.locked) return false
    return true
  })
  const start = (query.page - 1) * query.pageSize
  return {
    rows: filtered.slice(start, start + query.pageSize).map(cloneUser),
    total: filtered.length
  }
}

export async function mockGetUserDetail(id: string) {
  await delay()
  return cloneUser(requireUser(id))
}

export async function mockPageSelectableUsers(query: UserQuery): Promise<UserPageResult> {
  return mockPageUsers({ ...query, enabled: true, locked: false })
}

export async function mockLoadUserSelectOptions(keyword = ''): Promise<UserSelectOption[]> {
  const term = keyword.trim().toLowerCase()
  return users
    .filter(
      (user) =>
        user.enabled &&
        !user.locked &&
        (!term ||
          user.username.toLowerCase().includes(term) ||
          user.realName.toLowerCase().includes(term))
    )
    .map((user) => ({
      id: user.id,
      username: user.username,
      realName: user.realName,
      label: `${user.realName}（${user.username}）`
    }))
}

export async function mockCreateUser(payload: UserSavePayload) {
  await delay()
  validatePayload(payload)
  sequence += 1
  const now = new Date().toISOString().replace('T', ' ').slice(0, 19)
  const user: UserRecord = {
    id: `user-${String(sequence).padStart(3, '0')}`,
    username: payload.username.trim(),
    realName: payload.realName.trim(),
    phone: payload.phone.trim(),
    email: payload.email.trim(),
    roleIds: [...payload.roleIds],
    roleNames: resolveRoleNames(payload.roleIds),
    enabled: payload.enabled,
    locked: false,
    accessFailedCount: 0,
    lockoutEnd: '',
    createdAt: now,
    updatedAt: now
  }
  users.unshift(user)
  return cloneUser(user)
}

export async function mockUpdateUser(id: string, payload: UserSavePayload) {
  await delay()
  validatePayload(payload, id)
  const user = requireUser(id)
  user.username = payload.username.trim()
  user.realName = payload.realName.trim()
  user.phone = payload.phone.trim()
  user.email = payload.email.trim()
  user.roleIds = [...payload.roleIds]
  user.roleNames = resolveRoleNames(payload.roleIds)
  user.enabled = payload.enabled
  user.updatedAt = new Date().toISOString().replace('T', ' ').slice(0, 19)
  return cloneUser(user)
}

export async function mockSetUserEnabled(id: string, enabled: boolean) {
  await delay()
  const user = requireUser(id)
  if (user.username.toLowerCase() === 'admin' && !enabled) {
    throw new Error('系统管理员不能停用')
  }
  user.enabled = enabled
  user.updatedAt = new Date().toISOString().replace('T', ' ').slice(0, 19)
  return cloneUser(user)
}

export async function mockSetUserLocked(id: string, locked: boolean) {
  await delay()
  const user = requireUser(id)
  if (user.username.toLowerCase() === 'admin' && locked) {
    throw new Error('系统管理员不能锁定')
  }
  user.locked = locked
  user.accessFailedCount = locked ? Math.max(user.accessFailedCount, 5) : 0
  user.lockoutEnd = locked ? '2099-12-31 23:59:59' : ''
  user.updatedAt = new Date().toISOString().replace('T', ' ').slice(0, 19)
  return cloneUser(user)
}

/** 为已有用户页面保留的“解锁”调用，实际对应同一个锁定/解锁接口。 */
export const mockUnlockUser = (id: string) => mockSetUserLocked(id, false)

export async function mockChangeUserPassword(id: string, password: string) {
  await delay()
  const user = requireUser(id)
  if (password.length < 6) throw new Error('密码长度不能少于 6 位')
  // Mock 不保存明文密码；真实环境由后端负责加密、校验和持久化。
  user.updatedAt = new Date().toISOString().replace('T', ' ').slice(0, 19)
}

export async function mockRemoveUser(id: string) {
  await delay()
  const user = requireUser(id)
  if (user.username.toLowerCase() === 'admin') throw new Error('系统管理员不能删除')
  const index = users.findIndex((item) => item.id === id)
  users.splice(index, 1)
}

export async function mockLoadRoleOptions(keyword: string, signal?: AbortSignal) {
  await delay()
  if (signal?.aborted) throw new DOMException('请求已取消', 'AbortError')
  const normalized = keyword.trim().toLowerCase()
  return roles.filter((role) => !normalized || role.name.toLowerCase().includes(normalized))
}
