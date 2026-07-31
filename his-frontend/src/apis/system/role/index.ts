import type { RoleQuery, RoleSavePayload } from './type'
import { mockCreateRole, mockLoadRoleNameOptions, mockUpdateRole, roleStore } from './mock'

/** 获取角色名称。 */
export const loadRoleNameOptions = () => mockLoadRoleNameOptions()
/** 获取角色列表（条件 + 分页）。 */
export const pageRoles = (query: RoleQuery) => roleStore.page(query)
/** 获取指定角色详情。 */
export const getRoleDetail = (id: string) => roleStore.detail(id)
/** 新增角色。 */
export const createRole = (data: RoleSavePayload) => mockCreateRole(data)
/** 修改角色。 */
export const updateRole = (id: string, data: RoleSavePayload) => mockUpdateRole(id, data)
/** 删除角色。 */
export const removeRole = (id: string) => roleStore.remove(id)

export type * from './type'
