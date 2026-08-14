import { toC7PageResult, toMutationResult, unwrap } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { ServiceDeviceBindingQuery, ServiceDeviceBindingSavePayload } from './type'

const baseUrl = '/c7-configuration-api/consumable'
const validBindingTypes = ['USAGE', 'ACTIVITY'] as const
const validTargetTables = ['adm_device_definition', 'wkf_activity_definition'] as const
const validUnitCodes = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '14', '71'] as const
const validStatusEnums = ['ACTIVE', 'INACTIVE'] as const

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
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/add-consumable-binding`, normalizeBindingPayload(data))))
}

export async function updateServiceDeviceBinding(id: string, data: ServiceDeviceBindingSavePayload) {
  return toMutationResult(unwrap(await useHttp().put<string>(`${baseUrl}/modify-consumable-binding`, { id, ...normalizeBindingPayload(data) })))
}

export async function removeServiceDeviceBinding(id: string) {
  unwrap(await useHttp().delete(`${baseUrl}/remove-consumable-binding`, [id]))
}

function normalizeBindingPayload(data: ServiceDeviceBindingSavePayload): ServiceDeviceBindingSavePayload {
  const payload = {
    ...data,
    typeCode: data.typeCode?.trim(),
    sourceId: data.sourceId?.trim(),
    devActTable: data.devActTable?.trim(),
    devActId: data.devActId?.trim(),
    unitCode: data.unitCode?.trim(),
    statusEnum: data.statusEnum?.trim()
  }
  if (!validBindingTypes.includes(payload.typeCode as typeof validBindingTypes[number])) throw new Error('绑定类型无效')
  if (!validTargetTables.includes(payload.devActTable as typeof validTargetTables[number])) throw new Error('目标表无效')
  if (!validUnitCodes.includes(payload.unitCode as typeof validUnitCodes[number])) throw new Error('单位编码无效')
  if (!validStatusEnums.includes(payload.statusEnum as typeof validStatusEnums[number])) throw new Error('状态无效')
  if (!payload.sourceId || !payload.devActId || Number(payload.quantity) <= 0) throw new Error('来源 ID、目标 ID 和数量不能为空')
  return payload
}

export type * from './type'
