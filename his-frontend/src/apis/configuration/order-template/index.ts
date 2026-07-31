import type { OrderTemplateQuery, OrderTemplateSavePayload } from './type'
import {
  mockCreateOrderTemplate,
  mockLoadOrderTemplateNameOptions,
  mockUpdateOrderTemplate,
  orderTemplateStore
} from './mock'

/** 获取组套名称。 */
export const loadOrderTemplateNameOptions = () => mockLoadOrderTemplateNameOptions()
/** 获取组套列表（条件 + 分页）。 */
export const pageOrderTemplates = (query: OrderTemplateQuery) => orderTemplateStore.page(query)
/** 获取组套详情。 */
export const getOrderTemplateDetail = (id: string) => orderTemplateStore.detail(id)
/** 新增组套。 */
export const createOrderTemplate = (data: OrderTemplateSavePayload) => mockCreateOrderTemplate(data)
/** 修改组套。 */
export const updateOrderTemplate = (id: string, data: OrderTemplateSavePayload) =>
  mockUpdateOrderTemplate(id, data)
/** 删除组套。 */
export const removeOrderTemplate = (id: string) => orderTemplateStore.remove(id)
/** 启用或停用组套。 */
export const setOrderTemplateEnabled = (id: string, enabled: boolean) =>
  orderTemplateStore.update(id, {
    status: enabled ? 'ENABLED' : 'DISABLED',
    statusText: enabled ? '启用' : '停用'
  })

export type * from './type'
