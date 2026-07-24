import {
  mockCreateDepartment,
  mockGetDepartmentDetail,
  mockListDepartments,
  mockLoadDepartmentFormOptions,
  mockRemoveDepartment,
  mockSetDepartmentEnabled,
  mockUpdateDepartment
} from './mock'
import type { DepartmentQuery, DepartmentSavePayload } from './type'

/** 页面只依赖本文件；后端完成后在此替换 Mock 实现即可。 */
export const listDepartments = (query: DepartmentQuery) => mockListDepartments(query)
export const getDepartmentDetail = (id: string) => mockGetDepartmentDetail(id)
export const createDepartment = (data: DepartmentSavePayload) => mockCreateDepartment(data)
export const updateDepartment = (id: string, data: DepartmentSavePayload) =>
  mockUpdateDepartment(id, data)
export const setDepartmentEnabled = (id: string, enabled: boolean) =>
  mockSetDepartmentEnabled(id, enabled)
export const removeDepartment = (id: string) => mockRemoveDepartment(id)
export const loadDepartmentFormOptions = (excludeId?: string) =>
  mockLoadDepartmentFormOptions(excludeId)

export type * from './type'
