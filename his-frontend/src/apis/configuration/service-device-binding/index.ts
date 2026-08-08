import { toC7PageResult, toMutationResult, unwrap } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { ServiceDeviceBindingQuery, ServiceDeviceBindingSavePayload } from './type'

const baseUrl = '/c7-configuration-api/consumable'

export async function initServiceDeviceBinding() {
  return unwrap(await useHttp().get(`${baseUrl}/init-binding`))
}

export async function pageServiceDeviceBindings(query: ServiceDeviceBindingQuery) {
  const page = unwrap(await useHttp().get(`${baseUrl}/query-all-binding`, {
    pageNo: query.pageIndex,
    pageSize: query.pageSize,
    bindType: query.bindType,
    sourceId: query.sourceId
  }))
  return toC7PageResult(page, query.pageIndex, query.pageSize)
}

export async function createServiceDeviceBinding(data: ServiceDeviceBindingSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/add-consumable-binding`, data)))
}

export async function updateServiceDeviceBinding(id: string, data: ServiceDeviceBindingSavePayload) {
  return toMutationResult(unwrap(await useHttp().put<string>(`${baseUrl}/modify-consumable-binding`, { id, ...data })))
}

export async function removeServiceDeviceBinding(id: string) {
  unwrap(await useHttp().delete(`${baseUrl}/remove-consumable-binding`, [id]))
}

export type * from './type'
