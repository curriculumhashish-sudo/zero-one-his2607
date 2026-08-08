import { toPageResult, unwrap, type BackendPage } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { RoleQuery, RoleRecord, RoleSavePayload } from './type'

const baseUrl = `${import.meta.env.VITE_J1_SYSMGR_API_URL || '/sysmgr'}/role`

/** 获取角色名称。 */
export async function loadRoleNameOptions() {
  const rows = unwrap(
    await useHttp().get<Array<{ id: string; displayName: string }>>(`${baseUrl}/list-role-names`)
  )
  return rows.map((row) => ({ id: row.id, name: row.displayName }))
}
/** 获取角色列表（条件 + 分页）。 */
export async function pageRoles(query: RoleQuery) {
  const page = unwrap(
    await useHttp().post<BackendPage<RoleRecord>>(`${baseUrl}/query-roles`, {
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      keyword: query.keyword
    })
  )
  const result = toPageResult(page)
  result.rows = result.rows ?? []
  return result
}
/** 获取指定角色详情。 */
export async function getRoleDetail(id: string) {
  const row = unwrap(await useHttp().get<RoleRecord>(`${baseUrl}/get-role`, { id }))
  return normalizeRole(row)
}
/** 新增角色。 */
export async function createRole(data: RoleSavePayload) {
  const row = unwrap(
    await useHttp().post<RoleRecord>(`${baseUrl}/add-role`, {
      displayName: data.displayName,
      description: data.description,
      isDefault: data.isDefault
    })
  )
  return normalizeRole(row)
}
/** 修改角色。 */
export async function updateRole(id: string, data: RoleSavePayload) {
  const row = unwrap(
    await useHttp().put<RoleRecord>(`${baseUrl}/modify-role`, {
      id,
      displayName: data.displayName,
      description: data.description,
      isDefault: data.isDefault
    })
  )
  return normalizeRole(row)
}
/** 删除角色。 */
export async function removeRole(id: string) {
  return unwrap(await useHttp().delete(`${baseUrl}/remove-role`, undefined, { params: { id } }))
}

function normalizeRole(row: RoleRecord): RoleRecord {
  return {
    ...row,
    permissionIds: row.permissionIds ?? [],
    permissionNames: row.permissionNames ?? []
  }
}

export type * from './type'
