import {
  mockCreateUser,
  mockGetUserDetail,
  mockLoadRoleOptions,
  mockPageUsers,
  mockSetUserEnabled,
  mockUnlockUser,
  mockUpdateUser
} from './mock'
import type { UserQuery, UserSavePayload } from './type'

/** 页面只依赖本文件；后端完成后在此替换 Mock 实现即可。 */
export const pageUsers = (query: UserQuery) => mockPageUsers(query)
export const getUserDetail = (id: string) => mockGetUserDetail(id)
export const createUser = (data: UserSavePayload) => mockCreateUser(data)
export const updateUser = (id: string, data: UserSavePayload) => mockUpdateUser(id, data)
export const setUserEnabled = (id: string, enabled: boolean) => mockSetUserEnabled(id, enabled)
export const unlockUser = (id: string) => mockUnlockUser(id)
export const loadRoleOptions = (keyword: string, signal?: AbortSignal) =>
  mockLoadRoleOptions(keyword, signal)

export type * from './type'
