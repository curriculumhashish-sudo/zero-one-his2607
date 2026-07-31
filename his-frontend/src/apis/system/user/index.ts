import {
  mockChangeUserPassword,
  mockCreateUser,
  mockGetUserDetail,
  mockLoadUserSelectOptions,
  mockLoadRoleOptions,
  mockPageUsers,
  mockPageSelectableUsers,
  mockRemoveUser,
  mockSetUserEnabled,
  mockSetUserLocked,
  mockUnlockUser,
  mockUpdateUser
} from './mock'
import type { UserQuery, UserSavePayload } from './type'

/** 页面只依赖本文件；后端完成后在此替换 Mock 实现即可。 */
export const pageUsers = (query: UserQuery) => mockPageUsers(query)
/** 获取可选择用户列表（条件 + 分页）。 */
export const pageSelectableUsers = (query: UserQuery) => mockPageSelectableUsers(query)
/** 获取用于下拉或选择表格的可选择用户。 */
export const loadUserSelectOptions = (keyword = '') => mockLoadUserSelectOptions(keyword)
export const getUserDetail = (id: string) => mockGetUserDetail(id)
export const createUser = (data: UserSavePayload) => mockCreateUser(data)
export const updateUser = (id: string, data: UserSavePayload) => mockUpdateUser(id, data)
export const removeUser = (id: string) => mockRemoveUser(id)
export const changeUserPassword = (id: string, password: string) =>
  mockChangeUserPassword(id, password)
export const setUserEnabled = (id: string, enabled: boolean) => mockSetUserEnabled(id, enabled)
/** 锁定或解锁用户；与后端的“锁定/解锁用户”接口一一对应。 */
export const setUserLocked = (id: string, locked: boolean) => mockSetUserLocked(id, locked)
/** 兼容现有页面调用。 */
export const unlockUser = (id: string) => mockUnlockUser(id)
export const loadRoleOptions = (keyword: string, signal?: AbortSignal) =>
  mockLoadRoleOptions(keyword, signal)

export type * from './type'
