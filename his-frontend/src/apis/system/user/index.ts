import { toPageResult, unwrap, type BackendPage } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { UserQuery, UserRecord, UserSavePayload } from './type'

const baseUrl = '/sysmgr/user'

/** 页面只依赖本文件；后端完成后在此替换 Mock 实现即可。 */
export async function pageUsers(query: UserQuery) {
  const page = unwrap(
    await useHttp().post<BackendPage<any>>(`${baseUrl}/page`, {
      pageIndex: query.page,
      pageSize: query.pageSize,
      keyword: query.keyword,
      isActive: query.enabled == null ? undefined : query.enabled ? 1 : 0,
      isLockedOut: query.locked == null ? undefined : query.locked ? 1 : 0
    })
  )
  const result = toPageResult(page)
  result.rows = result.rows.map(normalizeUser)
  return result
}
/** 获取可选择用户列表（条件 + 分页）。 */
export const pageSelectableUsers = pageUsers
/** 获取用于下拉或选择表格的可选择用户。 */
export async function loadUserSelectOptions(keyword = '') {
  return pageUsers({ keyword, enabled: true, locked: false, page: 1, pageSize: 20 })
}
export async function getUserDetail(id: string) {
  return normalizeUser(unwrap(await useHttp().get<any>(`${baseUrl}/${id}`)))
}
export async function createUser(data: UserSavePayload): Promise<any> {
  return unwrap(await useHttp().post<boolean>(baseUrl, toUserPayload(data, true)))
}
export async function updateUser(id: string, data: UserSavePayload): Promise<any> {
  return unwrap(
    await useHttp().put<boolean>(`${baseUrl}/update`, { id, ...toUserPayload(data, false) })
  )
}
export async function removeUser(id: string) {
  return unwrap(await useHttp().delete<boolean>(`${baseUrl}/${id}`))
}
export async function changeUserPassword(id: string, password: string) {
  return unwrap(
    await useHttp().put<boolean>(`${baseUrl}/changePassword`, { id, passwordHash: password })
  )
}
export async function setUserEnabled(id: string, enabled: boolean): Promise<any> {
  return unwrap(
    await useHttp().put<boolean>(`${baseUrl}/updateStatus/${id}/status/${enabled ? 1 : 0}`)
  )
}
export async function setUserLocked(id: string, locked: boolean): Promise<any> {
  return unwrap(
    await useHttp().put<boolean>(`${baseUrl}/lockUser/${id}/lockStatus/${locked ? 1 : 0}`)
  )
}
export const unlockUser = (id: string) => setUserLocked(id, false)
export async function loadRoleOptions(keyword: string, _signal?: AbortSignal) {
  const rows = unwrap(
    await useHttp().get<Array<{ id: string; displayName: string }>>('/sysmgr/role/list-role-names')
  )
  return rows
    .filter((row) => !keyword || row.displayName.includes(keyword))
    .map((row) => ({ id: row.id, name: row.displayName }))
}

function toUserPayload(data: UserSavePayload, creating: boolean) {
  return {
    username: data.username,
    ...(creating && { password: data.password }),
    roleIds: data.roleIds,
    name: data.realName,
    phoneNumber: data.phone,
    email: data.email,
    genderEnum: data.genderEnum ?? 0,
    isActive: data.enabled ? 1 : 0
  }
}

function normalizeUser(row: any): UserRecord {
  return {
    ...row,
    realName: row.realName ?? row.name ?? '',
    phone: row.phone ?? row.phoneNumber ?? '',
    enabled: row.enabled ?? row.isActive === 1,
    locked: row.locked ?? row.isLockedOut === 1,
    roleIds: Array.isArray(row.roleIds) ? row.roleIds : [],
    roleNames: Array.isArray(row.roleNames) ? row.roleNames : [],
    lockoutEnd: row.lockoutEnd ?? ''
  }
}

export type * from './type'
