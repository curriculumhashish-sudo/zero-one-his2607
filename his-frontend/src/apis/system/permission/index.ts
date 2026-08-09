import { unwrap, toPageResult, type BackendPage } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { PermissionQuery, PermissionRecord, PermissionSavePayload } from './type'

const baseUrl = `${import.meta.env.VITE_J1_SYSMGR_API_URL || '/sysmgr'}/permission`

/** 获取权限名称树。 */
export async function loadPermissionNameTree() {
  const tree = unwrap(await useHttp().get<PermissionRecord[]>(`${baseUrl}/tree`)) ?? []
  return flattenPermissionTree(tree).map((item) => ({ label: item.displayName, value: item.name }))
}
/** 获取权限列表（条件 + 分页）。 */
export async function pagePermissions(query: PermissionQuery) {
  const page = unwrap(
    await useHttp().get<BackendPage<PermissionRecord>>(`${baseUrl}/list`, {
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      keyword: query.keyword
    })
  )
  const result = toPageResult(page)
  result.rows = result.rows ?? []
  return result
}
/** 新增权限。 */
export async function createPermission(data: PermissionSavePayload) {
  return unwrap(await useHttp().post<PermissionRecord>(baseUrl, toCreatePayload(data)))
}
/** 修改权限。 */
export async function updatePermission(id: string, data: PermissionSavePayload) {
  return unwrap(
    await useHttp().put<PermissionRecord>(`${baseUrl}/${id}`, {
      displayName: data.displayName,
      parentName: data.parentName,
      description: data.description
    })
  )
}
/** 删除权限。 */
export async function removePermission(id: string) {
  return unwrap(await useHttp().delete(`${baseUrl}/${id}`))
}

function toCreatePayload(data: PermissionSavePayload) {
  return {
    name: data.name,
    displayName: data.displayName,
    parentName: data.parentName,
    description: data.description
  }
}

function flattenPermissionTree(rows: PermissionRecord[]): PermissionRecord[] {
  return rows.flatMap((row) => [row, ...flattenPermissionTree(row.children ?? [])])
}

export type * from './type'
