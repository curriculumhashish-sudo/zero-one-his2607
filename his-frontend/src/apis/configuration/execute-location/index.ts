import type { ExecuteLocationQuery, ExecuteLocationSavePayload } from './type'
import {
  executeLocationStore,
  mockCreateExecuteLocation,
  mockLoadExecuteDepartmentOptions,
  mockUpdateExecuteLocation
} from './mock'

/** 获取执行科室名称。 */
export const loadExecuteDepartmentOptions = () => mockLoadExecuteDepartmentOptions()
/** 获取执行项目列表（条件 + 分页）。 */
export const pageExecuteLocations = (query: ExecuteLocationQuery) =>
  executeLocationStore.page(query)
/** 新增执行项目。 */
export const createExecuteLocation = (data: ExecuteLocationSavePayload) =>
  mockCreateExecuteLocation(data)
/** 修改执行项目。 */
export const updateExecuteLocation = (id: string, data: ExecuteLocationSavePayload) =>
  mockUpdateExecuteLocation(id, data)
/** 删除执行项目。 */
export const removeExecuteLocation = (id: string) => executeLocationStore.remove(id)

export type * from './type'
