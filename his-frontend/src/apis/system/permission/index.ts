import type { PermissionQuery, PermissionSavePayload } from './type'
import {
  mockCreatePermission,
  mockLoadPermissionNameTree,
  mockUpdatePermission,
  permissionStore
} from './mock'

/** 获取权限名称树。 */
export const loadPermissionNameTree = () => mockLoadPermissionNameTree()
/** 获取权限列表（条件 + 分页）。 */
export const pagePermissions = (query: PermissionQuery) => permissionStore.page(query)
/** 新增权限。 */
export const createPermission = (data: PermissionSavePayload) => mockCreatePermission(data)
/** 修改权限。 */
export const updatePermission = (id: string, data: PermissionSavePayload) =>
  mockUpdatePermission(id, data)
/** 删除权限。 */
export const removePermission = (id: string) => permissionStore.remove(id)

export type * from './type'
