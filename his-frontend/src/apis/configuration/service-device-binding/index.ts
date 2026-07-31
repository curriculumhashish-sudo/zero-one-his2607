import type { ServiceDeviceBindingQuery, ServiceDeviceBindingSavePayload } from './type'
import {
  mockCreateServiceDeviceBinding,
  mockInitServiceDeviceBinding,
  mockUpdateServiceDeviceBinding,
  serviceDeviceBindingStore
} from './mock'

/** 初始化诊疗耗材绑定页面选项。 */
export const initServiceDeviceBinding = () => mockInitServiceDeviceBinding()
/** 获取绑定数据（条件 + 分页）。 */
export const pageServiceDeviceBindings = (query: ServiceDeviceBindingQuery) =>
  serviceDeviceBindingStore.page(query)
/** 添加绑定。 */
export const createServiceDeviceBinding = (data: ServiceDeviceBindingSavePayload) =>
  mockCreateServiceDeviceBinding(data)
/** 修改绑定。 */
export const updateServiceDeviceBinding = (id: string, data: ServiceDeviceBindingSavePayload) =>
  mockUpdateServiceDeviceBinding(id, data)
/** 删除绑定。 */
export const removeServiceDeviceBinding = (id: string) => serviceDeviceBindingStore.remove(id)

export type * from './type'
