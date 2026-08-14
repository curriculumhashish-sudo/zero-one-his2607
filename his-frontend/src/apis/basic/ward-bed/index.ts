import { useUserStore } from '@/stores/user'
import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type { WardBedForm, WardBedNameOption, WardBedQuery, WardBedRecord, WardBedSavePayload } from './type'

const baseUrl = '/c5-basicdata-api/ward'
interface BackendWardNode {
  id: string | number; name: string; busNo?: string; organizationId?: string | number; pyStr?: string; wbStr?: string
  typeJson?: string; version?: number; type?: string; formEnum?: WardBedForm; statusEnum?: string | number; children?: BackendWardNode[]
}

function organizationId(value?: string) {
  return value || String(useUserStore().departmentId || import.meta.env.VITE_DEV_ORGANIZATION_ID || '')
}
async function loadTree(id?: string) {
  return unwrap(await useHttp().get<BackendWardNode[]>(`${baseUrl}/tree-list`, { organizationId: organizationId(id) }))
}
export async function pageWardBeds(query: WardBedQuery) {
  const rows = flattenTree(await loadTree(query.organizationId))
  const pageIndex = Math.max(1, query.pageIndex); const pageSize = Math.max(1, query.pageSize)
  const filtered = query.keyword ? rows.filter((row) => row.name.includes(query.keyword!) || row.busNo.includes(query.keyword!)) : rows
  return toPageResult({ pageIndex, pageSize, total: filtered.length, pages: Math.ceil(filtered.length / pageSize), rows: filtered.slice((pageIndex - 1) * pageSize, pageIndex * pageSize) })
}
async function loadNames(form: WardBedForm, keyword: string): Promise<WardBedNameOption[]> {
  const term = keyword.trim().toLowerCase()
  return flattenTree(await loadTree()).filter((row) => row.formEnum === form && (!term || row.name.toLowerCase().includes(term))).map((row) => ({ label: row.name, value: row.id }))
}
export const loadBedNameOptions = (keyword = '') => loadNames('BED', keyword)
export const loadRoomNameOptions = (keyword = '') => loadNames('HOUSE', keyword)
export const loadWardNameOptions = (keyword = '') => loadNames('WARD', keyword)

export async function createWardBed(data: WardBedSavePayload) {
  const payload = { name: data.name, parentId: data.parentId || null, organizationId: organizationId(data.organizationId), formEnum: data.formEnum, statusEnum: 'ACTIVE', operationalEnum: data.operationalEnum || 'NOT_IN_USE', displayOrder: data.displayOrder ?? 0 }
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/location`, payload)))
}
export async function updateWardBed(id: string, data: WardBedSavePayload) {
  const path = data.formEnum === 'WARD' ? `${baseUrl}` : data.formEnum === 'HOUSE' ? '/c5-basicdata-api/room' : '/c5-basicdata-api/bed'
  return toMutationResult(unwrap(await useHttp().put<string>(path, { id, version: data.version, name: data.name, parentId: data.parentId || null, organizationId: organizationId(data.organizationId), statusEnum: data.statusEnum, pyStr: data.pyStr, wbStr: data.wbStr, typeJson: data.typeJson })))
}
export async function removeWardBed(id: string) {
  const row = (await pageWardBeds({ pageIndex: 1, pageSize: 10000 })).rows.find((item) => item.id === id)
  if (!row) throw new Error('病区、病房或床位不存在')
  const path = row.formEnum === 'WARD' ? baseUrl : row.formEnum === 'HOUSE' ? '/c5-basicdata-api/room' : '/c5-basicdata-api/bed'
  unwrap(await useHttp().delete<string>(`${path}/${id}`))
}
export async function setWardBedEnabled(id: string, enabled: boolean) {
  const row = (await pageWardBeds({ pageIndex: 1, pageSize: 10000 })).rows.find((item) => item.id === id)
  if (!row) throw new Error('病区、病房或床位不存在')
  const path = row.formEnum === 'WARD' ? `${baseUrl}/status` : row.formEnum === 'HOUSE' ? '/c5-basicdata-api/room/status' : '/c5-basicdata-api/bed/status'
  return toMutationResult(unwrap(await useHttp().put<string>(path, { id, version: row.version, statusEnum: enabled ? 'ACTIVE' : 'DISABLED' })))
}
function flattenTree(items: BackendWardNode[], parent?: WardBedRecord): WardBedRecord[] {
  return items.flatMap((item) => {
    const formEnum = item.formEnum ?? (item.type === 'ROOM' ? 'HOUSE' : item.type === 'BED' ? 'BED' : 'WARD')
    const enabled = String(item.statusEnum ?? '') === '1' || item.statusEnum === 'ACTIVE' || item.statusEnum === 'NORMAL'
    const row: WardBedRecord = { id: String(item.id), version: item.version, busNo: item.busNo ?? '', name: item.name, formEnum, formEnumText: { WARD: '病区', HOUSE: '病房', BED: '床位' }[formEnum], organizationId: item.organizationId == null ? '' : String(item.organizationId), organizationName: '', parentId: parent?.id ?? '', parentName: parent?.name ?? '', displayOrder: 0, statusEnum: enabled ? 'ACTIVE' : 'INACTIVE', statusEnumText: enabled ? '启用' : '停用', operationalEnum: 'NOT_IN_USE', operationalEnumText: '未使用', typeJson: item.typeJson, pyStr: item.pyStr, wbStr: item.wbStr, typeCode: item.type }
    return [row, ...flattenTree(item.children ?? [], row)]
  })
}
export type * from './type'
